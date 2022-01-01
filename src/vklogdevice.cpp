#include <hellotriangle.hpp>
using namespace vtuto;

namespace vtuto {
void HelloTriangle::createLogicalDevice() {
  logical_dev = vulkan_device<VkDevice>(enableValidationLayers, physical_dev);
}

} // namespace vtuto
