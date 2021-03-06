#ifndef STAN_MATH_GPU_KERNELS_SUBTRACT_HPP
#define STAN_MATH_GPU_KERNELS_SUBTRACT_HPP
#ifdef STAN_OPENCL

#include <stan/math/gpu/kernel_cl.hpp>

namespace stan {
namespace math {
namespace opencl_kernels {
// \cond
const char *subtract_kernel_code = STRINGIFY(
    // \endcond
    /**
     * Matrix subtraction on the GPU Subtracts the second matrix
     * from the first matrix and stores the result in the third matrix (C=A-B).
     *
     * @param[out] C The output matrix.
     * @param[in] B RHS input matrix.
     * @param[in] A LHS input matrix.
     * @param rows The number of rows for matrix A.
     * @param cols The number of columns for matrix A.
     * @note Code is a <code>const char*</code> held in
     * <code>subtract_kernel_code.</code>
     * Used in math/gpu/subtract_opencl.hpp
     *  This kernel uses the helper macros available in helpers.cl.
     */
    __kernel void subtract(
        __global write_only double *C, __global read_only double *A,
        __global read_only double *B, read_only unsigned int rows,
        read_only unsigned int cols) {
      int i = get_global_id(0);
      int j = get_global_id(1);
      if (i < rows && j < cols) {
        C(i, j) = A(i, j) - B(i, j);
      }
    }
    // \cond
);
// \endcond

/**
 * See the docs for \link kernels/subtract.hpp subtract() \endlink
 */
const global_range_kernel<cl::Buffer, cl::Buffer, cl::Buffer, int, int>
    subtract("subtract", subtract_kernel_code);

}  // namespace opencl_kernels
}  // namespace math
}  // namespace stan
#endif
#endif
