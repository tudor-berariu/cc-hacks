#include <iostream>
#include <random>

#include "convolution.h"

using namespace std;

int main(int, char *[]) {
  constexpr size_t IN_MAPS = 10;
  constexpr size_t IN_HEIGHT = 227;
  constexpr size_t IN_WIDTH = 227;

  constexpr size_t CONV_HEIGHT = 11;
  constexpr size_t CONV_WIDTH = 11;
  constexpr size_t VERT_STRIDE = 4;
  constexpr size_t HORIZ_STRIDE = 4;

  constexpr size_t OUT_MAPS = 596;

  //using MyLayer = ConvolutionLayer<IN_MAPS, IN_HEIGHT, IN_WIDTH, CONV_HEIGHT,
  //                                 CONV_WIDTH, VERT_STRIDE, HORIZ_STRIDE, OUT_MAPS>;

  using MyLayer2 = ConvolutionLayer2<IN_MAPS, IN_HEIGHT, IN_WIDTH, CONV_HEIGHT,
                                     CONV_WIDTH, VERT_STRIDE, HORIZ_STRIDE, OUT_MAPS>;

  double* params = (double*)malloc(sizeof(double) * MyLayer2::parameters_no);
  double* inputs = (double*)malloc(sizeof(double) * IN_MAPS * IN_HEIGHT * IN_WIDTH);
  //double* storage = (double*)malloc(sizeof(double) * MyLayer2::internal_no);
  //double* outputs = (double*)malloc(sizeof(double) * MyLayer::outputs_no);
  double* outputs2 = (double*)malloc(sizeof(double) * MyLayer2::outputs_no);

  cout << "Am facut un mare layer cu "
       << MyLayer2::inputs_no << " intrari, "
       << MyLayer2::parameters_no << " parametri, "
       << MyLayer2::out_height << "x" << MyLayer2::out_width << " = "
       << MyLayer2::outputs_no << " iesiri" << endl;

  random_device r{ };
  default_random_engine e {r()};
  uniform_real_distribution<double> next_double(-0.05, 0.05);

  // Init inputs
  for (size_t i = 0; i < IN_MAPS; i++) {
    for (size_t r = 0; r < IN_HEIGHT; r++) {
      for (size_t c = 0; c < IN_WIDTH; c++) {
        inputs[i * (IN_HEIGHT * IN_WIDTH) + r * IN_WIDTH + c] = next_double(e);
      }
    }
  }
  /*
  for (size_t i = 0; i < IN_MAPS; i++) {
    for (size_t r = 0; r < IN_HEIGHT; r++) {
      for (size_t c = 0; c < IN_WIDTH; c++) {
        cout.width(4);
        cout << inputs[i * (IN_HEIGHT * IN_WIDTH) + r * IN_WIDTH + c] << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
  */

  for (size_t j = 0; j < OUT_MAPS; j++) {
    for (size_t i = 0; i < IN_MAPS; i++) {
      for (size_t r = 0; r < CONV_HEIGHT; r++) {
        for (size_t c = 0; c < CONV_WIDTH; c++) {
          params[MyLayer2::p_idx(j, i, r, c)] = next_double(e);
        }
      }
    }
  }

  /*
  cout << "============" << endl;
  for (size_t j = 0; j < OUT_MAPS; j++) {
    for (size_t i = 0; i < IN_MAPS; i++) {
      for (size_t r = 0; r < CONV_HEIGHT; r++) {
        for (size_t c = 0; c < CONV_WIDTH; c++) {
          cout.width(6);
          cout.precision(4);
          cout << params[MyLayer::p_idx(j, i, r, c)] << " ";
        }
        cout << endl;
      }
      cout << " ---- " << endl;
    }
    cout << "============" << endl;
  }
  */
  //MyLayer::forward(inputs, storage, params, outputs);

  /*
  for (size_t j = 0; j < OUT_MAPS; j++) {
    for (size_t r = 0; r < MyLayer::out_height; r++) {
      for (size_t c = 0; c < MyLayer::out_width; c++) {
        cout.width(8);
        cout.precision(6);
        cout << outputs[j * (MyLayer::out_height * MyLayer::out_width) +
                        r * MyLayer::out_width + c] << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
  */

  MyLayer2::forward(inputs, params, outputs2);

  double x = .0;

  for (size_t j = 0; j < OUT_MAPS; j++) {
    for (size_t r = 0; r < MyLayer2::out_height; r++) {
      for (size_t c = 0; c < MyLayer2::out_width; c++) {
        x += ((j % 2) - 0.5) * outputs2[j *
                                        (MyLayer2::out_height * MyLayer2::out_width) +
                                        r * MyLayer2::out_width + c];
      }
    }
  }

  cout << x << endl;
  return 0;
}
