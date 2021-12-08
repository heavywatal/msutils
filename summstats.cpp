#include <iostream>

#include <Sequence/variant_matrix/msformat.hpp>
#include <Sequence/summstats.hpp>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cerr.tie(nullptr);
    std::cout.setf(std::ios::fixed);
    std::cout.precision(6);
    std::cout << "pi\tS\tD\tthetaH\tH\n";
    do {
        const auto vm = Sequence::from_msformat(std::cin);
        Sequence::AlleleCountMatrix ac(vm);
        const double pi = Sequence::thetapi(ac);
        const double tH = Sequence::thetah(ac, 0);
        std::cout << pi << '\t'
                  << Sequence::nvariable_sites(ac) << '\t'
                  << Sequence::tajd(ac) << '\t'
                  << tH << '\t'
                  << pi - tH << '\n';
    } while (!std::cin.eof());
    return 0;
}
