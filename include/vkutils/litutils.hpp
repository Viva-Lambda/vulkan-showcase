#pragma once
// literal types and related functions
#include <external.hpp>
namespace vtuto {

template <class T> class const_obj {
  const T *s;
  std::size_t len;

public:
  template <std::size_t Nb>
  constexpr const_obj(const T (&a)[Nb]) : s(a), len(Nb - 1) {}
};
typedef const_obj<char> const_str;
typedef const_obj<int> const_ints;
typedef const_obj<bool> const_bools;
typedef const_obj<unsigned int> const_uints;
typedef const_obj<float> const_floats;
typedef const_obj<double> const_doubles;

} // namespace vtuto
