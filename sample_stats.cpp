#include <iostream>
#include <string>
#include <vector>

#include <Sequence/SimData.hpp>
#include <Sequence/PolySIM.hpp>
#include <Sequence/FST.hpp>

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
        std::cin >> std::ws;
        std::vector<std::string> samples;
        samples.reserve(ms.nsam);
        for (unsigned i=0u; i<ms.nsam; ++i) {
            std::getline(std::cin, buffer);
            samples.push_back(buffer);
        }
        std::cin >> std::ws;
        std::getline(std::cin, buffer);
        auto begit = samples.begin();
        for (const auto n: ms.sample_sizes) {
            auto endit = begit + n;
            Sequence::SimData data(positions, {begit, endit});
            Sequence::PolySIM poly(&data);
            std::cout << poly.ThetaPi() << '\t'
                      << poly.NumPoly() << '\t'
                      << poly.TajimasD() << '\t'
                      << poly.ThetaH();
            if (endit != samples.end()) {std::cout << '\t';}
            begit = endit;
        }
        if (ms.npop() > 1u) {
            Sequence::SimData data(positions, samples);
            Sequence::FST fst(&data, static_cast<unsigned>(ms.npop()), ms.sample_sizes.data());
            std::cout << '\t' << fst.HSM();
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
    if (ms.nrep >= 10000u) std::cerr << std::endl;
    return 0;
}
