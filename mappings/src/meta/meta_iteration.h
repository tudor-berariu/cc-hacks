// Copyright (C) 2015 Tudor Berariu

#ifndef META_ITERATION_H
#define META_ITERATION_H

#include <cstddef>

template<template<size_t i, size_t j> class Functor,
         size_t rows_no, size_t cols_no>
struct For;

template<template<size_t, size_t> class Functor,
         size_t rows_no, size_t cols_no,
         size_t crt_row, size_t crt_col>
struct _For;

template<template<size_t, size_t> class Functor,
         size_t rows_no, size_t cols_no>
struct For {
  static void iterate() {
    _For<Functor, rows_no, cols_no, 0, 0>::iterate();
  }
};

template<template<size_t, size_t> class Functor,
         size_t rows_no, size_t cols_no>
struct _For<Functor, rows_no, cols_no, rows_no, 0ul> {
  static void iterate() {
    Functor<rows_no, 0>::end();
  }
};

template<template<size_t, size_t> class Functor,
         size_t rows_no, size_t cols_no, size_t crt_row>
struct _For<Functor, rows_no, cols_no, crt_row, cols_no> {
  static void iterate() {
    Functor<crt_row, cols_no>::end_row();
    _For<Functor, rows_no, cols_no, crt_row + 1, 0>::iterate();
  }
};

template<template<size_t, size_t> class Functor,
         size_t rows_no, size_t cols_no, size_t crt_row, size_t crt_col>
struct _For {
  static void iterate() {
    Functor<crt_row, crt_col>::apply();
    _For<Functor, rows_no, cols_no, crt_row, crt_col + 1>::iterate();
  }
};

#endif
