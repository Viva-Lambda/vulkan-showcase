#pragma once
#include <external.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {

//
/**
typedef struct VkImageCreateInfo {
  VkStructureType sType;
  const void *pNext;
  VkImageCreateFlags flags;
  VkImageType imageType;
  VkFormat format;
  VkExtent3D extent;
  uint32_t mipLevels;
  uint32_t arrayLayers;
  VkSampleCountFlagBits samples;
  VkImageTiling tiling;
  VkImageUsageFlags usage;
  VkSharingMode sharingMode;
  uint32_t queueFamilyIndexCount;
  const uint32_t *pQueueFamilyIndices;
  VkImageLayout initialLayout;
} VkImageCreateInfo;

- sType is the type of this structure.
- pNext is NULL or a pointer to a structure extending this structure.

- flags is a bitmask of VkImageCreateFlagBits describing additional parameters
of the image.

- imageType is a VkImageType value specifying the basic dimensionality of the
image. Layers in array textures do not count as a dimension for the purposes
of the image type.

- format is a VkFormat describing the format and type of the texel blocks that
will be contained in the image.

- extent is a VkExtent3D describing the number of data elements in each
dimension of the base level.

- mipLevels describes the number of levels of detail available for minified
sampling of the image.

- arrayLayers is the number of layers in the image.

- samples is a VkSampleCountFlagBits value specifying the number of samples
per texel.

- tiling is a VkImageTiling value specifying the tiling arrangement of the
texel blocks in memory.

- usage is a bitmask of VkImageUsageFlagBits describing the intended usage of
the image.

- sharingMode is a VkSharingMode value specifying the sharing mode of the
image when it will be accessed by multiple queue families.

- queueFamilyIndexCount is the number of entries in the pQueueFamilyIndices
array.

- pQueueFamilyIndices is a pointer to an array of queue families that will
access this image. It is ignored if sharingMode is not
VK_SHARING_MODE_CONCURRENT.

- initialLayout is a VkImageLayout value specifying the initial VkImageLayout
of all image subresources of the image. See Image Layouts.

*/
struct ImageCreateInfo {
  //
private:
  VkStructureType _stype = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  void *_pnext = NULL;

public:
  std::set<VkImageCreateFlagBits> img_flags;
  std::set<VkImageUsageFlagBits> usage_flags;
  std::set<std::uint32_t> queue_family_indices;
  VkImageType imageType;
  VkFormat format;
  VkExtent3D extent;
  std::uint32_t mipLevels;
  std::uint32_t arrayLayers;
  VkSampleCountFlagBits samples;
  VkImageTiling tiling;
  VkImageUsageFlags usage;
  VkSharingMode sharingMode;
  VkImageLayout initialLayout;

  VkStructureType stype() const { return _stype; }
  void *pNext() const { return _pnext; }
};

