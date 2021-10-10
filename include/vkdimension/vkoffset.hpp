#pragma once
#include <external.hpp>
#include <vkdimension/vkdims.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {

struct OffsetInfo2D {
private:
  DimInfo<VkDimension::NB_2, std::int32_t> diminfo;

public:
  std::int32_t x() const { return diminfo.first(); }
  std::int32_t y() const { return diminfo.second(); }
  void set_x(std::int32_t w) { diminfo.set_first(w); }
  void set_y(std::int32_t h) { diminfo.set_second(h); }
};

struct OffsetInfo3D {
private:
  DimInfo<VkDimension::NB_3, std::int32_t> diminfo;

public:
  std::int32_t x() const { return diminfo.first(); }
  std::int32_t y() const { return diminfo.second(); }
  std::int32_t z() const { return diminfo.third(); }
  //
  void set_x(std::int32_t w) { diminfo.set_first(w); }
  void set_y(std::int32_t h) { diminfo.set_second(h); }
  void set_z(std::int32_t d) { diminfo.set_third(d); }
};

} // namespace vtuto
