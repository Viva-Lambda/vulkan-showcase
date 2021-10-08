#include <hellotriangle.hpp>
using namespace vtuto;

namespace vtuto {

void HelloTriangle::createPhysicalDevice() {

  /**
    3. Create physical device
    Create physical device by creating the surface from glfw window.
    Then we enumerate available physical devices in the host machine. This
    does not necessarily mean having multiple gpu's, it is possible that the
    cpu also understands vulkan (intel?), however the major use case for this
    is having multiple gpus.
    Each devices is tested against a set of conditions. These conditions are
    determined by the nature of our application. The criteria is specified in
    the body of is_device_suitable() function.
   */
  physical_dev = vulkan_device<VkPhysicalDevice>(&instance, window);
}
} // namespace vtuto
