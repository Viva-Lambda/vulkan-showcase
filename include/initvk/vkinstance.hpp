#pragma once
#include <external.hpp>
#include <initvk/vkapp.hpp>
#include <vkresult/debug.hpp>
#include <vkresult/paramchecker.hpp>

using namespace vtuto;

namespace vtuto {

/**

  VkInstanceCreateInfo from Vulkan 1.2 API docs:
struct VkInstanceCreateInfo {
    VkStructureType             sType;
    const void*                 pNext;
    VkInstanceCreateFlags       flags;
    const VkApplicationInfo*    pApplicationInfo;
    uint32_t                    enabledLayerCount;
    const char* const*          ppEnabledLayerNames;
    uint32_t                    enabledExtensionCount;
    const char* const*          ppEnabledExtensionNames;
};

- sType is the type of this structure.

- pNext is NULL or a pointer to a structure extending this
structure.

- flags is reserved for future use.

- pApplicationInfo is NULL or a pointer to a
VkApplicationInfo structure. If
not NULL, this information helps implementations recognize
behavior inherent
to classes of applications. VkApplicationInfo is defined in
detail below.

- enabledLayerCount is the number of global layers to
enable.

- ppEnabledLayerNames is a pointer to an array of
enabledLayerCount
null-terminated UTF-8 strings containing the names of layers
to enable for the
created instance. The layers are loaded in the order they
are listed in this
array, with the first array element being the closest to the
application, and
the last array element being the closest to the driver. See
the Layers section
for further details.

- enabledExtensionCount is the number of global extensions
to enable.

- ppEnabledExtensionNames is a pointer to an array of
enabledExtensionCount
null-terminated UTF-8 strings containing the names of
extensions to enable.
  Create a Vulkan Instance

  Creation has two steps:

  - Optional step: Application info

  - Required step: Instance info

  Application info contains regular
  information with respect to vulkan application.
  For example, name, version, whether it uses a
  specific engine etc. The information mentioned
  here can be used by the driver to optimize certain
  aspects of the runtime with respect to parameters
  entered here.

  The second info, that of instance, is gives information
  with respect to extensions, and validation layers
  we would like to use for the application instance.
 */
template <typename T = void>
VkInstanceCreateInfo
mkInstanceCreateInfo(const VkApplicationInfo &appInfo,
                     const std::vector<const char *> &extensions,
                     const std::vector<const char *> &validation_layers,
                     const T pnext = NULL) {
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  //

  createInfo.pApplicationInfo = &appInfo; /**VkApplicationInfo reference*/

  // 3. Request extensions from glfw in order to visualize
  // vulkan
  // application instance
  //
  /** pass number of enabled extensions*/
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  /** std::vector<const char *> containing extensions */
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validation_layers.size());
    createInfo.ppEnabledLayerNames = validation_layers.data();

    // 4. create debug messenger handler
    createInfo.pNext = &pnext;
  } else {
    //
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = &pnext;
  }
  return createInfo;
}

} // namespace vtuto
