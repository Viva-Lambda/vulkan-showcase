// debugging related code for deprecating stdexcept
#pragma once
#include <external.hpp>

namespace vtuto {

enum status_t_vk : uint_least8_t {
  SUCCESS_VK = 0,
  FAIL_VK = 1,
  STRUCT_PARAM_ERROR_VK = 2,
  STRUCT_TYPE_ERROR_VK = 3,
  STYPE_CHAIN_ERROR_VK = 4,
  INACTIVE_VALIDATION_LAYERS_VK = 5
};

struct Result_Vk {
  unsigned int line = 0;
  std::string filepath = "";
  std::string fn_name = "";
  std::string msg = "";
  std::string description = "";
  status_t_vk status;
};

#define UPDATE_RESULT_VK(res, msg)                                             \
  do {                                                                         \
    res.line = __LINE__;                                                       \
    res.filepath = __FILE__;                                                   \
    res.fn_name = __FUNCTION__;                                                \
    res.msg = msg;                                                             \
  } while (0)

#define CHECK_VK(call, msg, res)                                               \
  do {                                                                         \
    VkResult r = call;                                                         \
    res.line = __LINE__;                                                       \
    res.fn_name = __FUNCTION__;                                                \
    res.filepath = __FILE__;                                                   \
    res.msg = msg;                                                             \
    if (r != VK_SUCCESS) {                                                     \
      res.description = #call;                                                 \
      res.status = FAIL_VK;                                                    \
    } else {                                                                   \
      res.status = SUCCESS_VK;                                                 \
    }                                                                          \
  } while (0)

} // namespace vtuto
