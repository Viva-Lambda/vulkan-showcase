/**
  parameter checker for all vulkan functions
 */
#pragma once
#include <external.hpp>
#include <vkresult/debug.hpp>

namespace vtuto {
template <class T> struct StructChecker {
  static Result_Vk check(const T &s) {
    Result_Vk r;
    r.line = __LINE__;
    r.filepath = __FILE__;
    r.fn_name = __FUNCTION__;
    r.status = FAIL_OP;
    return r;
  }
};
}; // namespace vtuto
