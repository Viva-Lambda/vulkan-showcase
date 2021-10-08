#pragma once
#include <external.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {
/**
 */
struct AppInfo_Vk_Params {
private:
  VkStructureType _stype = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  void *_pNext = NULL;

public:
  std::string app_name = "My Triangle";
  unsigned int app_version[3];
  std::string engine_name = "No Engine";
  unsigned int engine_version[3];
  uint32_t api_version = VK_API_VERSION_1_0;

  void *pnext() const { return _pNext; }
  VkStructureType stype() const { return _stype; }

  /** Make application info struct with given parameters*/
  VkApplicationInfo mkApplicationInfo() const {
    VkApplicationInfo appInfo{};
    appInfo.sType = stype();
    appInfo.pApplicationName = app_name.c_str();
    /**semver uint app version*/
    unsigned int major = app_version[0];
    unsigned int minor = app_version[1];
    unsigned int alpha = app_version[2];
    appInfo.applicationVersion = VK_MAKE_VERSION(major, minor, alpha);
    appInfo.pEngineName = engine_name.c_str();
    /**semver uint engine version*/
    major = engine_version[0];
    minor = engine_version[1];
    alpha = engine_version[2];

    appInfo.engineVersion = VK_MAKE_VERSION(major, minor, alpha);
    appInfo.apiVersion = api_version;
    return appInfo;
  }
};

/**
  Check AppInfo_Vk_Params values as per vulkan specification
  1.2

  Specification mentions the following for valid usage:

  If apiVersion is not 0, then it MUST be greater than or
  equal to VK_API_VERSION_1_0
 */
template <> struct StructChecker<AppInfo_Vk_Params> {
  static Result_Vk check(const AppInfo_Vk_Params &params) {
    Result_Vk vr;
    // check api version. If it is not zero it must be
    // bigger than 1.0
    if (params.api_version != 0) {
      if (params.api_version < VK_API_VERSION_1_0) {
        vr.status = STRUCT_PARAM_ERROR_VK;
        return vr;
      }
    }
    if (params.stype() != VK_STRUCTURE_TYPE_APPLICATION_INFO) {
      vr.status = STRUCT_TYPE_ERROR_VK;
      return vr;
    }
    vr.status = SUCCESS_OP;
    return vr;
  }
};

} // namespace vtuto
