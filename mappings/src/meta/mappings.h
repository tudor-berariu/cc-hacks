#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <cstddef>

template<size_t in_size, size_t out_size>
struct _All;

template<size_t in_size, size_t out_size>
struct Mapping {
  using All = _All<in_size, out_size>;
};

template<size_t in_size, size_t out_size>
struct _All {
  template<size_t in_idx, size_t out_idx>
  static constexpr bool value_of() {
    return (in_idx < in_size) && (out_idx < out_size);
  }
};

#endif
