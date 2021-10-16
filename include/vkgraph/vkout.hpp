#pragma once
// output of a computation in graph
// graph like architecture
#include <external.hpp>
#include <vkresult/debug.hpp>

namespace vtuto {

struct vk_output {
  Result_Vk result_info;
  unsigned int signal;
};
} // namespace vtuto
