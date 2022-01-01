// main file
#include <hellotriangle.hpp>
using namespace vtuto;

namespace vtuto {
/**
  \brief get required extensions from glfw and debug utils

  First we query glfw api for enabled extensions. Then we
  add the debug flag
  to enable validation layers.
 */
std::vector<const char *> HelloTriangle::getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  // we reserve the size for the requested extensions
  // vector
  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);
  if (enableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  return extensions;
}

} // namespace vtuto
