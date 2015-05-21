// Copyright (C) 2015 Tudor Berariu

#ifndef META_SEQUENCES_H
#define META_SEQUENCES_H

#include <cstddef>
#include <iostream>

using namespace std;

template<size_t n, size_t k>
struct _Sequences {
  inline static constexpr size_t count();
  template<size_t i, size_t j>
  struct Ith_elem_in_Jth {
    inline static constexpr bool exists();
  };
};

template<size_t n, size_t k>
inline constexpr size_t _Sequences<n, k>::count() {
  return ((n > k) ? n : 1ul);
}

template<size_t n, size_t k>
template<size_t i, size_t j>
inline constexpr bool _Sequences<n, k>::Ith_elem_in_Jth<i, j>::exists(){
  return ((i >= j) && (i < j + k)) || ((i < j + k - n) && (i < j));
}

#endif
