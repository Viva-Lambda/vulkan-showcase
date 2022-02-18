#pragma once
#include <external.hpp>
#include <vkutils/varia.hpp>

namespace vtuto {
/**
 */

struct VersionInfo {
  constexpr VersionInfo(unsigned int maj, unsigned int min,
                        unsigned int alp)
      : major(maj), minor(min), alpha(alp) {}
  unsigned int major = 1;
  unsigned int minor = 0;
  unsigned int alpha = 0;
};

typedef VersionInfo EngineVersion;
typedef VersionInfo AppVersion;

template <>
struct VkStructSetter<VkApplicationInfo, EngineVersion,
                      AppVersion, std::uint32_t, const_str,
                      const_str> {
  static void
  set(VkApplicationInfo &appInfo, const EngineVersion &e,
      const AppVersion &a, const std::uint32_t &ApiVersion,
      const_str app_name, const_str engine_name) {
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = app_name.obj();
    appInfo.applicationVersion =
        VK_MAKE_VERSION(a.major, a.minor, a.alpha);
    appInfo.pEngineName = engine_name.obj();
    appInfo.engineVersion =
        VK_MAKE_VERSION(e.major, e.minor, e.alpha);
    appInfo.apiVersion = ApiVersion;
    // appInfo.pNext = &extension;
  }
};

template <std::uint32_t ApiVersion, typename T = void>
VkApplicationInfo mkAppInfo(const EngineVersion &e,
                            const AppVersion &a,
                            const std::string &app_name,
                            const std::string &engine_name,
                            const T extension) {
  //
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = app_name.c_str();
  appInfo.applicationVersion =
      VK_MAKE_VERSION(a.major, a.minor, a.alpha);
  appInfo.pEngineName = engine_name.c_str();
  appInfo.engineVersion =
      VK_MAKE_VERSION(e.major, e.minor, e.alpha);
  appInfo.apiVersion = ApiVersion;
  appInfo.pNext = &extension;
  return appInfo;
}

} // namespace vtuto
