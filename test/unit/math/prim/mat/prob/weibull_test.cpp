#include <gtest/gtest.h>
#include <boost/math/distributions.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <stan/math/prim/mat.hpp>
#include <test/unit/math/prim/mat/prob/vector_rng_test_helper.hpp>
#include <limits>
#include <vector>

class WeibullTestRig : public VectorRealRNGTestRig {
 public:
  WeibullTestRig()
      : VectorRealRNGTestRig(10000, 10, {0.5, 1.0, 1.3, 2.0}, {1, 2, 3},
                             {-2.5, -1.7, -0.1, 0.0}, {-3, -2, -1, 0},
                             {0.1, 1.0, 1.7, 2.1}, {1, 2, 3, 4},
                             {-2.7, -1.5, -0.5, 0.0}, {-3, -2, -1, 0}) {}

  template <typename T1, typename T2, typename T3, typename T_rng>
  auto generate_samples(const T1& alpha, const T2& sigma, const T3&,
                        T_rng& rng) const {
    return stan::math::weibull_rng(alpha, sigma, rng);
  }

  std::vector<double> generate_quantiles(double alpha, double sigma,
                                         double) const {
    std::vector<double> quantiles;
    double K = boost::math::round(2 * std::pow(N_, 0.4));
    boost::math::weibull_distribution<> dist(alpha, sigma);

    for (int i = 1; i < K; ++i) {
      double frac = i / K;
      quantiles.push_back(quantile(dist, frac));
    }
    quantiles.push_back(std::numeric_limits<double>::max());

    return quantiles;
  }
};

TEST(ProbDistributionsWeibull, errorCheck) {
  check_dist_throws_all_types(WeibullTestRig());
}

TEST(ProbDistributionsWeibull, distributionTest) {
  check_quantiles_real_real(WeibullTestRig());
}
