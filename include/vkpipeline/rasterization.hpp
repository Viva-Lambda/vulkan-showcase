#pragma once

// rasterization state of vk pipeline
#include <external.hpp>
#include <vkutils/varia.hpp>

namespace vtuto {
//
/**
typedef struct VkPipelineRasterizationStateCreateInfo {

VkStructureType sType;

const void* pNext;

VkPipelineRasterizationStateCreateFlags flags;

VkBool32 depthClampEnable;

VkBool32 rasterizerDiscardEnable;

VkPolygonMode polygonMode;

VkCullModeFlags cullMode;

VkFrontFace frontFace;

VkBool32 depthBiasEnable;

float depthBiasConstantFactor;

float depthBiasClamp;

float depthBiasSlopeFactor;

float lineWidth;
} VkPipelineRasterizationStateCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this
structure.
- flags is reserved for future use.
- depthClampEnable controls whether to clamp the fragment’s
depth values as
described in Depth Test. If the pipeline is not created with
VkPipelineRasterizationDepthClipStateCreateInfoEXT present
then enabling depth
clamp will also disable clipping primitives to the z planes
of the frustrum as
described in Primitive Clipping. Otherwise depth clipping is
controlled by the
state set in
VkPipelineRasterizationDepthClipStateCreateInfoEXT.
- rasterizerDiscardEnable controls whether primitives are
discarded
immediately before the rasterization stage.
- polygonMode is the triangle rendering mode. See
VkPolygonMode.
- cullMode is the triangle facing direction used for
primitive culling. See
VkCullModeFlagBits.
- frontFace is a VkFrontFace value specifying the
front-facing triangle
orientation to be used for culling.
- depthBiasEnable controls whether to bias fragment depth
values.
- depthBiasConstantFactor is a scalar factor controlling the
constant depth
value added to each fragment.
- depthBiasClamp is the maximum (or minimum) depth bias of a
fragment.
- depthBiasSlopeFactor is a scalar factor applied to a
fragment’s slope in
depth bias calculations.
- lineWidth is the width of rasterized line segments.

 */

constexpr void VkFlagSetter(
    VkPipelineRasterizationStateCreateInfo &createInfo,
    const VkBool32 &depthClampEnable,
    const VkBool32 &rasterizerDiscardEnable,
    const VkPolygonMode &polygonMode,
    const VkCullModeFlags &cullMode,
    const VkFrontFace &frontFace,
    const VkBool32 &depthBiasEnable) {
  //
  createInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  createInfo.depthClampEnable = depthClampEnable;
  createInfo.rasterizerDiscardEnable =
      rasterizerDiscardEnable;
  createInfo.polygonMode = polygonMode;
  createInfo.lineWidth = 1.0f;
  createInfo.cullMode = cullMode;
  createInfo.frontFace = frontFace;
  createInfo.depthBiasEnable = depthBiasEnable;
}
void VkOptSetter(
    VkPipelineRasterizationStateCreateInfo &createInfo,
    const std::optional<float> &depthBiasConstantFactorRef = std::nullopt,
    const std::optional<float> &depthBiasClampRef = std::nullopt,
    const std::optional<float> &depthBiasSlopeFactorRef = std::nullopt,
    const std::optional<float> &lineWidthRef = std::nullopt) {
  if (depthBiasSlopeFactorRef.has_value()) {
    auto d = depthBiasSlopeFactorRef.value();
    createInfo.depthBiasSlopeFactor = d;
  }
  if (depthBiasConstantFactorRef.has_value()) {
    auto d = depthBiasConstantFactorRef.value();
    createInfo.depthBiasConstantFactor = d;
  }
  if (depthBiasClampRef.has_value()) {
    auto d = depthBiasClampRef.value();
    createInfo.depthBiasClamp = d;
  }
  if (lineWidthRef.has_value()) {
    auto d = lineWidthRef.value();
    createInfo.lineWidth = d;
  }
}
} // namespace vtuto
