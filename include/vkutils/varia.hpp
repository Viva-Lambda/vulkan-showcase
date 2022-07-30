#pragma once
// general utility functions

#include <external.hpp>
#include <vkutils/temputils.hpp>

namespace vtuto {

struct NextSetter {

  template <typename Parent, typename PNextType>
  static void set(Parent &obj,
                  std::optional<PNextType> &pNext) {
    if (pNext.has_value()) {
      auto val = pNext.value();
      if (std::is_pointer<PNextType>::value) {
        obj.pNext = static_cast<void *>(val);
      } else {
        obj.pNext = static_cast<void *>(&val);
      }
    }
  }
};

/** set flags to structs at compile time */
constexpr void VkFlagSetter() {}

/** set optional arguments to structs at run time */
void VkOptSetter() {}

/** set normal arguments to structs at run time */
void VkArgSetter() {}

/** a template for setting same sized arrays to structs at runtime*/
template <class StructType, std::size_t N, class... Ts>
struct VkArraySetter {
  static void set(StructType &s, const std::array<Ts, N> &... arrs) {}
};

template <class FlagBitMask, unsigned long... FlagBits>
class flags_vk {
  //
public:
  constexpr FlagBitMask mask() { return (FlagBits | ...); }
};

template <class ObjType, class FlagBitList, class... Args>
struct VkStructSetter {
  static void set(ObjType &obj, FlagBitList &f,
                  Args &... args) {}
};

template <class ObjType, unsigned long... FlagBits,
          typename... Args>
struct VkStructSetter<ObjType,
                      UnsignedLongContainer<FlagBits...>,
                      TypeContainer<Args...>> {
  constexpr std::array<unsigned long, sizeof...(FlagBits)>
  flag_bits() {
    std::array<unsigned long, sizeof...(FlagBits)> arr = {
        FlagBits...};
    return arr;
  }

  constexpr unsigned long flag_mask() {
    return (FlagBits | ...);
  }

  static void set(ObjType &obj, Args &... args) {}
};

} // namespace vtuto
