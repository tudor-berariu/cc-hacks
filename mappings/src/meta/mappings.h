// Copyright (C) 2015 Tudor Berariu

#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <cstddef>
#include "meta/meta_combinations.h"
#include "meta/meta_sequences.h"

struct All {
  template<size_t in_maps, size_t out_maps>
  struct Synapses {
    template<size_t in_idx, size_t out_idx>
    static constexpr bool exists() {
      static_assert(in_idx < in_maps && out_idx < out_maps, "wrong map idx");
      return true;
    }
  };
};

template<size_t... values>
struct Combinations;

template<size_t k>
struct Combinations<k> {
  template<size_t in_maps>
  inline static constexpr size_t out_maps_no() {
    return _Combinations<in_maps, k>::count();
  }
  template<size_t in_maps>
  struct Synapses {
    template<size_t in_idx, size_t out_idx>
    inline static constexpr bool exists() {
      static_assert(in_idx < in_maps, "wrong input map index");
      using __C = _Combinations<in_maps, k>;
      static_assert(out_idx < __C::count(), "wrong output map index");
      using __IJ = typename __C::template Ith_elem_in_Jth<in_idx, out_idx>;
      return __IJ::exists();
    }
  };
};

template<size_t k, size_t... values>
struct Combinations<k, values...> {
  using Next = Combinations<values...>;
  template<size_t in_maps>
  inline static constexpr size_t out_maps_no() {
    return _Combinations<in_maps, k>::count() +
      Next::template out_maps_no<in_maps>();
  }
  template<size_t in_maps>
  struct Synapses {
    using __C = _Combinations<in_maps, k>;

    template<bool here, size_t in_idx, size_t out_idx>
    struct __Choose;

    template<size_t in_idx, size_t out_idx>
    inline static constexpr bool exists() {
      return __Choose<out_idx < __C::count(), in_idx, out_idx>::exists();
    }

    template<size_t in_idx, size_t out_idx>
    struct __Choose<true, in_idx, out_idx> {
      inline static constexpr bool exists() {
        using __IJ = typename __C::template Ith_elem_in_Jth<in_idx, out_idx>;
        return __IJ::exists();
      }
    };
    template<size_t in_idx, size_t out_idx>
    struct __Choose<false, in_idx, out_idx> {
      inline static constexpr bool exists() {
        return 
          Next::template Synapses<in_maps>::template exists<in_idx, out_idx-__C::count()>();
      }
    };
  };
};

template<size_t in_maps, size_t... values>
struct MappingsPrinter {
  template<size_t in_idx, size_t out_idx>
  struct Iterator {
    inline static void apply() {
      cout << Combinations<values...>::
        template Synapses<in_maps>::template exists<in_idx, out_idx>();
    }

    inline static void end_row() {
      cout << endl;
    }

    inline static void end() {
      cout << "Done!" << endl;
    }  
  };
};

template<size_t... values>
struct Sequences;

template<size_t k>
struct Sequences<k> {
  template<size_t in_maps>
  inline static constexpr size_t out_maps_no() {
    return _Sequences<in_maps, k>::count();
  }
  template<size_t in_maps>
  struct Synapses {
    template<size_t in_idx, size_t out_idx>
    inline static constexpr bool exists() {
      static_assert(in_idx < in_maps, "wrong input map index");
      using __S = _Sequences<in_maps, k>;
      static_assert(out_idx < __S::count(), "wrong output map index");
      using __IJ = typename __S::template Ith_elem_in_Jth<in_idx, out_idx>;
      return __IJ::exists();
    }
  };
};

template<size_t k, size_t... values>
struct Sequences<k, values...> {
  using Next = Sequences<values...>;
  template<size_t in_maps>
  inline static constexpr size_t out_maps_no() {
    return _Sequences<in_maps, k>::count() +
      Next::template out_maps_no<in_maps>();
  }
  template<size_t in_maps>
  struct Synapses {
    using __S = _Sequences<in_maps, k>;

    template<bool here, size_t in_idx, size_t out_idx>
    struct __Choose;

    template<size_t in_idx, size_t out_idx>
    inline static constexpr bool exists() {
      return __Choose<out_idx < __S::count(), in_idx, out_idx>::exists();
    }

    template<size_t in_idx, size_t out_idx>
    struct __Choose<true, in_idx, out_idx> {
      inline static constexpr bool exists() {
        using __IJ = typename __S::template Ith_elem_in_Jth<in_idx, out_idx>;
        return __IJ::exists();
      }
    };
    template<size_t in_idx, size_t out_idx>
    struct __Choose<false, in_idx, out_idx> {
      inline static constexpr bool exists() {
        return 
          Next::template Synapses<in_maps>::template exists<in_idx, out_idx-__S::count()>();
      }
    };
  };
};

template<size_t in_maps, size_t... values>
struct SequencesPrinter {
  template<size_t in_idx, size_t out_idx>
  struct Iterator {
    inline static void apply() {
      cout << Sequences<values...>::
        template Synapses<in_maps>::template exists<in_idx, out_idx>();
    }

    inline static void end_row() {
      cout << endl;
    }

    inline static void end() {
      cout << "Done!" << endl;
    }  
  };
};
#endif
