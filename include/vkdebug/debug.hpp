// vk debug utils
#pragma once
#include <external.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

namespace vtuto {

/**
  Enable validation layers for debug build. NDEBUG macro is
  a part of c++ standard.
 */

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

std::ostream &operator<<(std::ostream &out,
                         VkDebugUtilsMessageSeverityFlagBitsEXT b) {
  if (b == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    out << "Severity level: verbose" << std::endl;
  } else if (b == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
    out << "Severity level: info" << std::endl;
  } else if (b == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    out << "Severity level: warning" << std::endl;
  } else if (b == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    out << "Severity level: warning" << std::endl;
  } else {
    out << "Severity level: unknown" << std::endl;
  }
  return out;
}

std::ostream &operator<<(std::ostream &out,
                         VkDebugUtilsMessageTypeFlagBitsEXT b) {
  if (b == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
    out << "Message type: general" << std::endl;
  } else if (b == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
    out << "Message level: validation" << std::endl;
  } else if (b == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
    out << "Message level: performance" << std::endl;
  } else {
    out << "Message level: unknown" << std::endl;
  }
  return out;
}

struct DebugUtilsCreateInfoExt {

  // struct type
private:
  VkStructureType _stype =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

  // reserved for future use
  VkDebugUtilsMessengerCreateFlagsEXT _flags = 0;

  void *_pNext = NULL;

public:
  /**
      Determines message severity. If null, messages are
  skipped

- VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT specifies the most verbose
output indicating all diagnostic messages from the Vulkan loader, layers, and
drivers should be captured.

- VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT specifies an informational
message such as resource details that may be handy when debugging an
application.

- VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT specifies use of Vulkan that
may expose an app bug. Such cases may not be immediately harmful, such as a
fragment shader outputting to a location with no attachment. Other cases may
point to behavior that is almost certainly bad when unintended such as using
an image whose memory has not been filled. In general if you see a warning but
you know that the behavior is intended/desired, then simply ignore the
warning.

- VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT specifies that the application
has violated a valid usage condition of the specification.
     */
  std::vector<VkDebugUtilsMessageSeverityFlagBitsEXT> severity_flags;

  /**

- VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT specifies that some general
event has occurred. This is typically a non-specification, non-performance
event.

- VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT specifies that something has
occurred during validation against the Vulkan specification that may indicate
invalid behavior.

- VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT specifies a potentially
non-optimal use of Vulkan, e.g. using vkCmdClearColorImage when setting
VkAttachmentDescription::loadOp to VK_ATTACHMENT_LOAD_OP_CLEAR would have
worked.
     */

  std::vector<VkDebugUtilsMessageTypeFlagBitsEXT> message_type_bits;

  // user debugging call back
  /**
    pfnUserCallback: specified as a static function in this
    api.
    typedef VkBool32 (VKAPI_PTR
    *PFN_vkDebugUtilsMessangerCallbackEXT) (
            VkDebugUtilsMessageSeverityFlagBitsEXT
    messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT*
            pCallbackData,
            void* pUserData)
    \param messageSeverity: same as createInfo
    \param messageType: same as createInfo
    \param VkDebugUtilsMessengerCallbackDataEXT
   */
  PFN_vkDebugUtilsMessengerCallbackEXT callbackFn;

  // user data
  void *userData = nullptr;

  void *pnext() const { return _pNext; }

  /**
    add a struct to pnext chain
   */
  void add_pnext(VkBaseInStructure s) { _pNext = static_cast<void *>(&s); }

  VkDebugUtilsMessengerCreateInfoEXT mkDebugMessengerInfo() const {
    //

    // create messenger type
    VkDebugUtilsMessageTypeFlagsEXT messengerType = message_type_bits[0];
    unsigned int i = 1;
    for (i; i < message_type_bits.size(); i++) {
      // bitwise concat
      messengerType |= message_type_bits[i];
    }

    // create messenger severity
    VkDebugUtilsMessageSeverityFlagsEXT severity = severity_flags[0];
    i = 1;
    for (i; i < severity_flags.size(); i++) {
      // bitwise concat
      severity |= severity_flags[i];
    }

    // create info
    /**
    typedef struct VkDebugUtilsMessengerCreateInfoEXT {
        VkStructureType sType;
        const void* pNext;
        VkDebugUtilsMessengerCreateFlagsEXT flags;
        VkDebugUtilsMessageSeverityFlagsEXT messageSeverity;
        VkDebugUtilsMessageTypeFlagsEXT messageType;
        PFN_vkDebugUtilsMessengerCallbackEXT
    pfnUserCallback;
        void* pUserData;
    } VkDebugUtilsMessengerCreateInfoEXT;

    - sType is the type of this structure.

    - pNext is NULL or a pointer to a structure extending
    this structure.

    - flags is 0 and is reserved for future use.

    - messageSeverity is a bitmask of
    VkDebugUtilsMessageSeverityFlagBitsEXT
    specifying which severity of event(s) will cause this
    callback to be called.

    - messageType is a bitmask of
    VkDebugUtilsMessageTypeFlagBitsEXT specifying
    which type of event(s) will cause this callback to be
    called.

    - pfnUserCallback is the application callback function
    to call.

    - pUserData is user data to be passed to the callback.

     */
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.sType = stype();
    createInfo.messageSeverity = severity;
    createInfo.messageType = messengerType;
    createInfo.pfnUserCallback = callbackFn;
    createInfo.pUserData = userData;
    createInfo.pNext = pnext();
    createInfo.flags = flags();

    return createInfo;
  }
  VkStructureType stype() const { return _stype; }
  VkDebugUtilsMessengerCreateFlagsEXT flags() const { return _flags; }
};
/**

Valid Usage (Explicit):

- pfnUserCallback must be a valid
PFN_vkDebugUtilsMessengerCallbackEXT


Valid Usage (Implicit)

- sType must be
VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT
- flags must be 0
- messageSeverity must be a valid combination of
VkDebugUtilsMessageSeverityFlagBitsEXT values
- messageSeverity must not be 0
- messageType must be a valid combination of
VkDebugUtilsMessageTypeFlagBitsEXT values
- messageType must not be 0
- pfnUserCallback must be a valid
PFN_vkDebugUtilsMessengerCallbackEXT value
*/
template <> struct StructChecker<DebugUtilsCreateInfoExt> {
  static Result_Vk check(const DebugUtilsCreateInfoExt &params) {
    Result_Vk vr;
    vr.status = SUCCESS_OP;

    // check type
    if (params.stype() !=
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT) {
      vr.status = STRUCT_TYPE_ERROR_VK;
      return vr;
    }

    // check message types
    if (params.message_type_bits.size() == 0) {
      vr.status = STRUCT_PARAM_ERROR_VK;
      return vr;
    }
    // check message severity
    if (params.severity_flags.size() == 0) {
      vr.status = STRUCT_PARAM_ERROR_VK;
      return vr;
    }

    // check flags
    if (params.flags() != 0) {
      vr.status = STRUCT_PARAM_ERROR_VK;
      return vr;
    }
    return vr;
  }
};

Result_Vk createDebugMessenger(VkInstance &instance,
                               VkDebugUtilsMessengerCreateInfoEXT &info,
                               VkDebugUtilsMessengerEXT &messenger) {
  Result_Vk vr;
  if (!enableValidationLayers) {
    vr.status = INACTIVE_VALIDATION_LAYERS_VK;
    return vr;
  }
  std::string msg = "failed to create and setup debug messenger";
  CHECK_VK(CreateDebugUtilsMessengerExt(instance, &info, nullptr, &messenger),
           msg, vr);
  return vr;
}

} // namespace vtuto
