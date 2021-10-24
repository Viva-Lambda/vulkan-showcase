#pragma once
// literal types and related functions
#include <external.hpp>
namespace vtuto {

template <class T> class const_obj {
  const T *_obj;
  std::size_t _max_index;
  std::size_t len;

public:
  template <std::size_t Nb>
  constexpr const_obj(const T (&a)[Nb])
      : _obj(a), _max_index(Nb - 1), len(Nb) {}
  constexpr const T *obj() const { return _obj; }
  constexpr std::size_t last() const { return _max_index; }
  constexpr std::size_t length() const { return len; }
};
typedef const_obj<char> const_str;
typedef const_obj<int> const_ints;
typedef const_obj<bool> const_bools;
typedef const_obj<unsigned int> const_uints;
typedef const_obj<float> const_floats;
typedef const_obj<double> const_doubles;

} // namespace vtuto
