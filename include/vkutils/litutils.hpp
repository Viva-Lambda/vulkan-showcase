#pragma once
// literal types and related functions
#include <external.hpp>
namespace vtuto {

template <class T> class array_vk {
  const T *_obj;
  std::size_t _max_index;
  std::size_t len;

public:
  template <std::size_t Nb>
  constexpr array_vk(const T (&arr)[Nb])
      : _obj(arr), _max_index(Nb - 1), len(Nb) {}
  constexpr const T *obj() const { return _obj; }
  constexpr std::size_t last() const { return _max_index; }
  constexpr std::size_t length() const { return len; }
};

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
  bool operator==(const const_obj<T> &other) const {
    if (other.last() != last())
      return false;
    for (unsigned int i = 0; i <= last(); i++) {
      if (other.obj()[i] != obj()[i])
        return false;
    }
    return true;
  }
  bool operator!=(const const_obj<T> &other) const {
    auto r = !((*this) == other);
    return r;
  }
};
template <class T> struct const_comp {
  bool operator()(const const_obj<T> &a,
                  const const_obj<T> &b) const {
    return a.obj() < b.obj();
  }
};
typedef const_obj<char> const_str;
typedef const_obj<int> const_ints;
typedef const_obj<bool> const_bools;
typedef const_obj<unsigned int> const_uints;
typedef const_obj<std::uint32_t> const_uint32s;
typedef const_obj<float> const_floats;
typedef const_obj<double> const_doubles;

struct const_uint_longs : const_obj<unsigned long> {

public:
  template <std::size_t Nb>
  constexpr const_uint_longs(const unsigned long (&a)[Nb])
      : const_obj<unsigned long>(a) {}

  constexpr unsigned long concat() {
    auto f = obj()[0];
    for (std::size_t i = 1; i <= last(); i++) {
      f |= obj()[i];
    }
    return f;
  }
};

} // namespace vtuto
