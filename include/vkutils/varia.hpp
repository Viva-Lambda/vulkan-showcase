#pragma once
// general utility functions

#include <external.hpp>
#include <vkutils/temputils.hpp>

namespace vtuto {

struct NextSetter {

  template <typename Parent, typename PNextType>
  static void set(Parent &obj, std::optional<PNextType> &pNext) {
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

template <class ObjType, typename... Args> struct VkStructSetter {

  static void set(ObjType &obj, Args &... args) {}
};

} // namespace vtuto
