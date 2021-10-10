#pragma once
#include <external.hpp>
#include <vkdimension/vkextent.hpp>
#include <vkdimension/vkoffset.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {
//
struct Rect2D_Vk {
  ExtentInfo2D extent;
  OffsetInfo2D offset;
};
} // namespace vtuto
