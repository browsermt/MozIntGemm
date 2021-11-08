/** A fallback (non-optimized) implementation of "wasm_gemm_interface.h"
 * interface for integer matrix multiplication for wasm target.
 *
 * This implementation is built and exported from the main module and can serve
 * as a polyfill (fallback) for browsers that don't support an optimized
 * implementation of "wasm_gemm_interface.h".
 */

#include "3rd-party/intgemm/intgemm/intgemm.h"
#include "3rd-party/wasm_intgemm/wasm_intgemm_interface.h"
#include <iostream>

namespace pg::Intgemm {

void int8PrepareA(const float *input_A, float scale, float zero_point,
                  Index rows_A, Index width, int8_t *output) {
  intgemm::Int8Shift::PrepareA(input_A, output, scale, /*Quant Mult*/
                               rows_A, width);
}

void int8PrepareB(const float *input_B, float scale, float zero_point,
                  Index width, Index cols_B, int8_t *output) {
  intgemm::Int8::PrepareB(input_B, output, scale, /*Quant Mult*/
                          width, cols_B);
}

void int8PrepareBFromTransposed(const float *input_B_transposed, float scale,
                                float zero_point, Index width, Index cols_B,
                                int8_t *output) {
  std::cerr << "Unimplemented int8PrepareBFromTransposed" << std::endl;
  std::abort();
}

void int8PrepareBFromQuantizedTransposed(const int8_t *input_B_quant_transposed,
                                         Index width, Index cols_B,
                                         int8_t *output) {
  intgemm::Int8::PrepareBQuantizedTransposed(input_B_quant_transposed, output,
                                             width, cols_B);
}

void int8PrepareBias(const int8_t *input_B_prepared, float scale,
                     float zero_point, Index width, Index cols_B,
                     const float *input_bias, float *output) {
  intgemm::Int8Shift::PrepareBias(
      input_B_prepared, width, cols_B,
      intgemm::callbacks::UnquantizeAndAddBiasAndWrite(scale, input_bias,
                                                       output));
}

void int8MultiplyAndAddBias(const int8_t *input_A_prepared, float scale_A,
                            float zero_point_A, const int8_t *input_B_prepared,
                            float scale_B, float zero_point_B,
                            const float *input_bias_prepared, Index rows_A,
                            Index width, Index cols_B, float *output) {
  intgemm::Int8Shift::Multiply(input_A_prepared, input_B_prepared, rows_A,
                               width, cols_B,
                               intgemm::callbacks::UnquantizeAndAddBiasAndWrite(
                                   scale_A, input_bias_prepared, output));
}

void int8SelectColumnsOfB(const int8_t *input_B_prepared, Index width,
                          Index cols_B, const Index *cols, const Index num_cols,
                          int8_t *output) {
  intgemm::Int8::SelectColumnsB(input_B_prepared, output, width, cols,
                                cols + num_cols);
}
} // namespace pg::Intgemm
