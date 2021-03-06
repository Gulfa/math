#include <stan/math/rev/mat.hpp>
#include <gtest/gtest.h>
#include <test/unit/math/rev/mat/fun/expect_matrix_eq.hpp>
#include <test/unit/math/rev/mat/util.hpp>
#include <vector>

using Eigen::Dynamic;
using Eigen::Matrix;
using stan::math::diag_pre_multiply;
using stan::math::var;

TEST(MathMatrix, diagPreMultiply2_vv) {
  Matrix<var, Dynamic, Dynamic> m(3, 3);
  m << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<var, Dynamic, 1> v(3);
  v << 1, 2, 3;

  Matrix<var, Dynamic, Dynamic> v_m(3, 3);
  v_m << 1, 0, 0, 0, 2, 0, 0, 0, 3;

  Matrix<var, Dynamic, Dynamic> v_m_times_m = v_m * m;

  expect_matrix_eq(v_m_times_m, diag_pre_multiply(v, m));

  Matrix<var, 1, Dynamic> rv(3);
  rv << 1, 2, 3;

  expect_matrix_eq(v_m_times_m, diag_pre_multiply(rv, m));
}

TEST(MathMatrix, diagPreMultiply2_vd) {
  Matrix<var, Dynamic, Dynamic> m1(3, 3);
  m1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<double, Dynamic, 1> v(3);
  v << 1, 2, 3;

  Matrix<var, Dynamic, Dynamic> v_m(3, 3);
  v_m << 1, 0, 0, 0, 2, 0, 0, 0, 3;

  Matrix<var, Dynamic, Dynamic> v_m_times_m1 = v_m * m1;

  Matrix<var, Dynamic, Dynamic> m2(3, 3);
  m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  expect_matrix_eq(v_m_times_m1, diag_pre_multiply(v, m2));
}

TEST(MathMatrix, diagPreMultiply2_dv) {
  Matrix<double, Dynamic, Dynamic> m1(3, 3);
  m1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<var, Dynamic, 1> v(3);
  v << 1, 2, 3;

  Matrix<var, Dynamic, Dynamic> m2(3, 3);
  m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<var, Dynamic, Dynamic> v_m(3, 3);
  v_m << 1, 0, 0, 0, 2, 0, 0, 0, 3;

  Matrix<var, Dynamic, Dynamic> v_m_times_m2 = v_m * m2;

  expect_matrix_eq(v_m_times_m2, diag_pre_multiply(v, m1));
}

TEST(MathMatrix, diagPreMultiplyGrad_vv) {
  Matrix<var, Dynamic, Dynamic> m1(3, 3);
  m1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<var, Dynamic, 1> v(3);
  v << 1, 2, 3;

  std::vector<var> xs1;
  for (int i = 0; i < 9; ++i)
    xs1.push_back(m1(i));
  for (int i = 0; i < 3; ++i)
    xs1.push_back(v(i));

  Matrix<var, Dynamic, Dynamic> v_pre_multipy_m1 = diag_pre_multiply(v, m1);

  var norm1 = v_pre_multipy_m1.norm();

  std::vector<double> g1;
  norm1.grad(xs1, g1);

  Matrix<var, Dynamic, Dynamic> m2(3, 3);
  m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<var, Dynamic, Dynamic> v_m(3, 3);
  v_m << 1, 0, 0, 0, 2, 0, 0, 0, 3;

  std::vector<var> xs2;
  for (int i = 0; i < 9; ++i)
    xs2.push_back(m2(i));
  for (int i = 0; i < 3; ++i)
    xs2.push_back(v_m(i, i));

  Matrix<var, Dynamic, Dynamic> v_m_times_m = v_m * m2;

  var norm2 = v_m_times_m.norm();
  std::vector<double> g2;
  norm2.grad(xs2, g2);

  EXPECT_EQ(g1.size(), g2.size());
  for (size_t i = 0; i < g1.size(); ++i)
    EXPECT_FLOAT_EQ(g1[i], g2[i]);
}

TEST(MathMatrix, diagPreMultiplyGrad_vd) {
  Matrix<var, Dynamic, Dynamic> m1(3, 3);
  m1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<double, Dynamic, 1> v(3);
  v << 1, 2, 3;

  std::vector<var> xs1;
  for (int i = 0; i < 9; ++i)
    xs1.push_back(m1(i));

  Matrix<var, Dynamic, Dynamic> v_pre_multipy_m1 = diag_pre_multiply(v, m1);

  var norm1 = v_pre_multipy_m1.norm();

  std::vector<double> g1;
  norm1.grad(xs1, g1);

  Matrix<var, Dynamic, Dynamic> m2(3, 3);
  m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  // OK to use var, just for comparison
  Matrix<var, Dynamic, Dynamic> v_m(3, 3);
  v_m << 1, 0, 0, 0, 2, 0, 0, 0, 3;

  std::vector<var> xs2;
  for (int i = 0; i < 9; ++i)
    xs2.push_back(m2(i));

  Matrix<var, Dynamic, Dynamic> v_m_times_m = v_m * m2;

  var norm2 = v_m_times_m.norm();
  std::vector<double> g2;
  norm2.grad(xs2, g2);

  EXPECT_EQ(g1.size(), g2.size());
  for (size_t i = 0; i < g1.size(); ++i)
    EXPECT_FLOAT_EQ(g1[i], g2[i]);
}

TEST(MathMatrix, diagPreMultiplyGrad_dv) {
  Matrix<double, Dynamic, Dynamic> m1(3, 3);
  m1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<var, Dynamic, 1> v(3);
  v << 1, 2, 3;

  std::vector<var> xs1;
  for (int i = 0; i < 3; ++i)
    xs1.push_back(v(i));

  Matrix<var, Dynamic, Dynamic> v_pre_multipy_m1 = diag_pre_multiply(v, m1);

  var norm1 = v_pre_multipy_m1.norm();

  std::vector<double> g1;
  norm1.grad(xs1, g1);

  Matrix<var, Dynamic, Dynamic> m2(3, 3);
  m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  // OK to use var, just for comparison
  Matrix<var, Dynamic, Dynamic> v_m(3, 3);
  v_m << 1, 0, 0, 0, 2, 0, 0, 0, 3;

  std::vector<var> xs2;
  for (int i = 0; i < 3; ++i)
    xs2.push_back(v_m(i, i));

  Matrix<var, Dynamic, Dynamic> v_m_times_m = v_m * m2;

  var norm2 = v_m_times_m.norm();
  std::vector<double> g2;
  norm2.grad(xs2, g2);

  EXPECT_EQ(g1.size(), g2.size());
  for (size_t i = 0; i < g1.size(); ++i)
    EXPECT_FLOAT_EQ(g1[i], g2[i]);
}

TEST(MathMatrix, diagPreMultiplyException) {
  Matrix<var, Dynamic, Dynamic> m(2, 2);
  m << 2, 3, 4, 5;
  EXPECT_THROW(diag_pre_multiply(m, m), std::invalid_argument);

  Matrix<var, Dynamic, 1> v(3);
  v << 1, 2, 3;
  EXPECT_THROW(diag_pre_multiply(v, m), std::invalid_argument);
}

TEST(AgradRevMatrix, check_varis_on_stack) {
  stan::math::matrix_v m(3, 3);
  m << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  stan::math::vector_v v(3);
  v << 1, 2, 3;

  test::check_varis_on_stack(stan::math::diag_pre_multiply(v, m));
}
