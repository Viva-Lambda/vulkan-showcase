// device template for physical and logical devices
#pragma once

namespace vtuto {
template <class VDeviceType> class vulkan_device {

public:
  VDeviceType device();
  void destroy() {}
};
}
