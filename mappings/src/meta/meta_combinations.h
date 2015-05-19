#ifndef META_MATH_H
#define META_MATH_H

#include <cstddef>

template<size_t n, size_t k>
struct _Combinations;

template<>
struct _Combinations<0, 0> {
  static constexpr size_t count();
};

template<size_t n>
struct _Combinations<n, 0> {
  static constexpr size_t count();
};

template<size_t n>
struct _Combinations<n, n> {
  static constexpr size_t count();
};

template<size_t n, size_t k>
struct _Combinations {
  static constexpr size_t count();
};

constexpr size_t _Combinations<0, 0>::count() {
  return 1ul;
}

template<size_t n>
constexpr size_t _Combinations<n, 0>::count() {
  return 1ul;
}

template<size_t n>
constexpr size_t _Combinations<n, n>::count() {
  return 1ul;
}

template<size_t n, size_t k>
constexpr size_t _Combinations<n, k>::count() {
  return _Combinations<n-1, k-1>::count() + _Combinations<n-1, k>::count();
}

#endif
