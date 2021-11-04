#pragma once
#include <external.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {
/**
 */

struct EngineVersion {
  constexpr EngineVersion(unsigned int maj, unsigned int min, unsigned int alp)
      : major(maj), minor(min), alpha(alp) {}
  unsigned int major = 1;
  unsigned int minor = 0;
  unsigned int alpha = 0;
};

struct AppVersion {
  constexpr AppVersion(unsigned int maj, unsigned int min, unsigned int alp)
      : major(maj), minor(min), alpha(alp) {}
  unsigned int major = 1;
  unsigned int minor = 0;
  unsigned int alpha = 0;
};

template <std::uint32_t ApiVersion, typename T = void>
VkApplicationInfo mkAppInfo(const EngineVersion &e, const AppVersion &a,
                            const std::string &app_name,
                            const std::string &engine_name, const T extension) {
  //
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = app_name.c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(a.major, a.minor, a.alpha);
  appInfo.pEngineName = engine_name.c_str();
  appInfo.engineVersion = VK_MAKE_VERSION(e.major, e.minor, e.alpha);
  appInfo.apiVersion = ApiVersion;
  appInfo.pNext = &extension;
  return appInfo;
}

} // namespace vtuto
