// Copyright (C) 2015 Tudor Berariu

#include <iostream>
#include "meta/mappings.h"

int main(int, char * [])
{
  // Test All
  static_assert(Mapping<3ul, 5ul>::All::value_of<2, 2>(), "test All");
  static_assert(Mapping<3ul, 5ul>::All::value_of<1, 0>(), "test All");
  static_assert(Mapping<5ul, 10ul>::Combinations<3ul>::value_of<0, 0>(), "");
  return 0;
}
