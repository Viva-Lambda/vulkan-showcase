// main file
#include <debug.hpp>
#include <external.hpp>
#include <hellotriangle.hpp>
#include <ldevice.hpp>
#include <pdevice.hpp>
#include <support.hpp>
#include <triangle.hpp>
#include <ubo.hpp>
#include <utils.hpp>
// #include <vkdefault/instancedefaults.hpp>
//
using namespace vtuto;

namespace vtuto {

void HelloTriangle::draw() {
  vkWaitForFences(logical_dev.device(), 1, &current_fences[current_frame],
                  VK_TRUE, UINT64_MAX);

  uint32_t image_index;
  VkResult res = vkAcquireNextImageKHR(
      logical_dev.device(), swap_chain.chain, UINT64_MAX,
      image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

  if (res == VK_ERROR_OUT_OF_DATE_KHR) {
    //
    recreateSwapchain();
    return;
  } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("swap chain image request failed");
  }

  if (images_in_flight[image_index] != VK_NULL_HANDLE) {
    vkWaitForFences(logical_dev.device(), 1, &images_in_flight[image_index],
                    VK_TRUE, UINT64_MAX);
  }
  images_in_flight[image_index] = current_fences[current_frame];

  // update uniform
  updateUniformBuffer(image_index);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {image_available_semaphores[current_frame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  auto v = cmd_buffers.get(image_index);
  submitInfo.pCommandBuffers = &v;

  VkSemaphore signalSemaphores[] = {render_finished_semaphores[current_frame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  vkResetFences(logical_dev.device(), 1, &current_fences[current_frame]);
  //
  CHECK_VK2(vkQueueSubmit(logical_dev.graphics_queue, 1, &submitInfo,
                          current_fences[current_frame]),
            "failed to submit draw command buffer");
  //
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swap_chains[] = {swap_chain.chain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swap_chains;
  presentInfo.pImageIndices = &image_index;

  res = vkQueuePresentKHR(logical_dev.present_queue, &presentInfo);

  if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR ||
      framebuffer_resized) {
    framebuffer_resized = false;
    recreateSwapchain();
  } else if (res != VK_SUCCESS) {
    //
    throw std::runtime_error("failed to present swap chain image");
  }

  //
  current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}
}
