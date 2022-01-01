#pragma once
#include <external.hpp>
#include <vkdimension/vkdims.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {

struct ExtentInfo2D {
private:
  DimInfo<VkDimension::NB_2, std::uint32_t> diminfo;

public:
  std::uint32_t width() const { return diminfo.first(); }
  std::uint32_t height() const { return diminfo.second(); }
  void set_width(std::uint32_t w) { diminfo.set_first(w); }
  void set_height(std::uint32_t h) { diminfo.set_second(h); }
};
struct ExtentInfo3D {
private:
  DimInfo<VkDimension::NB_3, std::uint32_t> diminfo;

public:
  std::uint32_t width() const { return diminfo.first(); }
  std::uint32_t height() const { return diminfo.second(); }
  std::uint32_t depth() const { return diminfo.third(); }
  void set_width(std::uint32_t w) { diminfo.set_first(w); }
  void set_height(std::uint32_t h) { diminfo.set_second(h); }
  void set_depth(std::uint32_t d) { diminfo.set_third(d); }
};

} // namespace vtuto
