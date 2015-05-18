#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <cstddef>
#include "meta/meta_math.h"

template<size_t in_size, size_t out_size>
struct _All;

template<size_t in_size, size_t out_size, size_t n>
struct _Combinations;

template<size_t in_size, size_t out_size>
struct Mapping {
  using All = _All<in_size, out_size>;

  template<size_t n>
  using Combinations = _Combinations<in_size, out_size, n>;
};

template<size_t in_size, size_t out_size>
struct _All {
  template<size_t in_idx, size_t out_idx>
  static constexpr bool value_of() {
    return (in_idx < in_size) && (out_idx < out_size);
  }
};

template<size_t in_size, size_t out_size, size_t k>
struct _Combinations {
  static_assert(comb<size_t, in_size, k>() == out_size,
                "The # of output maps must be equal to the # of combinations");

  template<size_t in_idx, size_t out_idx>
  static constexpr bool value_of() {
    return j_in_ith_comb<size_t, out_size, in_size, in_idx, out_idx>(); 
  }
};

#endif