Result_Vk check_sharingMode(const ImageCreateInfo &ic) {
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (ic.sharingMode == VK_SHARING_MODE_CONCURRENT) {
    if (queue_family_indices.size() == 0) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-sharingMode-00941 ::
If sharingMode is VK_SHARING_MODE_CONCURRENT, pQueueFamilyIndices must be a
valid pointer to an array of queueFamilyIndexCount uint32_t values

VUID-VkImageCreateInfo-sharingMode-00942 ::
If sharingMode is VK_SHARING_MODE_CONCURRENT, queueFamilyIndexCount must be
greater than 1
)";
      return vr;
    }
  }
  return vr;
}
Result_Vk check_extent(const ImageCreateInfo &im) {
  //
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (im.extent.width <= 0) {
    vr.status = IMAGE_CREATE_ERROR_VK;
    vr.spec_info = R"(
VUID-VkImageCreateInfo-extent-00944 ::
extent.width must be greater than 0
)";
    return vr;
  }
  if (im.extent.height <= 0) {
    vr.status = IMAGE_CREATE_ERROR_VK;
    vr.spec_info = R"(
VUID-VkImageCreateInfo-extent-00945 ::
extent.height must be greater than 0
)";
    return vr;
  }
  if (im.extent.depth <= 0) {
    vr.status = IMAGE_CREATE_ERROR_VK;
    vr.spec_info = R"(
VUID-VkImageCreateInfo-extent-00946 ::
extent.depth must be greater than 0
)";
    return vr;
  }
  return vr;
}
Result_Vk check_mipLevelArray(const ImageCreateInfo &im) {
  //
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (im.mipLevels <= 0) {
    vr.status = IMAGE_CREATE_ERROR_VK;
    vr.spec_info = R"(
VUID-VkImageCreateInfo-mipLevels-00947 ::
mipLevels must be greater than 0
)";
    return vr;
  }
  if (im.arrayLayers <= 0) {
    vr.status = IMAGE_CREATE_ERROR_VK;
    vr.spec_info = R"(
VUID-VkImageCreateInfo-arrayLayers-00948
arrayLayers must be greater than 0
)";
    return vr;
  }
  return vr;
}
Result_Vk check_flags(const ImageCreateInfo &im) {
  //
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (im.img_flags.find(VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) !=
      im.img_flags.end()) {
    if (im.imageType != VK_IMAGE_TYPE_2D) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-flags-00949
If flags contains VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, imageType must be
VK_IMAGE_TYPE_2D
)";
      return vr;
    } else if (!(im.extent.width == im.extent.height && im.arrayLayers >= 6)) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-imageType-00954 ::
If imageType is VK_IMAGE_TYPE_2D and flags contains
VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, extent.width and extent.height must be
equal and arrayLayers must be greater than or equal to 6
)";
      return vr;
    }
  }
  if (im.img_flags.find(VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT) !=
      im.img_flags.end()) {
    if (im.imageType != VK_IMAGE_TYPE_3D) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-flags-00950 ::
If flags contains VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT, imageType must be
VK_IMAGE_TYPE_3D
)";
      return vr;
    }
  }
  if (im.img_flags.find(VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT) !=
      im.img_flags.end()) {
    if (im.imageType != VK_IMAGE_TYPE_2D) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-flags-02557 ::
If flags contains VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT, imageType must
be VK_IMAGE_TYPE_2D
)";
      return vr;
    }
  }
  return vr;
}
Result_Vk check_imageType(const ImageCreateInfo &im) {
  //
  Result_Vk vr;
  vr.status = SUCCESS_OP;
  if (im.imageType == VK_IMAGE_TYPE_1D) {
    if (im.extent.height != 1 || im.extent.depth != 1) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-imageType-00956 ::
If imageType is VK_IMAGE_TYPE_1D, both extent.height and extent.depth must be
1
)";
      return vr;
    }
  }
  if (im.imageType == VK_IMAGE_TYPE_1D) {
    if (im.extent.height != 1 || im.extent.depth != 1) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-imageType-00956 ::
If imageType is VK_IMAGE_TYPE_1D, both extent.height and extent.depth must be
1
)";
      return vr;
    }
  }
  if (im.imageType == VK_IMAGE_TYPE_2D) {
    if (im.extent.depth != 1) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-imageType-00957 ::
If imageType is VK_IMAGE_TYPE_2D, extent.depth must be 1
)";
      return vr;
    }
  }
  if (im.imageType == VK_IMAGE_TYPE_3D) {
    if (im.arrayLayers != 1) {
      vr.status = IMAGE_CREATE_ERROR_VK;
      vr.spec_info = R"(
VUID-VkImageCreateInfo-imageType-00961 ::
If imageType is VK_IMAGE_TYPE_3D, arrayLayers must be 1
)";
      return vr;
    }
}

