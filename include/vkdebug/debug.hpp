// vk debug utils
#pragma once
#include <external.hpp>
#include <vkresult/debug.hpp>
#include <vkutils/temputils.hpp>

namespace vtuto {

typedef decltype(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) mtype_bit;
typedef decltype(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) mseverity_bit;
//
template <std::size_t... Indices, class Tpl, class Ret>
Ret concat_bit_indices(std::index_sequence<Indices...>, const Tpl &t) {
  return (std::get<(Indices)>(t) | ...);
}
template <typename Ret, typename... Args>
constexpr Ret concat_bits(Args... args) {
  //
  auto r = (args | ...);
  return static_cast<Ret>(r);
}

template <typename MessageSeverityBit, typename MessageTypeBit>
VkDebugUtilsMessengerCreateInfoEXT mkDebugMessengerCreateInfo(
    const MessageSeverityBit &sbits, const MessageTypeBit &tbits,
    PFN_vkDebugUtilsMessengerCallbackEXT debugMessengerCallback) {
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageType = tbits;
  createInfo.messageSeverity = sbits;
  createInfo.pfnUserCallback = debugMessengerCallback;
  return createInfo;
}

} // namespace vtuto
