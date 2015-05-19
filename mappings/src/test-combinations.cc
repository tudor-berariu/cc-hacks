// Copyright (C) 2015 Tudor Berariu

#include "meta/meta_combinations.h"

int main(int, char*[]) {
  static_assert(_Combinations<5, 3>::count() == 10, "C(5,3) != 10");
  static_assert(_Combinations<6, 6>::count() == 1, "C(6,6) != 1");
  static_assert(_Combinations<9, 6>::count() == 84, "C(9,6) != 84");
  static_assert(_Combinations<0, 0>::count() == 1, "C(0,0) != 1");
  static_assert(_Combinations<16, 11>::count() == 4368, "C(16,11) != 4368");
  return 0;
}
