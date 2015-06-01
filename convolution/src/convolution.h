#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <cstddef>

#include <cstring>

#include <iostream>

extern "C" {
  #include <cblas.h>
}

using namespace std;


template<typename T>
constexpr T relu(T x) {
  return (x > 0) ? x : 0;
}

template<typename T>
void mul_mat(T *A, double *B, double *C, size_t m, size_t k, size_t n) {
  for(size_t r = 0; r < m; r++) {
    for(size_t c = 0; c < n; c++) {
      double x = 0.0;
      for(size_t z = 0; z < k; z++) {
        x += A[r * k + z] * B[z * n + c];
      }
      C[r * n + c] = x;
    }
  }
}

template<size_t in_maps, size_t in_height, size_t in_width,
         size_t conv_height, size_t conv_width,
         size_t vert_stride, size_t horiz_stride,
         size_t out_maps>
struct ConvolutionLayer {
  static constexpr size_t conv_size = conv_height * conv_width;
  static constexpr size_t in_map_size = in_height * in_width;
  static constexpr size_t inputs_no = in_maps * in_height * in_width;
  static constexpr size_t parameters_no = out_maps * (conv_width * conv_height * in_maps + 1);
  static constexpr size_t bias_idx = conv_width * conv_height * in_maps;
  static constexpr size_t out_height = (in_height - conv_height) / vert_stride +1;
  static constexpr size_t out_width = (in_width - conv_width) / horiz_stride + 1;
  static constexpr size_t out_map_size = out_height * out_width;
  static constexpr size_t internal_no = out_maps * out_height * out_width;
  static constexpr size_t outputs_no = out_maps * out_height * out_width;

  static constexpr size_t p_idx(size_t out_map, size_t in_map, size_t conv_r, size_t conv_c) {
    return out_map * (in_maps * conv_height * conv_width + 1) + in_map * (conv_height * conv_width)
      + conv_r * conv_width + conv_c;
  }

  static void forward(double* inputs, double * parameters, double* outputs) {
    for (size_t j = 0; j < out_maps; j++) {
      double *pj = &(parameters[j*(conv_height * conv_width * in_maps + 1)]);

      for (size_t out_r = 0; out_r < out_height; out_r++) {
        for (size_t out_c = 0; out_c < out_width; out_c++) {
          const size_t out_idx = j * (out_map_size) + out_r * out_width + out_c;
          double a = 0;
          for (size_t i = 0; i < in_maps; i++) {
            const size_t i_idx = i * conv_size;
            const size_t i2_idx = i * in_map_size;
            for (size_t conv_r = 0; conv_r < conv_height; conv_r++) {
              for (size_t conv_c = 0; conv_c < conv_width; conv_c++) {
                size_t p2_idx =  i_idx + conv_r * conv_width + conv_c;
                size_t in_idx =
                  i2_idx + (out_r * vert_stride + conv_r) * in_width + out_c * horiz_stride + conv_c;
                a += pj[p2_idx] * inputs[in_idx];
              }
            }
          }
          a += pj[bias_idx];
          outputs[out_idx] = relu(a);
        }
      }
    }
  }
};

template<size_t in_maps, size_t in_height, size_t in_width,
         size_t conv_height, size_t conv_width,
         size_t vert_stride, size_t horiz_stride,
         size_t out_maps>
struct ConvolutionLayer2 {
  static constexpr size_t in_map_size = in_height * in_width;
  static constexpr size_t inputs_no = in_maps * in_map_size;
  static constexpr size_t out_height = (in_height - conv_height) / vert_stride +1;
  static constexpr size_t out_width = (in_width - conv_width) / horiz_stride + 1;
  static constexpr size_t out_map_size = out_height * out_width;
  static constexpr size_t internal_no = out_maps * out_map_size;
  static constexpr size_t outputs_no = out_maps * out_map_size;
  static constexpr size_t conv_size = conv_height * conv_width;
  static constexpr size_t parameters_no = out_maps * (in_maps * conv_size + 1);

  static constexpr size_t p_idx(size_t out_map, size_t in_map, size_t conv_r, size_t conv_c) {
    return out_map * (in_maps * conv_height * conv_width + 1) + in_map * (conv_height * conv_width)
      + conv_r * conv_width + conv_c;
  }

  static void forward(double* inputs, double *parameters, double* outputs) {
    double* _in = (double*) malloc(sizeof(double) * in_maps * conv_size * out_map_size);

    for (size_t out_r = 0; out_r < out_height; out_r++) {
      for (size_t out_c = 0; out_c < out_width; out_c++) {
        for (size_t i = 0; i < in_maps; i++) {
          for (size_t conv_r = 0; conv_r < conv_height; conv_r++) {
            for (size_t conv_c = 0; conv_c < conv_width; conv_c++) {
              size_t in_idx =
                i * in_map_size +
                (out_r * vert_stride + conv_r) * in_width +
                out_c * horiz_stride + conv_c;
              size_t row_idx = i * conv_size + conv_r * conv_width + conv_c;
              size_t _in_idx = row_idx * out_map_size + out_r * out_width + out_c;
              _in[_in_idx] = inputs[in_idx];
            }
          }
        }
      }
    }

    for (size_t j = 0; j < out_maps;j++) {
      double bias = parameters[(j+1) * (in_maps * conv_size + 1) - 1];
      for (size_t i = 0; i < out_map_size; i++) {
        outputs[j * out_map_size + i] = bias;
      }
    }

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                out_maps, out_map_size, in_maps * conv_size,
                1.0, parameters, in_maps * conv_size + 1,
                _in, out_map_size,
                1.0, outputs, out_map_size);

    for(size_t j = 0; j < out_maps * out_map_size; j++) {
      outputs[j] = relu(outputs[j]);
    }
  }
};


#endif
