#pragma once

// rasterization state of vk pipeline
#include <external.hpp>

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
- pNext is NULL or a pointer to a structure extending this structure.
- flags is reserved for future use.
- depthClampEnable controls whether to clamp the fragment’s depth values as
described in Depth Test. If the pipeline is not created with
VkPipelineRasterizationDepthClipStateCreateInfoEXT present then enabling depth
clamp will also disable clipping primitives to the z planes of the frustrum as
described in Primitive Clipping. Otherwise depth clipping is controlled by the
state set in VkPipelineRasterizationDepthClipStateCreateInfoEXT.
- rasterizerDiscardEnable controls whether primitives are discarded
immediately before the rasterization stage.
- polygonMode is the triangle rendering mode. See VkPolygonMode.
- cullMode is the triangle facing direction used for primitive culling. See
VkCullModeFlagBits.
- frontFace is a VkFrontFace value specifying the front-facing triangle
orientation to be used for culling.
- depthBiasEnable controls whether to bias fragment depth values.
- depthBiasConstantFactor is a scalar factor controlling the constant depth
value added to each fragment.
- depthBiasClamp is the maximum (or minimum) depth bias of a fragment.
- depthBiasSlopeFactor is a scalar factor applied to a fragment’s slope in
depth bias calculations.
- lineWidth is the width of rasterized line segments.

 */
struct PipelineRasterizationStateCreateInfoVk {
  VkPipelineRasterizationStateCreateInfo createInfo;

  PipelineRasterizationStateCreateInfoVk() {}

  // PipelineRasterizationStateCreateInfoVk() {}
};
} // namespace vtuto
