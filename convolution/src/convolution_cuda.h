#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <cstddef>

#include <cstring>

#include <iostream>

// CUDA runtime
#include <cuda_runtime.h>
#include "cublas_v2.h"
constexpr int IDX2C (int i,int j, int ld){
  return (((j)*(ld))+( i ));
} 
// CUDA and CUBLAS functions
//#include <helper_functions.h>
//#include <helper_cuda.h>

using namespace std;


template<typename T>
constexpr T relu(T x) {
  return (x > 0) ? x : 0;
}

template<typename T>
void mul_mat(T *A, float *B, float *C, size_t m, size_t k, size_t n) {
  for(size_t r = 0; r < m; r++) {
    for(size_t c = 0; c < n; c++) {
      float x = 0.0;
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

  static void forward(float* inputs, float * parameters, float* outputs) {
    for (size_t j = 0; j < out_maps; j++) {
      float *pj = &(parameters[j*(conv_height * conv_width * in_maps + 1)]);

      for (size_t out_r = 0; out_r < out_height; out_r++) {
        for (size_t out_c = 0; out_c < out_width; out_c++) {
          const size_t out_idx = j * (out_map_size) + out_r * out_width + out_c;
          float a = 0;
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

  static void forward(float* inputs, float *parameters, float* outputs) {
    float* _in = (float*) malloc(sizeof(float) * in_maps * conv_size * out_map_size);

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
      float bias = parameters[(j+1) * (in_maps * conv_size + 1) - 1];
      for (size_t i = 0; i < out_map_size; i++) {
        outputs[j * out_map_size + i] = bias;
      }
    }

    //cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                //out_maps, out_map_size, in_maps * conv_size,
                //1.0, parameters, in_maps * conv_size + 1,
                //_in, out_map_size,
                //1.0, outputs, out_map_size);
    
    
    cudaError_t cudaStat;
    cublasStatus_t stat;
    cublasHandle_t handle;

    // on the device
    float* a = parameters;
    float* b = _in;
    float* c = outputs;
    float * d_a; // d_a - a on the device
    float * d_b; // d_b - b on the device
    float * d_c; // d_c - c on the device
    size_t m = out_maps;
    size_t n = out_map_size;
    size_t k = in_maps * conv_size;
    cudaStat = cudaMalloc (( void **)& d_a ,m*k* sizeof (*a)); // device
    // memory alloc for a
    cudaStat = cudaMalloc (( void **)& d_b ,k*n* sizeof (*b)); // device
    // memory alloc for b
    cudaStat = cudaMalloc (( void **)& d_c ,m*n* sizeof (*c)); // device
    // memory alloc for c
    stat = cublasCreate (& handle ); // initialize CUBLAS context
    // copy matrices from the host to the device
    stat = cublasSetMatrix (m,k, sizeof (*a) ,a,m,d_a ,m); //a -> d_a
    stat = cublasSetMatrix (k,n, sizeof (*b) ,b,k,d_b ,k); //b -> d_b
    stat = cublasSetMatrix (m,n, sizeof (*c) ,c,m,d_c ,m); //c -> d_c
    float al =1.0f; // al =1
    float bet =1.0f; // bet =1
    // matrix - matrix multiplication : d_c = al*d_a *d_b + bet *d_c
    // d_a -mxk matrix , d_b -kxn matrix , d_c -mxn matrix ;
    // al ,bet -scalars
    stat=cublasSgemm(handle,CUBLAS_OP_N,CUBLAS_OP_N,m,n,k,&al,d_a,
    m,d_b,k,&bet,d_c,m);
    stat = cublasGetMatrix (m,n, sizeof (*c) ,d_c ,m,c,m); // cp d_c - >c
    int i;
    int j;
    //printf ("c after Sgemm :\n");
    //for(i=0;i<m;i ++){
      //for(j=0;j<n;j ++){
        //printf (" %7.0 f",c[ IDX2C (i,j,m )]); // print c after Sgemm
      //}
      //printf ("\n");
    //}
    cudaFree (d_a ); // free device memory
    cudaFree (d_b ); // free device memory
    cudaFree (d_c ); // free device memory
    cublasDestroy ( handle ); // destroy CUBLAS context
    free (a); // free host memory
    free (b); // free host memory
    free (c); // free host memory

    for(size_t j = 0; j < out_maps * out_map_size; j++) {
      outputs[j] = relu(outputs[j]);
    }
  }
};


#endif
