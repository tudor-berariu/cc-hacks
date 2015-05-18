#ifndef META_MATH_H
#define META_MATH_H

template<typename T, T n, T k>
static constexpr T comb() {
  return (k > 0 && n > 1) ? (comb<T, n-1, k-1>() + comb<T, n-1, k>()) : 1;
}

/*
 * a b c d e
 * 0: a b c
 * 1: a b d
 * 2: a b e
 * 3: a c d
 * 4: a c e
 * 5: a d e
 * 6: b c d
 * 7: b c e
 * 8: b d e
 * 9: c d e
*/

template<typename T, T n, T k, T i, T j>
static constexpr T j_in_ith_comb() {
  return (i == 0) ? 
         (j <= comb<n-1, k-1>()) :
         ((j <= comb<n-1, k-1>()) ?
          j_in_ith_comb<n-1, k-1, i-1, j>() :
          j_in_ith_comb<n-1, k, i-1, j - comb<T, n-1, k-1>()>());
}

#endif
