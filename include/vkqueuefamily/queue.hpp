#pragma once
#include <external.hpp>

namespace vtuto {
//

typedef std::map<VkQueueFlagBits, std::optional<uint32_t>>
    queue_index_map;

typedef std::map<VkQueueFlagBits, VkQueue> queue_map;
} // namespace vtuto
