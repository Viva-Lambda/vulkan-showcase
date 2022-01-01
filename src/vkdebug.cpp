// main file
#include <hellotriangle.hpp>
//
using namespace vtuto;

namespace vtuto {

/**
  Specify properties of the debug messenger callback

  We add its type, requested message severities, message
  types, and we add which debug callback function is going
  to be used
 */
void HelloTriangle::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

  // Determines message severity. If null, messages are
  // skipped:

  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
  // specifies
  // the most verbose output from validation layers

  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
  // specifies
  // messages that *may* indicate an api bug.

  // VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: outputs
  // error
  // message when the api has violated a valid usage
  // condition of the
  // specification.
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  // VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: specifies
  // if
  // some general event has occured

  // VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
  // specifies if
  // some event has occured during the validation stage
  // against the vulkan
  // specification, meaning that the application has either
  // an undefined
  // behaviour or an error.

  // VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
  // specifies
  // if a potential suboptimal use of Vulkan API is
  // happening in the
  // application.
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

  // pfnUserCallback: specified as a static function in this
  // api.
  // typedef VkBool32 (VKAPI_PTR
  // *PFN_vkDebugUtilsMessangerCallbackEXT
  // )(VkDebugUtilsMessageSeverityFlagBitsEXT
  // messageSeverity,
  //   VkDebugUtilsMessageTypeFlagsEXT messageTypes,
  //   const VkDebugUtilsMessengerCallbackDataEXT*
  //  pCallbackData,
  //  void* pUserData )
  // \param messageSeverity: same as createInfo
  // \param messageType: same as createInfo
  // VkDebugUtilsMessengerCallbackDataEXT
  createInfo.pfnUserCallback = debugCallback;
}

/**
Set @see debugMessenger up by populating its related
info.
*/

void HelloTriangle::setupDebugMessenger() {
  if (!enableValidationLayers)
    return;
  //
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);

  CHECK_VK2(CreateDebugUtilsMessengerExt(instance, &createInfo, nullptr,
                                         &debugMessenger),
            "failed to create and setup debug messenger");
}

} // namespace vtuto
