// debugging related code for deprecating stdexcept
#pragma once
#include <external.hpp>

namespace vtuto {

/**
  All success code explanations are taken from
  vulkan specification 1.2
 */
std::string toString(VkResult r) {
  std::string str = "";
  if (r == VK_SUCCESS) {
    str = "SuccessCode :: Command successfully completed";
  } else if (r == VK_NOT_READY) {
    str = "SuccessCode :: A fence or query has not yet completed";
  } else if (r == VK_TIMEOUT) {
    str =
        R"(SuccessCode :: A wait operation has not completed in the specified time)";
  } else if (r == VK_EVENT_SET) {
    str = "SuccessCode :: An event is signaled";
  } else if (r == VK_EVENT_RESET) {
    str = "SuccessCode :: An event is unsignaled";
  } else if (r == VK_INCOMPLETE) {
    str = "SuccessCode :: A return array was too small for the result";
  } else if (r == VK_SUBOPTIMAL_KHR) {
    str =
        R"(
SuccessCode :: A swapchain no longer matches the surface properties exactly,
but can still be used to present to the surface successfully
)";
  } else if (r == VK_THREAD_IDLE_KHR) {
    str = R"(
SuccessCode :: A deferred operation is not complete but there is currently no
work for this thread to do at the time of this call.
)";
  } else if (r == VK_THREAD_DONE_KHR) {
    str = R"(
SuccessCode :: A deferred operation is not complete but there is no work
remaining to assign to additional threads
)";
  } else if (r == VK_OPERATION_DEFERRED_KHR) {
    str =
        R"(
SuccessCode :: A deferred operation was requested and at least some of the
work was deferred
)";
  } else if (r == VK_OPERATION_NOT_DEFERRED_KHR) {
    str = R"(
SuccessCode :: A deferred operation was requested and no operations were
deferred.)";
  } else if (r == VK_PIPELINE_COMPILE_REQUIRED_EXT) {
    str = R"(
SuccessCode :: A requested pipeline creation would have required compilation,
but the application requested compilation to not be performed.
)";
  } else if (r == VK_ERROR_OUT_OF_HOST_MEMORY) {
    str = "ErrorCode :: A host memory allocation has failed.";
  } else if (r == VK_ERROR_OUT_OF_DEVICE_MEMORY) {
    str = R"(ErrorCode :: A device memory allocation has failed.)";
  } else if (r == VK_ERROR_INITIALIZATION_FAILED) {
    str = R"(
ErrorCode :: Initialization of an object could not be completed for
implementation-specific reasons
)";
  } else if (r == VK_ERROR_DEVICE_LOST) {
    str = R"(ErrorCode :: The logical or physical device has been lost.)";
  } else if (r == VK_ERROR_MEMORY_MAP_FAILED) {
    str = "ErrorCode :: Mapping of a memory object has failed";
  } else if (r == VK_ERROR_LAYER_NOT_PRESENT) {
    str =
        "ErrorCode :: A requested layer is not present or could not be loaded";
  } else if (r == VK_ERROR_EXTENSION_NOT_PRESENT) {
    str = "ErrorCode :: A requested extension is not supported";
  } else if (r == VK_ERROR_FEATURE_NOT_PRESENT) {
    str = "ErrorCode :: A requested feature is not supported.";
  } else if (r == VK_ERROR_INCOMPATIBLE_DRIVER) {
    str = R"(
ErrorCode :: The requested version of Vulkan is not supported by the driver or
is otherwise incompatible for implementation-specific reasons
)";
  } else if (r == VK_ERROR_TOO_MANY_OBJECTS) {
    str = "ErrorCode :: Too many objects of the type have already been created";
  } else if (r == VK_ERROR_FORMAT_NOT_SUPPORTED) {
    str = "ErrorCode :: A requested format is not supported on this device";
  } else if (r == VK_ERROR_FRAGMENTED_POOL) {
    str = R"(
ErrorCode :: A pool allocation has failed due to fragmentation of the pool’s
memory. This must only be returned if no attempt to allocate host or device
memory was made to accommodate the new allocation. This should be returned in
preference to VK_ERROR_OUT_OF_POOL_MEMORY, but only if the implementation is
certain that the pool allocation failure was due to fragmentation.
)";
  } else if (r == VK_ERROR_SURFACE_LOST_KHR) {
    str = "ErrorCode :: A surface is no longer available";
  } else if (r == VK_ERROR_NATIVE_WINDOW_IN_USE_KHR) {
    str = R"(
ErrorCode :: The requested window is already in use by Vulkan or another API
in a manner which prevents it from being used again
)";
  } else if (r == VK_ERROR_OUT_OF_DATE_KHR) {
    str = R"(
ErrorCode :: A surface has changed in such a way that it is no longer
compatible with the swapchain, and further presentation requests using the
swapchain will fail. Applications must query the new surface properties and
recreate their swapchain if they wish to continue presenting to the surface
)";
  } else if (r == VK_ERROR_INCOMPATIBLE_DISPLAY_KHR) {
    str = R"(
ErrorCode :: The display used by a swapchain does not use the same presentable
image layout, or is incompatible in a way that prevents sharing an image
)";
  } else if (r == VK_ERROR_INVALID_SHADER_NV) {
    str = R"(
ErrorCode :: One or more shaders failed to compile or link. More details are
reported back to the application via VK_EXT_debug_report if enabled
)";
  } else if (r == VK_ERROR_OUT_OF_POOL_MEMORY) {
    str = R"(
ErrorCode :: A pool memory allocation has failed. This must only be returned
if no attempt to allocate host or device memory was made to accommodate the
new allocation. If the failure was definitely due to fragmentation of the
pool, VK_ERROR_FRAGMENTED_POOL should be returned instead
)";
  } else if (r == VK_ERROR_INVALID_EXTERNAL_HANDLE) {
    str = R"(
ErrorCode :: An external handle is not a valid handle of the specified type
)";
  } else if (r == VK_ERROR_FRAGMENTATION) {
    str = R"(
ErrorCode :: A descriptor pool creation has failed due to fragmentation.
)";
  } else if (r == VK_ERROR_INVALID_DEVICE_ADDRESS_EXT) {
    str = R"(
ErrorCode :: A buffer creation failed because the requested address is not
available
)";
  } else if (r == VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS) {
    str = R"(
ErrorCode :: A buffer creation or memory allocation failed because the
requested address is not available. A shader group handle assignment failed
because the requested shader group handle information is no longer valid
)";
  } else if (r == VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT) {
    str = R"(
ErrorCode :: An operation on a swapchain created with
VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed as it did not have
exlusive full-screen access. This may occur due to implementation-dependent
reasons, outside of the application’s control.
)";
  } else if (r == VK_ERROR_UNKNOWN) {
    str = R"(
ErrorCode :: An unknown error has occurred; either the application has
provided invalid input, or an implementation failure has occurred
)";
  } else {
    str = "Unknown result error code";
  }
  return str;
}

