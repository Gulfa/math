#ifndef STAN_VARIATIONAL_ADVI_PARAMS_MEANFIELD__HPP
#define STAN_VARIATIONAL_ADVI_PARAMS_MEANFIELD__HPP

#include <stan/math/prim/mat/fun/Eigen.hpp>
#include <stan/math/prim/scal/fun/max.hpp>
#include <stan/math/prim/scal/meta/constants.hpp>

#include <stan/math/prim/mat/meta/get.hpp>
#include <stan/math/prim/arr/meta/get.hpp>
#include <stan/math/prim/mat/meta/length.hpp>
#include <stan/math/prim/mat/meta/is_vector.hpp>
#include <stan/math/prim/mat/meta/is_vector_like.hpp>
#include <stan/math/prim/mat/fun/value_of_rec.hpp>

#include <stan/math/prim/scal/err/check_size_match.hpp>
#include <stan/math/prim/scal/err/check_not_nan.hpp>
#include <vector>

namespace stan {

  namespace variational {

    class advi_params_meanfield {
    private:
      Eigen::VectorXd mu_;           // Mean vector
      Eigen::VectorXd sigma_tilde_;  // Log standard deviation vector
      int dimension_;

    public:
      advi_params_meanfield(const Eigen::VectorXd& mu,
                            const Eigen::VectorXd& sigma_tilde) :
      mu_(mu), sigma_tilde_(sigma_tilde), dimension_(mu.size()) {
        static const char* function =
          "stan::variational::advi_params_meanfield";

        stan::math::check_size_match(function,
                             "Dimension of mean vector", dimension_,
                             "Dimension of std vector", sigma_tilde_.size() );
        for (int i = 0; i < dimension_; ++i) {
          stan::math::check_not_nan(function, "Mean vector", mu_(i));
          stan::math::check_not_nan(function, "Sigma tilde vector",
                                              sigma_tilde_(i));
        }
      }

      // Accessors
      int dimension() const { return dimension_; }
      const Eigen::VectorXd& mu()          const { return mu_; }
      const Eigen::VectorXd& sigma_tilde() const { return sigma_tilde_; }

      // Mutators
      void set_mu(const Eigen::VectorXd& mu) {
        static const char* function =
          "stan::variational::advi_params_meanfield::set_mu";

        stan::math::check_size_match(function,
                               "Dimension of input vector", mu.size(),
                               "Dimension of current vector", dimension_);
        stan::math::check_not_nan(function, "Input vector", mu);
        mu_ = mu;
      }

      void set_sigma_tilde(const Eigen::VectorXd& sigma_tilde) {
        static const char* function =
          "stan::variational::advi_params_meanfield::set_sigma_tilde";

        stan::math::check_size_match(function,
                               "Dimension of input vector", sigma_tilde.size(),
                               "Dimension of current vector", dimension_);
        stan::math::check_not_nan(function, "Input vector", sigma_tilde);
        sigma_tilde_ = sigma_tilde;
      }

      // Entropy of normal:
      // 0.5 * dim * (1+log2pi) + 0.5 * log det diag(sigma^2) =
      // 0.5 * dim * (1+log2pi) + sum(log(sigma)) =
      // 0.5 * dim * (1+log2pi) + sum(sigma_tilde)
      double entropy() const {
        return 0.5 * static_cast<double>(dimension_) *
               (1.0 + stan::prob::LOG_TWO_PI) + sigma_tilde_.sum();
      }

      // Implement f^{-1}(\check{z}) = sigma * \check{z} + \mu
      Eigen::VectorXd loc_scale_transform(const Eigen::VectorXd& z_check) const {
        static const char* function = "stan::variational::advi_params_meanfield"
                                      "::loc_scale_transform";

        stan::math::check_size_match(function,
                         "Dimension of mean vector", dimension_,
                         "Dimension of input vector", z_check.size() );
        stan::math::check_not_nan(function, "Input vector", z_check);

        // exp(sigma_tilde) * z_check + mu
        return z_check.array().cwiseProduct(sigma_tilde_.array().exp())
               + mu_.array();
      }
    };
  }  // variational
}  // stan

#endif