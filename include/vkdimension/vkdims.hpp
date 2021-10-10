#pragma once
#include <external.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {

enum class VkDimension { NB_2 = 2, NB_3 = 3 };

template <VkDimension d, class T> struct DimInfo {
  T get(unsigned int i) const { return static_cast<T>(0); }
};

template <> struct DimInfo<VkDimension::NB_2, class T> {
private:
  T e[2];
  T get(unsigned int i) const { return e[i]; }

public:
  T first() const { return get(0); }
  T second() const { return get(1); }
  void set_first(T w) { e[0] = w; }
  void set_second(T h) { e[1] = h; }
};
template <> struct DimInfo<VkDimension::NB_3, class T> {
private:
  T e[3];
  T get(unsigned int i) const { return e[i]; }

public:
  T first() const { return get(0); }
  T second() const { return get(1); }
  T third() const { return get(2); }
  void set_first(T w) { e[0] = w; }
  void set_second(T h) { e[1] = h; }
  void set_third(T d) { e[2] = d; }
};

} // namespace vtuto

} // namespace vtuto
