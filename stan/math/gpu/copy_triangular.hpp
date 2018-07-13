#ifndef STAN_MATH_GPU_COPY_TRIANGULAR_HPP
#define STAN_MATH_GPU_COPY_TRIANGULAR_HPP
#ifdef STAN_OPENCL
#include <stan/math/gpu/matrix_gpu.hpp>
#include <stan/math/gpu/set_kernel_args.hpp>
#include <CL/cl.hpp>

namespace stan {
namespace math {
enum triangularity { LOWER = 0, UPPER = 1, NONE = 2 };

/**
 * Copies the lower or upper
 * triangular of the source matrix to
 * the destination matrix.
 * Both matrices are stored on the GPU.
 *
 * @param src the source matrix
 * @param lower_upper enum to describe
 * which part of the matrix to copy:
 * LOWER - copies the lower triangular
 * UPPER - copes the upper triangular
 *
 * @return the matrix with the copied content
 *
 */
inline matrix_gpu copy_triangular(matrix_gpu& src, triangularity lower_upper) {
  if (src.size() == 0) {
    matrix_gpu dst(src);
    return dst;
  }
  if (src.size() == 1) {
    matrix_gpu dst(src);
    return dst;
  }
  matrix_gpu dst(src.rows(), src.cols());
  cl::Kernel kernel = opencl_context.get_kernel("copy_triangular");
  cl::CommandQueue cmdQueue = opencl_context.queue();
  try {
    set_kernel_args(kernel, dst.buffer(), src.buffer(), dst.rows(), dst.cols(),
     lower_upper);
    cmdQueue.enqueueNDRangeKernel(kernel, cl::NullRange,
                                  cl::NDRange(dst.rows(), dst.cols()),
                                  cl::NullRange, NULL, NULL);
  } catch (const cl::Error& e) {
    check_opencl_error("copy_triangular", e);
  }
  return dst;
}
}  // namespace math
}  // namespace stan

#endif
#endif
