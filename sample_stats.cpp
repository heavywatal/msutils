#include "fst.hpp"

#include <Sequence/variant_matrix/msformat.hpp>
#include <Sequence/summstats.hpp>

#include <iostream>
#include <string>
#include <vector>

inline std::vector<double>
read_positions(std::istream& ist) {
    std::string buffer;
    unsigned segsites;
    while (buffer != "segsites:") {
        ist >> buffer;
    }
    ist >> segsites;
    ist >> buffer;
    std::vector<double> positions(segsites);
    for (unsigned i=0u; i<segsites; ++i) {
        ist >> positions[i];
    }
    ist >> std::ws;
    return positions;
}

inline std::vector<Sequence::VariantMatrix::value_type>
read_haplotypes(std::istream& ist, const unsigned nsam, const unsigned segsites) {
    char c;
    std::vector<Sequence::VariantMatrix::value_type> data(nsam * segsites);
    for (size_t i=0u; i<nsam; ++i) {
        for (size_t j=0u; j<segsites; ++j) {
            ist >> c;
            if (c == '1') {
                data[i + nsam * j] = 1; // row-major to col-major
            }
        }
        ist >> std::ws;
    }
    return data;
}

inline std::vector<Sequence::AlleleCountMatrix>
read_subpops(std::istream& ist, const std::vector<unsigned>& sample_sizes, const std::vector<double>& positions) {
    std::vector<Sequence::AlleleCountMatrix> matrices;
    matrices.reserve(sample_sizes.size());
    for (const auto nsam: sample_sizes) {
        matrices.emplace_back(
            Sequence::VariantMatrix(read_haplotypes(ist, nsam, positions.size()), positions)
        );
    }
    return matrices;
}

inline std::ostream&
write_header(std::ostream& ost, const unsigned npop) {
    if (npop > 1u) {
        for (size_t i=1u; i<=npop; ++i) {
            ost << "pi_" << i << '\t'
                <<  "S_" << i << '\t'
                <<  "D_" << i << '\t'
                << "tH_" << i << '\t';
        }
        ost << "Fst\n";
    } else {
        ost << "pi\tS\tD\ttH\n";
    }
    return ost;
}

inline std::ostream&
write_sample_stats(std::ostream& ost, const Sequence::AlleleCountMatrix& ac) {
    ost << Sequence::thetapi(ac) << '\t'
        << Sequence::nvariable_sites(ac) << '\t'
        << Sequence::tajd(ac) << '\t'
        << Sequence::thetah(ac, 0);
    return ost;
}

inline std::ostream&
write_sample_stats(std::ostream& ost, const std::vector<Sequence::AlleleCountMatrix>& matrices) {
    bool is_first = true;
    for (const auto& ac: matrices) {
        if (is_first) {
            is_first = false;
        } else {
            ost << '\t';
        }
        write_sample_stats(ost, ac);
    }
    return ost;
}

struct MsCommand {
    MsCommand(std::istream& ist) {
        std::string buffer;
        ist >> buffer >> nsam >> nrep;
        std::cerr << buffer << " " << nsam << " " << nrep;
        sample_sizes.push_back(nsam);
        while (ist >> buffer) {
            if (buffer == "//") break;
            std::cerr << ' ' << buffer;
            if (buffer == "-I") {
                unsigned npop;
                ist >> npop;
                sample_sizes.resize(npop);
                for (auto& n_i: sample_sizes) {
                    ist >> n_i;
                }
            }
        }
        std::cerr << std::endl;
    }
    size_t npop() const {return sample_sizes.size();}
    unsigned nsam;
    unsigned nrep;
    std::vector<unsigned> sample_sizes;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cerr.tie(nullptr);
    std::cout.setf(std::ios::fixed);
    std::cout.precision(6);
    std::string buffer;
    MsCommand ms(std::cin);
    write_header(std::cout, ms.npop());
    for (unsigned irep=1u; irep<=ms.nrep; ++irep) {
        if (ms.nrep >= 10000u && (irep % 1000u == 0u)) {
            std::cerr << "\r" << irep << " / " << ms.nrep << std::flush;
        }
        const auto positions = read_positions(std::cin);
        auto matrices = read_subpops(std::cin, ms.sample_sizes, positions);
        write_sample_stats(std::cout, matrices);
        if (ms.npop() > 1u) {
            wtl::FST fst(std::move(matrices));
            std::cout << '\t' << fst.HSM();
            // fst.write_debug(std::cout);
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
    if (ms.nrep >= 10000u) std::cerr << std::endl;
    return 0;
}
