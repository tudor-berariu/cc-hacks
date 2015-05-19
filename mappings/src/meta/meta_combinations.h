// Copyright (C) 2015 Tudor Berariu

#ifndef META_MATH_H
#define META_MATH_H

#include <cstddef>
#include <iostream>

using namespace std;

template<size_t n, size_t k>
struct _Combinations;

template<>
struct _Combinations<0, 0> {
  static constexpr size_t count();

  template<size_t i, size_t j>
  struct Ith_elem_in_Jth {
    static constexpr bool exists();
  };

  template<size_t i, size_t j>
  struct PrintElements {
    static void apply();
    static void end_row();
    static void end();
  };
};

template<size_t n>
struct _Combinations<n, 0> {
  static constexpr size_t count();

  template<size_t i, size_t j>
  struct Ith_elem_in_Jth {
    static constexpr bool exists();
  };
  template<size_t i, size_t j>
  struct PrintElements {
    static void apply();
    static void end_row();
    static void end();
  };
};

template<size_t n>
struct _Combinations<n, n> {
  static constexpr size_t count();

  template<size_t i, size_t j>
  struct Ith_elem_in_Jth {
    static constexpr bool exists();
  };
  template<size_t i, size_t j>
  struct PrintElements {
    static void apply();
    static void end_row();
    static void end();
  };
};

template<size_t n, size_t k>
struct _Combinations {
  static constexpr size_t count();

  template<size_t i, size_t j>
  struct Ith_elem_in_Jth;
 
  template<size_t j>
  struct Ith_elem_in_Jth<0, j> {
    static constexpr bool exists();
  };

  template<size_t i, size_t j>
  struct Ith_elem_in_Jth {
    static constexpr bool exists();
  };
  template<size_t i, size_t j>
  struct PrintElements {
    static void apply();
    static void end_row();
    static void end();
  };
};

template<bool t, size_t n, size_t k, size_t i, size_t j>
struct _Cond;

template<size_t n, size_t k, size_t i, size_t j>
struct _Cond<true, n, k, i, j> {
  static constexpr bool exists();
};

template<size_t n, size_t k, size_t i, size_t j>
struct _Cond<false, n, k, i, j> {
  static constexpr bool exists();
};

constexpr size_t _Combinations<0, 0>::count() {
  return 1ul;
}

template<size_t i, size_t j>
constexpr bool _Combinations<0, 0>::Ith_elem_in_Jth<i, j>::exists() {
  return false;
}

template<size_t n>
constexpr size_t _Combinations<n, 0>::count() {
  return 1ul;
}

template<size_t n>
template<size_t i, size_t j>
constexpr bool _Combinations<n, 0>::Ith_elem_in_Jth<i, j>::exists() {
  return false;
}


template<size_t n>
constexpr size_t _Combinations<n, n>::count() {
  return 1ul;
}

template<size_t n>
template<size_t i, size_t j>
constexpr bool _Combinations<n,n>::Ith_elem_in_Jth<i, j>::exists() {
  static_assert(i < n, "Checking bounds for i");
  return (j == 0);
}

template<size_t n, size_t k>
constexpr size_t _Combinations<n, k>::count() {
  return _Combinations<n-1, k-1>::count() + _Combinations<n-1, k>::count();
}

template<size_t n, size_t k>
template<size_t j>
constexpr bool _Combinations<n, k>::Ith_elem_in_Jth<0, j>::exists() {
  return (j < _Combinations<n-1, k-1>::count());
}

template<size_t n, size_t k>
template<size_t i, size_t j>
constexpr bool _Combinations<n, k>::Ith_elem_in_Jth<i, j>::exists() {
  return _Cond<(j < _Combinations<n-1, k-1>::count()), n, k, i, j>::exists();
}

template<size_t n, size_t k, size_t i, size_t j>
constexpr bool _Cond<true, n, k, i, j>::exists() {
  using __C = _Combinations<n-1, k-1>;
  using __IJ = typename __C::template Ith_elem_in_Jth<i-1, j>;
  return __IJ::exists();
}

template<size_t n, size_t k, size_t i, size_t j>
constexpr bool _Cond<false, n, k, i, j>::exists() {
  using __C = _Combinations<n-1, k>;
  using __IJ = typename __C::template
                Ith_elem_in_Jth<i-1, j- _Combinations<n-1, k-1>::count()>;
  return __IJ::exists();
}


template<size_t n, size_t k>
template<size_t i, size_t j>
void _Combinations<n, k>::PrintElements<i, j>::apply() {
  cout << (Ith_elem_in_Jth<i, j>::exists() ? "X" : "_");
}

template<size_t n, size_t k>
template<size_t i, size_t j>
void _Combinations<n, k>::PrintElements<i, j>::end_row() {
  cout << endl;
}

template<size_t n, size_t k>
template<size_t i, size_t j>
void _Combinations<n, k>::PrintElements<i, j>::end() {
  cout << "Done!" << endl;
}

#endif