std::ostream &operator<<(std::ostream &out, VkResult r) {
  out << toString(r);
  return out;
}

enum status_t_vk : uint_least8_t {
  SUCCESS_OP = 0,
  FAIL_OP = 1,
  GLFW_ERROR = 2,
  STRUCT_PARAM_ERROR_VK = 3,
  STRUCT_TYPE_ERROR_VK = 4,
  PNEXT_STYPE_CHAIN_ERROR_VK = 5,
  INACTIVE_VALIDATION_LAYERS_VK = 6,
  IMAGE_SUBRESOURCE_RANGE_ERROR_VK = 7,
  COMPONENT_SWIZZLE_ERROR_VK = 8
};

std::string toString(status_t_vk r) {
  std::string str = "";
  if (r == SUCCESS_OP) {
    str = "Success :: custom operation SUCCESSFUL";
  } else if (r == FAIL_OP) {
    str = "Error :: FAIL_OP :: custom operation FAILED";
  } else if (r == STRUCT_PARAM_ERROR_VK) {
    str = "Error :: STRUCT_PARAM_ERROR_VK :: struct parameter";
  } else if (r == STRUCT_TYPE_ERROR_VK) {
    str = "Error :: STRUCT_TYPE_ERROR_VK :: struct type";
  } else if (r == PNEXT_STYPE_CHAIN_ERROR_VK) {
    str = "Error :: PNEXT_STYPE_CHAIN_ERROR_VK :: stype error in pnext chain";
  } else if (r == INACTIVE_VALIDATION_LAYERS_VK) {
    str =
        "Error :: INACTIVE_VALIDATION_LAYERS_VK :: inactive validation layers";
  } else if (r == GLFW_ERROR) {
    str = "Error :: GLFW_ERROR :: error in glfw call";
  } else if (r == IMAGE_SUBRESOURCE_RANGE_ERROR_VK) {
    str = "Error :: IMAGE_SUBRESOURCE_RANGE_ERROR_VK :: error in image "
          "subresource range";
  }
  return str;
}
/**
  \param filepath contains the path to the file which called the function
  \param fn_name contains the caller function name
  \param context any contextual information that might help identifying the
  result. For example, arguments, or the calling object name, etc.
  \param call_info function call that produces the result most probably filled
  with a checking macro
  \param description description message for the vulkan result code, most
  probably filled with a checking macro
  \param spec_info if the error is caught by struct checker then this slot is
  filled with a valid usage information provided by the specification.
  \param status_info contains descriptive information with respect to status
  code provided by our api.
  \param status contains status code provided by our api.
 */
