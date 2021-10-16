#include <external.hpp>
#include <vkapp/vktriapp.hpp>
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vkgraphmaker.hpp>
#include <vkgraph/vknode.hpp>
#include <vkresult/debug.hpp>

using namespace vtuto;

int main() {
  // declare graph
  vk_graph<vk_triapp> graph;
  auto fnmap = vk_triAppFns();

  /** init window node:
    - node id 1
    - target nodes: {2}
   */
  // declare node
  {
    const unsigned int signal = 1;
    const unsigned int node_id = 2;
    auto vr = mkAddNode<vk_triapp, 1, true, signal, node_id>(
        graph, fnmap["initWindow"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** init vulkan node:
   It can be implemented either as a single node or a chain of sub nodes.
   I implement it as a sequence of nodes whose execution order is fixed by its
   edges. The node creation can be delegated to other encapsulating functions
   for facilitating reading
   */
  /** vulkan instance creation node:
    - node id 2
    - target nodes: {3}
   */
  {
    auto vr =
        mkAddNode<vk_triapp, 2, true, 1, 3>(graph, fnmap["createInstance"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 2 with end node 3";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan setup debug messenger node:
    - node id 3
    - target nodes: {4: createSurface, }
   */
  {
    auto vr = mkAddNode<vk_triapp, 3, true, 1, 4>(graph,
                                                  fnmap["setupDebugMessenger"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 3 with end node 4";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan create surface node:
    - node id 4
    - target nodes: {5: pickPhysicalDevice}
   */
  {
    auto vr =
        mkAddNode<vk_triapp, 4, true, 1, 5>(graph, fnmap["createSurface"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 4 with end node 5";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan pick physical device node:
    - node id 5
    - target nodes: {6: createLogicalDevice}
   */
  {
    auto vr =
        mkAddNode<vk_triapp, 5, true, 1, 6>(graph, fnmap["pickPhysicalDevice"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 5 with end node 6";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan create logical device node:
    - node id 6
    - target nodes: {7: createSwapChain}
   */
  {
    auto vr = mkAddNode<vk_triapp, 6, true, 1, 7>(graph,
                                                  fnmap["createLogicalDevice"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 6 with end node 7";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan create swap chain node:
    - node id 7
    - target nodes: {8: createImageViews}
   */
  {
    auto vr =
        mkAddNode<vk_triapp, 7, false, 1, 8>(graph, fnmap["createSwapChain"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 7 with end node 8";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create swap chain image views node:
    - node id 8
    - target nodes {9: createRenderPass}
   */
  {
    auto vr =
        mkAddNode<vk_triapp, 8, false, 1, 9>(graph, fnmap["createImageViews"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 8 with end node 9";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create render pass for all the images node:
    - node id 9
    - target nodes {10: createGraphicsPipeline}
   */
  {
    auto vr =
        mkAddNode<vk_triapp, 9, false, 1, 10>(graph, fnmap["createRenderPass"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 9 with end node 10";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create graphics pipeline node:
    - node id 10
    - target nodes {11: createFramebuffers}

    It accesses IO. It can probably be refactored to access it only one time.
   */
  {
    auto vr = mkAddNode<vk_triapp, 10, false, 1, 11>(
        graph, fnmap["createGraphicsPipeline"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 10 with end node 11";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** create framebuffers node:
    depends on:
    - swap chain image views
    - swap chain extent
    - swap chain render pass

    - node id 11
    - target node {12: createCommandPool}
   */
  {
    auto vr = mkAddNode<vk_triapp, 11, false, 1, 12>(
        graph, fnmap["createFramebuffers"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 11 with end node 12";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** create command pool node:
    depends on:
    - logical device
    - surface
    - physical device

    - node id 12
    - target node {13: createCommandBufferAlloc}
   */
  {
    auto vr = mkAddNode<vk_triapp, 12, false, 1, 13>(
        graph, fnmap["createCommandPool"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 12 with end node 13";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** create command buffer allocation node:

    - node id 13
    - target node {14: createCommandBuffers}
   */
  {
    auto vr = mkAddNode<vk_triapp, 13, false, 1, 14>(
        graph, fnmap["createCommandBufferAlloc"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 12 with end node 13";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  // a merging buffer which does not do anything
  {
      // add a merging node if need be
  }

  /** create command buffer node:
    depends on:
    - command allocation buffers node
    - swap chain framebuffers
    - render pass
    - graphics pipeline
    - swapchain extent

    - node id 14
    - target node id {15: createSyncObjects}
   */
  {
    auto vr = mkAddNode<vk_triapp, 14, false, 1, 15>(
        graph, fnmap["createCommandBuffers"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 14 with end node 15";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createSyncObjects node:
    - node id 15
    - target node id {16: windowShouldClose}
   */
  {
    auto vr = mkAddNode<vk_triapp, 15, false, 1, 16>(
        graph, fnmap["createSyncObjects"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 14 with end node 15";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /**
    windowShouldClose node:

    - node id 16
    - target node ids {17: drawFrame, 19: cleanUp}
   */
  {
    auto vr = mkAddNode<vk_triapp, 16, false, 1, 17, 2, 18>(
        graph, fnmap["windowShouldClose"]);
    if (vr.status != SUCCESS_OP) {
      std::string str =
          "node creation failed for node 15 with end node {16, 17}";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /**
    draw frame node:
    - node id 17
    - target node {15: windowShouldClose, 18: recreateSwapChain}
   */
  {
    auto vr = mkAddNode<vk_triapp, 17, false, 1, 15, 2, 18>(graph,
                                                            fnmap["drawFrame"]);
    if (vr.status != SUCCESS_OP) {
      std::string str =
          "node creation failed for node 15 with end node {16, 17}";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    recreate swap chain node:
    - node id 18
    - target node {19: cleanupSwapChain}
  */
  {}

  /**
    clean up swapchain in recreate swapchain node:
    - node id 20
    - target node id {21: createSwapChain}
   */
  {} /**
       createSwapChain in recreate swapchain node:
       - node id 21
       - target node id {22: createImageViews}
      */
  {} /**
       createImageViews in recreate swapchain node:
       - node id 22
       - target node id {23: createRenderPass}
      */
  {} /**
       createRenderPass in recreate swapchain node:
       - node id 23
       - target node id {24: createGraphicsPipeline}
      */
  {}

  /**
    createGraphicsPipeline in recreate swapchain node:
    - node id 23
    - target node id {24: createFramebuffers}
   */
  {}

  /**
    createFramebuffers in recreate swapchain node:
    - node id 24
    - target node id {25: createCommandBuffers}
   */
  {}

  /**
    createCommandBuffers in recreate swapchain node:
    - node id 25
    - target node id {26: imagesInFlightResize}
   */
  {} /**
       imagesInFlightResize in recreateSwapChain node:
       - node id 26
       - target node {16: windowShouldClose}
      */
  {} // recreate swapchain ends, back to window should close now it
     // is time for cleanup
  /**
    cleanup node:
    - node id: 27
    - target node {28: cleanupSwapChain}
   */
  /**
    cleanupSwapChain in cleanup node:
    - node id: 28
    - target node {29: destroyAll}
   */
  {} /**
       destroyAll in cleanup node:
       - node id: 29
       - target node: END_NODE

       Once runs this node terminates the application
      */
  {}

  // run first edge

  std::cout << "everything runs" << std::endl;

  // start declaring nodes
  return 0;
}
