#pragma once
#ifndef WTL_FST_HPP_
#define WTL_FST_HPP_

#include <Sequence/AlleleCountMatrix.hpp>

#include <cassert>
#include <ostream>
#include <vector>

namespace wtl {

template<class T> inline
T perm2(const T x) {return x * (x - 1);}

template<class T> inline
T comb2(const T x) {return perm2(x) / 2;}

inline size_t
pairwise_diff(const Sequence::AlleleCountMatrix& ac1, const Sequence::AlleleCountMatrix& ac2) {
    assert(ac1.nrow == ac2.nrow);
    size_t num = 0;
    for (size_t i=0; i<ac1.nrow; ++i) {
        num += ac1.counts[ac1.ncol * i] * ac2.counts[ac2.ncol * i + 1];
        num += ac1.counts[ac1.ncol * i + 1] * ac2.counts[ac2.ncol * i];
    }
    return num;
}

class FST {
  public:
    FST(std::vector<Sequence::AlleleCountMatrix>&& matrices)
    : matrices_(std::forward<std::vector<Sequence::AlleleCountMatrix>>(matrices)),
      npop_(matrices_.size())
    {
        size_t sum_npair_ii = 0;
        size_t sum_npair_ij = 0;
        double d_sw = 0.0;
        double d_bw = 0.0;
        for (size_t i=0; i<npop_; ++i) {
            const auto& ac_i = matrices_[i];
            const auto npair_ii = perm2(ac_i.nsam);
            const double d_i = pairwise_diff(ac_i, ac_i);
            nsam_ += ac_i.nsam;
            sum_npair_ii += npair_ii;
            pi_s_ += d_i / static_cast<double>(npair_ii);
            d_sw += d_i;
            for (size_t j=0; j<i; ++j) {
                const auto& ac_j = matrices_[j];
                const auto npair_ij = ac_i.nsam * ac_j.nsam;
                const auto d_ij = pairwise_diff(ac_i, ac_j);
                sum_npair_ij += npair_ij;
                pi_b_ += d_ij / static_cast<double>(npair_ij);
                d_bw += d_ij;
            }
        }
        assert((sum_npair_ii + 2 * sum_npair_ij) == perm2(nsam_));
        pi_s_ /= npop_;
        pi_b_ /= comb2(npop_);
        pi_sw_ = d_sw / sum_npair_ii;
        pi_bw_ = d_bw / sum_npair_ij;
        pi_tw_ = (d_sw + 2.0 * d_bw) / perm2(nsam_);
    }
    double HSM() const {
        // Estimation of Levels of Gene Flow From DNA Sequence Data
        return 1.0 - pi_s() / pi_b();
    }
    double HSM_weighted() const {
        return 1.0 - pi_sw() / pi_bw();
    }
    double HBK() const {
        // A Statistical Test for Detecting Geographic Subdivision
        return 1.0 - pi_s() / pi_t();
    }
    double HBK_weighted() const {
        // the most powerful test is obtainded when weighted by sample size
        return 1.0 - pi_sw() / pi_tw();
    }
    double HBK_libsequence() const {
        // bad approximation, but keep this for compatibility
        return 1.0 - pi_s() / pi_t(true);
    }

    std::ostream& write_debug(std::ostream& ost) const {
        ost << std::endl;
        ost << "S: " << pi_s() << "\tSw: " << pi_sw() << std::endl;
        ost << "B: " << pi_b() << "\tBw: " << pi_bw() << std::endl;
        ost << "T: " << pi_t() << "\tTw: " << pi_tw() << "\tTl: " << pi_t(true) << std::endl;
        ost << "D: " << pi_d() << "\tDw: " << pi_dw() << std::endl;
        ost << "FST: " << HSM()
            << "\tFSTw: " << HSM_weighted() << std::endl;
        ost << "KST: " << HBK()
            << "\tKSTw: " << HBK_weighted()
            << "\tKSTl: " << HBK_libsequence() << std::endl;
        return ost;
    }

    size_t nsam() const {return nsam_;}
    double pi_s() const {return pi_s_;}
    double pi_b() const {return pi_b_;}
    double pi_t(const bool approx=false) const {
        const double to0 = approx ? 0.0 : static_cast<double>(npop_) / nsam_;
        return ((1.0 - to0) / (npop_ - to0)) * pi_s_ + ((npop_ - 1.0) / (npop_ - to0)) * pi_b_;
    }
    double pi_d() const {return pi_b_ - pi_s_;}
    double pi_sw() const {return pi_sw_;}
    double pi_bw() const {return pi_bw_;}
    double pi_tw() const {return pi_tw_;}
    double pi_dw() const {return pi_bw_ - pi_sw_;}
  private:

    const std::vector<Sequence::AlleleCountMatrix> matrices_;
    const size_t npop_;
    size_t nsam_ = 0;
    double pi_s_ = 0.0;
    double pi_b_ = 0.0;
    double pi_sw_ = 0.0;
    double pi_bw_ = 0.0;
    double pi_tw_ = 0.0;
};

} // namespace wtl

#endif /* WTL_IOSTR_HPP_ */