struct Result_Vk {
  unsigned int line = 0;
  std::string filepath = "";
  std::string fn_name = "";
  std::string context = "";
  std::string call_info = "";
  std::string description = "";
  std::string spec_info = "";
  std::string status_info = "";
  status_t_vk status;
};

#define UPDATE_RESULT_VK(res, msg)                                             \
  do {                                                                         \
    res.line = __LINE__;                                                       \
    res.filepath = __FILE__;                                                   \
    res.fn_name = __FUNCTION__;                                                \
    res.context = msg;                                                         \
  } while (0)

#define CHECK_VK(call, msg, res)                                               \
  do {                                                                         \
    VkResult r = call;                                                         \
    if (r != VK_SUCCESS) {                                                     \
      res.line = __LINE__;                                                     \
      res.description = toString(r);                                           \
      res.fn_name = __FUNCTION__;                                              \
      res.filepath = __FILE__;                                                 \
      res.context = msg;                                                       \
      res.call_info = #call;                                                   \
      res.status = FAIL_OP;                                                    \
      res.status_info = toString(FAIL_OP);                                     \
    } else {                                                                   \
      res.status = SUCCESS_OP;                                                 \
      res.description = toString(r);                                           \
      res.status_info = toString(SUCCESS_OP);                                  \
    }                                                                          \
  } while (0)

#define CHECK_GLFW(res, msg)                                                   \
  do {                                                                         \
    const char *des;                                                           \
    int r = glfwGetError(&des);                                                \
    if (r != GLFW_NO_ERROR) {                                                  \
      res.line = __LINE__;                                                     \
      res.description = std::string(des);                                      \
      res.fn_name = __FUNCTION__;                                              \
      res.filepath = __FILE__;                                                 \
      res.context = msg;                                                       \
      res.status = GLFW_ERROR;                                                 \
      res.status_info = toString(GLFW_ERROR);                                  \
    } else {                                                                   \
      res.status = SUCCESS_OP;                                                 \
      res.description = std::string(des);                                      \
      res.status_info = toString(SUCCESS_OP);                                  \
    }                                                                          \
  } while (0)
} // namespace vtuto
