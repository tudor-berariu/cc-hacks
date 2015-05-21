// Copyright (C) 2015 Tudor Berariu

#include "meta/mappings.h"
#include "meta/meta_combinations.h"
#include "meta/meta_iteration.h"

int main(int, char*[]) {
  static_assert(_Combinations<5, 3>::count() == 10, "C(5,3) != 10");
  static_assert(_Combinations<6, 6>::count() == 1, "C(6,6) != 1");
  static_assert(_Combinations<9, 6>::count() == 84, "C(9,6) != 84");
  static_assert(_Combinations<0, 0>::count() == 1, "C(0,0) != 1");
  static_assert(_Combinations<16, 11>::count() == 4368, "C(16,11) != 4368");

  static_assert(_Combinations<8, 4>::Ith_elem_in_Jth<0, 4>::exists(),
                "0th element in 4th combination of C(8,4)");
  static_assert(_Combinations<8, 4>::Ith_elem_in_Jth<1, 4>::exists(),
                "1st element in 4th combination of C(8,4)");
  static_assert(_Combinations<8, 4>::Ith_elem_in_Jth<2, 4>::exists(),
                "2nd element in 4th combination of C(8,4)");
  static_assert(!_Combinations<8, 4>::Ith_elem_in_Jth<3, 4>::exists(),
                "3rd element in 4th combination of C(8,4)");
  static_assert(!_Combinations<8, 4>::Ith_elem_in_Jth<4, 4>::exists(),
                "4th element in 4th combination of C(8,4)");
  static_assert(!_Combinations<8, 4>::Ith_elem_in_Jth<5, 4>::exists(),
                "5th element in 4th combination of C(8,4)");
  static_assert(!_Combinations<8, 4>::Ith_elem_in_Jth<6, 4>::exists(),
                "6th element in 4th combination of C(8,4)");
  static_assert(_Combinations<8, 4>::Ith_elem_in_Jth<7, 4>::exists(),
                "7th element in 4th combination of C(8,4)");

  For<_Combinations<8, 4>::PrintElements, 8, _Combinations<8, 4>::count()>
      ::iterate();

  For<MappingsPrinter<5, 2, 3, 4, 5>::Iterator,
      5,
      Combinations<2,3,4,5>::template out_maps_no<5>()>::iterate();
  cout << Sequences<2,3,4,5>::template out_maps_no<5>() << " total maps"
       << endl;
  For<SequencesPrinter<6, 1, 2, 3, 4, 5, 6>::Iterator,
      6,
      Sequences<1,2,3,4,5,6>::template out_maps_no<6>()>::iterate();
  return 0;
}
