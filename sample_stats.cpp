// -*- mode: c++; coding: utf-8 -*-
#include <iostream>
#include <string>
#include <vector>

#include <Sequence/SimData.hpp>
#include <Sequence/PolySIM.hpp>
#include <Sequence/FST.hpp>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cerr.tie(nullptr);
    std::cout.setf(std::ios::fixed);
    std::cout.precision(6);
    std::string dev_null;
    unsigned int nsam, nrep;
    std::cin >> dev_null >> nsam >> nrep;
    std::cerr << dev_null << " " << nsam << " " << nrep;
    std::vector<unsigned int> sample_sizes{nsam};
    while (std::cin >> dev_null) {
        if (dev_null == "-I") {
            unsigned int npop;
            std::cin >> npop;
            std::cerr << " -I " << npop;
            sample_sizes.resize(npop);
            for (auto& n_i: sample_sizes) {
                std::cin >> n_i;
                std::cerr << " " << n_i;
            }
        }
        if (dev_null == "//") {break;}
    }
    std::cerr << std::endl;
    std::string line;
    if (sample_sizes.size() > 1) {
        for (size_t i=1; i<=sample_sizes.size(); ++i) {
            std::cout << "pi_" << i << '\t'
                      <<  "S_" << i << '\t'
                      <<  "D_" << i << '\t'
                      << "tH_" << i << '\t';
        }
        std::cout << "Fst\tKst\n";
    } else {
        std::cout << "pi\tS\tD\ttH\n";
    }
    for (unsigned int irep=1; irep<=nrep; ++irep) {
        if (irep > 1000 && (irep % 100 == 0)) {
            std::cerr << "\r" << irep << " / " << nrep << std::flush;
        }
        unsigned int segsites;
        std::cin >> dev_null;
        std::cin >> segsites;
        std::vector<double> positions(segsites);
        std::cin >> dev_null;
        for (unsigned int i=0; i<segsites; ++i) {
            std::cin >> positions[i];
        }
        std::cin >> std::ws;
        std::vector<std::string> samples;
        samples.reserve(nsam);
        for (unsigned int i=0; i<nsam; ++i) {
            std::getline(std::cin, line);
            samples.push_back(line);
        }
        std::cin >> std::ws;
        std::getline(std::cin, dev_null);
        auto begit = samples.begin();
        for (const auto n: sample_sizes) {
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
        if (sample_sizes.size() > 1) {
            Sequence::SimData data(positions, samples);
            Sequence::FST fst(&data, sample_sizes.size(), sample_sizes.data());
            std::cout << fst.HSM() << '\t' << fst.HBK();
        }
        std::cout << '\n';
    }
    std::cerr << std::endl;
    return 0;
}
