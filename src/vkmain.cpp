#include <debug.hpp>
#include <external.hpp>
#include <hellotriangle.hpp>
#include <ldevice.hpp>
#include <pdevice.hpp>
#include <support.hpp>
#include <triangle.hpp>
#include <ubo.hpp>
#include <utils.hpp>
#include "vkappinstance.cpp"
#include "vkextension.cpp"
#include "vulkantuto.cpp"
#include "vkgraphpipeline.cpp"
#include "vkrenderpass.cpp"
#include "vkbuffer.cpp"
#include "vkdescriptor.cpp"
#include "vkdraw.cpp"

using namespace vtuto;

extern "C" int main() {
  std::string wtitle = "Vulkan Window Title";
  HelloTriangle hello(wtitle, (uint32_t)WIDTH, (uint32_t)HEIGHT);

  try {
    hello.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