/**
Explicit Valid Usage:

- VUID-VkImageCreateInfo-imageCreateMaxMipLevels-02251
  - Each of the following values (as described in Image Creation Limits) must
  not be undefined : imageCreateMaxMipLevels, imageCreateMaxArrayLayers,
  imageCreateMaxExtent, and imageCreateSampleCounts

- VUID-VkImageCreateInfo-sharingMode-00941
  - If sharingMode is VK_SHARING_MODE_CONCURRENT, pQueueFamilyIndices must be
  a valid pointer to an array of queueFamilyIndexCount uint32_t values

- VUID-VkImageCreateInfo-sharingMode-00942
  - If sharingMode is VK_SHARING_MODE_CONCURRENT, queueFamilyIndexCount must
  be greater than 1

- VUID-VkImageCreateInfo-sharingMode-01420
  - If sharingMode is VK_SHARING_MODE_CONCURRENT, each element of
  pQueueFamilyIndices must be unique and must be less than
  pQueueFamilyPropertyCount returned by either
  vkGetPhysicalDeviceQueueFamilyProperties or
  vkGetPhysicalDeviceQueueFamilyProperties2 for the physicalDevice that was
  used to create device

- VUID-VkImageCreateInfo-pNext-01974
  - If the pNext chain includes a VkExternalFormatANDROID structure, and its
  externalFormat member is non-zero the format must be VK_FORMAT_UNDEFINED

- VUID-VkImageCreateInfo-pNext-01975
  - If the pNext chain does not include a VkExternalFormatANDROID structure,
  or does and its externalFormat member is 0, the format must not be
  VK_FORMAT_UNDEFINED

- VUID-VkImageCreateInfo-extent-00944
  - extent.width must be greater than 0

- VUID-VkImageCreateInfo-extent-00945
  - extent.height must be greater than 0

- VUID-VkImageCreateInfo-extent-00946
  - extent.depth must be greater than 0

- VUID-VkImageCreateInfo-mipLevels-00947
  - mipLevels must be greater than 0

- VUID-VkImageCreateInfo-arrayLayers-00948
  - arrayLayers must be greater than 0

- VUID-VkImageCreateInfo-flags-00949
  - If flags contains VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, imageType must be
  VK_IMAGE_TYPE_2D

- VUID-VkImageCreateInfo-flags-02557
  - If flags contains VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT, imageType
  must be VK_IMAGE_TYPE_2D

- VUID-VkImageCreateInfo-flags-00950
  - If flags contains VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT, imageType must
  be VK_IMAGE_TYPE_3D

- VUID-VkImageCreateInfo-extent-02252
  - extent.width must be less than or equal to imageCreateMaxExtent.width (as
  defined in Image Creation Limits)

- VUID-VkImageCreateInfo-extent-02253
  - extent.height must be less than or equal to imageCreateMaxExtent.height
  (as defined in Image Creation Limits)

- VUID-VkImageCreateInfo-extent-02254
  - extent.depth must be less than or equal to imageCreateMaxExtent.depth (as
  defined in Image Creation Limits)

- VUID-VkImageCreateInfo-imageType-00954
  - If imageType is VK_IMAGE_TYPE_2D and flags contains
  VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, extent.width and extent.height must be
  equal and arrayLayers must be greater than or equal to 6

- VUID-VkImageCreateInfo-imageType-00956
  - If imageType is VK_IMAGE_TYPE_1D, both extent.height and extent.depth must
  be 1

- VUID-VkImageCreateInfo-imageType-00957
  - If imageType is VK_IMAGE_TYPE_2D, extent.depth must be 1

- VUID-VkImageCreateInfo-mipLevels-00958
  - mipLevels must be less than or equal to the number of levels in the
  complete mipmap chain based on extent.width, extent.height, and extent.depth

- VUID-VkImageCreateInfo-mipLevels-02255
  - mipLevels must be less than or equal to imageCreateMaxMipLevels (as
    defined in Image Creation Limits)

- VUID-VkImageCreateInfo-arrayLayers-02256
  - arrayLayers must be less than or equal to imageCreateMaxArrayLayers (as
    defined in Image Creation Limits)

- VUID-VkImageCreateInfo-imageType-00961
  - If imageType is VK_IMAGE_TYPE_3D, arrayLayers must be 1

- VUID-VkImageCreateInfo-samples-02257
  - If samples is not VK_SAMPLE_COUNT_1_BIT, then imageType must be
  VK_IMAGE_TYPE_2D, flags must not contain
  VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, mipLevels must be equal to 1, and
  imageCreateMaybeLinear (as defined in Image Creation Limits) must be
  VK_FALSE,

- VUID-VkImageCreateInfo-samples-02558
  - If samples is not VK_SAMPLE_COUNT_1_BIT, usage must not contain
  VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT

- VUID-VkImageCreateInfo-usage-00963
  - If usage includes VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, then bits other
  than VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, and
  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT must not be set

- VUID-VkImageCreateInfo-usage-00964
  - If usage includes VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
  VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, or
  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, extent.width must be less than or equal
  to VkPhysicalDeviceLimits::maxFramebufferWidth

- VUID-VkImageCreateInfo-usage-00965
  - If usage includes VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
  VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, or
  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, extent.height must be less than or
  equal to VkPhysicalDeviceLimits::maxFramebufferHeight

- VUID-VkImageCreateInfo-usage-02559
  - If usage includes VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT,
  extent.width must be less than or equal to
  ⌈minFragmentDensityTexelSizewidth​maxFramebufferWidth​⌉

- VUID-VkImageCreateInfo-usage-02560
  - If usage includes VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT,
  extent.height must be less than or equal to
  ⌈minFragmentDensityTexelSizeheight​maxFramebufferHeight​⌉

- VUID-VkImageCreateInfo-usage-00966
  - If usage includes VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, usage must also
  contain at least one of VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, or
  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT

- VUID-VkImageCreateInfo-samples-02258
  - samples must be a bit value that is set in imageCreateSampleCounts (as
  defined in Image Creation Limits)

- VUID-VkImageCreateInfo-usage-00968
  - If the multisampled storage images feature is not enabled, and usage
  contains VK_IMAGE_USAGE_STORAGE_BIT, samples must be VK_SAMPLE_COUNT_1_BIT

- VUID-VkImageCreateInfo-flags-00969
  - If the sparse bindings feature is not enabled, flags must not contain
  VK_IMAGE_CREATE_SPARSE_BINDING_BIT

- VUID-VkImageCreateInfo-flags-01924
  - If the sparse aliased residency feature is not enabled, flags must not
  contain VK_IMAGE_CREATE_SPARSE_ALIASED_BIT

- VUID-VkImageCreateInfo-tiling-04121
  - If tiling is VK_IMAGE_TILING_LINEAR, flags must not contain
  VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT

- VUID-VkImageCreateInfo-imageType-00970
  - If imageType is VK_IMAGE_TYPE_1D, flags must not contain
  VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT

- VUID-VkImageCreateInfo-imageType-00971
  - If the sparse residency for 2D images feature is not enabled, and
  imageType is VK_IMAGE_TYPE_2D, flags must not contain
  VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT

- VUID-VkImageCreateInfo-imageType-00972
  - If the sparse residency for 3D images feature is not enabled, and
  imageType is VK_IMAGE_TYPE_3D, flags must not contain
  VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT

- VUID-VkImageCreateInfo-imageType-00973
  - If the sparse residency for images with 2 samples feature is not enabled,
  imageType is VK_IMAGE_TYPE_2D, and samples is VK_SAMPLE_COUNT_2_BIT, flags
  must not contain VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT

- VUID-VkImageCreateInfo-imageType-00974
  - If the sparse residency for images with 4 samples feature is not enabled,
  imageType is VK_IMAGE_TYPE_2D, and samples is VK_SAMPLE_COUNT_4_BIT, flags
  must not contain VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT

- VUID-VkImageCreateInfo-imageType-00975
  - If the sparse residency for images with 8 samples feature is not enabled,
  imageType is VK_IMAGE_TYPE_2D, and samples is VK_SAMPLE_COUNT_8_BIT, flags
  must not contain VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT

- VUID-VkImageCreateInfo-imageType-00976
  - If the sparse residency for images with 16 samples feature is not enabled,
  imageType is VK_IMAGE_TYPE_2D, and samples is VK_SAMPLE_COUNT_16_BIT, flags
  must not contain VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT

- VUID-VkImageCreateInfo-flags-00987
  - If flags contains VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT or
  VK_IMAGE_CREATE_SPARSE_ALIASED_BIT, it must also contain
  VK_IMAGE_CREATE_SPARSE_BINDING_BIT

- VUID-VkImageCreateInfo-None-01925
  - If any of the bits VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
  VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT, or VK_IMAGE_CREATE_SPARSE_ALIASED_BIT
  are set, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT must not also be set

- VUID-VkImageCreateInfo-flags-01890
  - If the protected memory feature is not enabled, flags must not contain
  VK_IMAGE_CREATE_PROTECTED_BIT

- VUID-VkImageCreateInfo-None-01891
  - If any of the bits VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
  VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT, or VK_IMAGE_CREATE_SPARSE_ALIASED_BIT
  are set, VK_IMAGE_CREATE_PROTECTED_BIT must not also be set

- VUID-VkImageCreateInfo-pNext-00988
  - If the pNext chain includes a VkExternalMemoryImageCreateInfoNV structure,
  it must not contain a VkExternalMemoryImageCreateInfo structure

- VUID-VkImageCreateInfo-pNext-00990
  - If the pNext chain includes a VkExternalMemoryImageCreateInfo structure,
  its handleTypes member must only contain bits that are also in
  VkExternalImageFormatProperties::externalMemoryProperties.compatibleHandleTypes,
  as returned by vkGetPhysicalDeviceImageFormatProperties2 with format,
  imageType, tiling, usage, and flags equal to those in this structure, and
  with a VkPhysicalDeviceExternalImageFormatInfo structure included in the
  pNext chain, with a handleType equal to any one of the handle types
  specified in VkExternalMemoryImageCreateInfo::handleTypes

- VUID-VkImageCreateInfo-pNext-00991
  - If the pNext chain includes a VkExternalMemoryImageCreateInfoNV structure,
  its handleTypes member must only contain bits that are also in
  VkExternalImageFormatPropertiesNV::externalMemoryProperties.compatibleHandleTypes,
  as returned by vkGetPhysicalDeviceExternalImageFormatPropertiesNV with
  format, imageType, tiling, usage, and flags equal to those in this
  structure, and with externalHandleType equal to any one of the handle types
  specified in VkExternalMemoryImageCreateInfoNV::handleTypes

- VUID-VkImageCreateInfo-physicalDeviceCount-01421
  - If the logical device was created with
  VkDeviceGroupDeviceCreateInfo::physicalDeviceCount equal to 1, flags must
  not contain VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT

- VUID-VkImageCreateInfo-flags-02259
  - If flags contains VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT, then
  mipLevels must be one, arrayLayers must be one, imageType must be
  VK_IMAGE_TYPE_2D. and imageCreateMaybeLinear (as defined in Image Creation
  Limits) must be VK_FALSE

- VUID-VkImageCreateInfo-flags-01572
  - If flags contains VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT, then
  format must be a block-compressed image format, an ETC compressed image
  format, or an ASTC compressed image format

- VUID-VkImageCreateInfo-flags-01573
  - If flags contains VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT, then
  flags must also contain VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT

- VUID-VkImageCreateInfo-initialLayout-00993
  - initialLayout must be VK_IMAGE_LAYOUT_UNDEFINED or
  VK_IMAGE_LAYOUT_PREINITIALIZED

- VUID-VkImageCreateInfo-pNext-01443
  - If the pNext chain includes a VkExternalMemoryImageCreateInfo or
  VkExternalMemoryImageCreateInfoNV structure whose handleTypes member is not
  0, initialLayout must be VK_IMAGE_LAYOUT_UNDEFINED

- VUID-VkImageCreateInfo-format-02561
  - If the image format is one of those listed in Formats requiring sampler
  Y′CBCR conversion for VK_IMAGE_ASPECT_COLOR_BIT image views, then mipLevels
  must be 1

- VUID-VkImageCreateInfo-format-02562
  - If the image format is one of those listed in Formats requiring sampler
  Y′CBCR conversion for VK_IMAGE_ASPECT_COLOR_BIT image views, samples must be
  VK_SAMPLE_COUNT_1_BIT

- VUID-VkImageCreateInfo-format-02563
  - If the image format is one of those listed in Formats requiring sampler
  Y′CBCR conversion for VK_IMAGE_ASPECT_COLOR_BIT image views, imageType must
  be VK_IMAGE_TYPE_2D

- VUID-VkImageCreateInfo-format-02653
  - If the image format is one of those listed in Formats requiring sampler
  Y′CBCR conversion for VK_IMAGE_ASPECT_COLOR_BIT image views, and the
  ycbcrImageArrays feature is not enabled, arrayLayers must be 1

- VUID-VkImageCreateInfo-imageCreateFormatFeatures-02260
  - If format is a multi-planar format, and if imageCreateFormatFeatures (as
  defined in Image Creation Limits) does not contain
  VK_FORMAT_FEATURE_DISJOINT_BIT, then flags must not contain
  VK_IMAGE_CREATE_DISJOINT_BIT

- VUID-VkImageCreateInfo-format-01577
  - If format is not a multi-planar format, and flags does not include
  VK_IMAGE_CREATE_ALIAS_BIT, flags must not contain
  VK_IMAGE_CREATE_DISJOINT_BIT

- VUID-VkImageCreateInfo-format-04712
  - If format has a _422 or _420 suffix, width must be a multiple of 2

- VUID-VkImageCreateInfo-format-04713
  - If format has a _420 suffix, height must be a multiple of 2

- VUID-VkImageCreateInfo-tiling-02261
  - If tiling is VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT, then the pNext chain
  must include exactly one of VkImageDrmFormatModifierListCreateInfoEXT or
  VkImageDrmFormatModifierExplicitCreateInfoEXT structures

- VUID-VkImageCreateInfo-pNext-02262
  - If the pNext chain includes a VkImageDrmFormatModifierListCreateInfoEXT or
  VkImageDrmFormatModifierExplicitCreateInfoEXT structure, then tiling must be
  VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT

- VUID-VkImageCreateInfo-tiling-02353
  - If tiling is VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT and flags contains
  VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT, then the pNext chain must include a
  VkImageFormatListCreateInfo structure with non-zero viewFormatCount

- VUID-VkImageCreateInfo-flags-01533
  - If flags contains VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT
  format must be a depth or depth/stencil format

- VUID-VkImageCreateInfo-pNext-02393
  - If the pNext chain includes a VkExternalMemoryImageCreateInfo structure
  whose handleTypes member includes
  VK_EXTERNAL_MEMORY_HANDLE_TYPE_ANDROID_HARDWARE_BUFFER_BIT_ANDROID,
  imageType must be VK_IMAGE_TYPE_2D

- VUID-VkImageCreateInfo-pNext-02394
  - If the pNext chain includes a VkExternalMemoryImageCreateInfo structure
  whose handleTypes member includes
  VK_EXTERNAL_MEMORY_HANDLE_TYPE_ANDROID_HARDWARE_BUFFER_BIT_ANDROID,
  mipLevels must either be 1 or equal to the number of levels in the complete
  mipmap chain based on extent.width, extent.height, and extent.depth

- VUID-VkImageCreateInfo-pNext-02396
  - If the pNext chain includes a VkExternalFormatANDROID structure whose
  externalFormat member is not 0, flags must not include
  VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT

- VUID-VkImageCreateInfo-pNext-02397
  - If the pNext chain includes a VkExternalFormatANDROID structure whose
  externalFormat member is not 0, usage must not include any usages except
  VK_IMAGE_USAGE_SAMPLED_BIT

- VUID-VkImageCreateInfo-pNext-02398
  - If the pNext chain includes a VkExternalFormatANDROID structure whose
  externalFormat member is not 0, tiling must be VK_IMAGE_TILING_OPTIMAL

- VUID-VkImageCreateInfo-format-02795
  - If format is a depth-stencil format, usage includes
  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, and the pNext chain includes a
  VkImageStencilUsageCreateInfo structure, then its
  VkImageStencilUsageCreateInfo::stencilUsage member must also include
  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT

- VUID-VkImageCreateInfo-format-02796
  - If format is a depth-stencil format, usage does not include
  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, and the pNext chain includes a
  VkImageStencilUsageCreateInfo structure, then its
  VkImageStencilUsageCreateInfo::stencilUsage member must also not include
  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT

- VUID-VkImageCreateInfo-format-02797
  - If format is a depth-stencil format, usage includes
  VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, and the pNext chain includes a
  VkImageStencilUsageCreateInfo structure, then its
  VkImageStencilUsageCreateInfo::stencilUsage member must also include
  VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT

- VUID-VkImageCreateInfo-format-02798
  - If format is a depth-stencil format, usage does not include
  VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, and the pNext chain includes a
  VkImageStencilUsageCreateInfo structure, then its
  VkImageStencilUsageCreateInfo::stencilUsage member must also not include
  VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT

- VUID-VkImageCreateInfo-Format-02536
  - If Format is a depth-stencil format and the pNext chain includes a
  VkImageStencilUsageCreateInfo structure with its stencilUsage member
  including VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, extent.width must be less
  than or equal to VkPhysicalDeviceLimits::maxFramebufferWidth

- VUID-VkImageCreateInfo-format-02537
  - If format is a depth-stencil format and the pNext chain includes a
  VkImageStencilUsageCreateInfo structure with its stencilUsage member
  including VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, extent.height must be less
  than or equal to VkPhysicalDeviceLimits::maxFramebufferHeight

- VUID-VkImageCreateInfo-format-02538
  - If the multisampled storage images feature is not enabled, format is a
  depth-stencil format and the pNext chain includes a
  VkImageStencilUsageCreateInfo structure with its stencilUsage including
  VK_IMAGE_USAGE_STORAGE_BIT, samples must be VK_SAMPLE_COUNT_1_BIT

- VUID-VkImageCreateInfo-flags-02050
  - If flags contains VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV, imageType must be
  VK_IMAGE_TYPE_2D or VK_IMAGE_TYPE_3D

- VUID-VkImageCreateInfo-flags-02051
  - If flags contains VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV, it must not
  contain VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT and the format must not be a
  depth/stencil format

- VUID-VkImageCreateInfo-flags-02052
  - If flags contains VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV and imageType is
  VK_IMAGE_TYPE_2D, extent.width and extent.height must be greater than 1

- VUID-VkImageCreateInfo-flags-02053
  - If flags contains VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV and imageType is
  VK_IMAGE_TYPE_3D, extent.width, extent.height, and extent.depth must be
  greater than 1

- VUID-VkImageCreateInfo-imageType-02082
  - If usage includes VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
  imageType must be VK_IMAGE_TYPE_2D

- VUID-VkImageCreateInfo-samples-02083
  - If usage includes VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
  samples must be VK_SAMPLE_COUNT_1_BIT

- VUID-VkImageCreateInfo-tiling-02084
  - If usage includes VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV, tiling must be
  VK_IMAGE_TILING_OPTIMAL

- VUID-VkImageCreateInfo-flags-02565
  - If flags contains VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT, tiling must be
  VK_IMAGE_TILING_OPTIMAL

- VUID-VkImageCreateInfo-flags-02566
  - If flags contains VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT, imageType must be
  VK_IMAGE_TYPE_2D

- VUID-VkImageCreateInfo-flags-02567
  - If flags contains VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT, flags must not
  contain VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT

- VUID-VkImageCreateInfo-flags-02568
  - If flags contains VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT, mipLevels must be 1

- VUID-VkImageCreateInfo-usage-04992
  - If usage includes VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI, tiling must
  be VK_IMAGE_TILING_LINEAR

- VUID-VkImageCreateInfo-imageView2DOn3DImage-04459
  - If the VK_KHR_portability_subset extension is enabled, and
  VkPhysicalDevicePortabilitySubsetFeaturesKHR::imageView2DOn3DImage is
  VK_FALSE, flags must not contain VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT

- VUID-VkImageCreateInfo-multisampleArrayImage-04460
  - If the VK_KHR_portability_subset extension is enabled, and
  VkPhysicalDevicePortabilitySubsetFeaturesKHR::multisampleArrayImage is
  VK_FALSE, and samples is not VK_SAMPLE_COUNT_1_BIT, then arrayLayers must be
  1

- VUID-VkImageCreateInfo-pNext-04737
  - If a VkImageFormatListCreateInfo structure was included in the pNext chain
  and VkImageFormatListCreateInfo::viewFormatCount is not zero, then all of
  the formats in VkImageFormatListCreateInfo::pViewFormats must be compatible
  with the format as described in the compatibility table

- VUID-VkImageCreateInfo-flags-04738
  - If flags does not contain VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT and the pNext
  chain includes a VkImageFormatListCreateInfo structure, then
  VkImageFormatListCreateInfo::viewFormatCount must be 0 or 1

- VUID-VkImageCreateInfo-usage-04815
  - If usage includes VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR,
  VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR,
  VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR, then the pNext chain must include a
  valid VkVideoProfilesKHR structure which includes at least one
  VkVideoProfileKHR with a decode codec-operation

- VUID-VkImageCreateInfo-usage-04816
  - If usage includes VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR,
  VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR,
  VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR, then the pNext chain must include a
  valid VkVideoProfilesKHR structure which includes at least one
  VkVideoProfileKHR with a encode codec-operation

Implicit Valid Usage:

- VUID-VkImageCreateInfo-sType-sType
  - sType must be VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO

- VUID-VkImageCreateInfo-pNext-pNext
  - Each pNext member of any structure (including this one) in the pNext chain
  must be either NULL or a pointer to a valid instance of
  VkDedicatedAllocationImageCreateInfoNV, VkExternalFormatANDROID,
  VkExternalMemoryImageCreateInfo, VkExternalMemoryImageCreateInfoNV,
  VkImageDrmFormatModifierExplicitCreateInfoEXT,
  VkImageDrmFormatModifierListCreateInfoEXT, VkImageFormatListCreateInfo,
  VkImageStencilUsageCreateInfo, VkImageSwapchainCreateInfoKHR,
  VkVideoProfileKHR, or VkVideoProfilesKHR

- VUID-VkImageCreateInfo-sType-unique
  - The sType value of each struct in the pNext chain must be unique

- VUID-VkImageCreateInfo-flags-parameter
  - flags must be a valid combination of VkImageCreateFlagBits values

- VUID-VkImageCreateInfo-imageType-parameter
  - imageType must be a valid VkImageType value

- VUID-VkImageCreateInfo-format-parameter
  - format must be a valid VkFormat value

- VUID-VkImageCreateInfo-samples-parameter
  - samples must be a valid VkSampleCountFlagBits value

- VUID-VkImageCreateInfo-tiling-parameter
  - tiling must be a valid VkImageTiling value

- VUID-VkImageCreateInfo-usage-parameter
  - usage must be a valid combination of VkImageUsageFlagBits values

- VUID-VkImageCreateInfo-usage-requiredbitmask
  - usage must not be 0

- VUID-VkImageCreateInfo-sharingMode-parameter
  - sharingMode must be a valid VkSharingMode value

- VUID-VkImageCreateInfo-initialLayout-parameter
  - initialLayout must be a valid VkImageLayout value

*/
template <> struct StructChecker<ImageCreateInfo> {
  Result_Vk check(const ImageCreateInfo &ici) {
    //
    Result_Vk vr;
    vr.status = SUCCESS_OP;
  }
};

} // namespace vtuto
