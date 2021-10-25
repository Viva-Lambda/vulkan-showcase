#include <external.hpp>
#include <vkapp/vktriapp.hpp>
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vkgraphmaker.hpp>
#include <vkgraph/vknode.hpp>
#include <vkresult/debug.hpp>

using namespace vtuto;

int main() {
  // declare graph
  auto fnmap = vk_triAppFns();
  vk_triapp triangle;
  vk_graph2<vk_triapp> ngraph;

  /** init window node:
    - node id 1,
    - node label: initWindowNode
    - target node ids: {2}
    - target node label: {"createInstanceNode"}
   */
  {
    const_str nlabel = const_str("initWindowNode");
    const NodeIdVk node_id = 1;
    std::function<vk_output(vk_triapp &)> nf = fnmap["initWindow"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createInstanceNode"))};
    auto task = std::make_pair("initWindow", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, true>(ngraph, nlabel, task, neighbours);
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
    const_str nlabel = const_str("createInstanceNode");
    const NodeIdVk node_id = 2;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createInstance"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("setupDebugMessengerNode"))};
    auto task = std::make_pair("createInstance", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, true>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** vulkan setup debug messenger node:
    - node id 3
    - target nodes: {4: createSurfaceNode, }
   */
  {
    const_str nlabel = const_str("setupDebugMessengerNode");
    const NodeIdVk node_id = 3;
    std::function<vk_output(vk_triapp &)> nf = fnmap["setupDebugMessenger"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createSurfaceNode"))};
    auto task = std::make_pair("setupDebugMessenger", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, true>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createSurfaceNode");
    const NodeIdVk node_id = 4;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createSurface"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("pickPhysicalDeviceNode"))};
    auto task = std::make_pair("createSurface", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, true>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("pickPhysicalDeviceNode");
    const NodeIdVk node_id = 5;
    std::function<vk_output(vk_triapp &)> nf = fnmap["pickPhysicalDevice"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createLogicalDeviceNode"))};
    auto task = std::make_pair("pickPhysicalDevice", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, true>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createLogicalDeviceNode");
    const NodeIdVk node_id = 6;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createLogicalDevice"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createSwapChainNode"))};
    auto task = std::make_pair("createLogicalDevice", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, true>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createSwapChainNode");
    const NodeIdVk node_id = 7;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createSwapChain"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createImageViewsNode"))};
    auto task = std::make_pair("createSwapChain", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createImageViewsNode");
    const NodeIdVk node_id = 8;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createImageViews"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createRenderPassNode"))};
    auto task = std::make_pair("createImageViews", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createRenderPassNode");
    const NodeIdVk node_id = 9;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createRenderPass"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createGraphicsPipelineNode"))};
    auto task = std::make_pair("createRenderPass", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createGraphicsPipelineNode");
    const NodeIdVk node_id = 10;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createGraphicsPipeline"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createFramebuffersNode"))};
    auto task = std::make_pair("createGraphicsPipeline", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createFramebuffersNode");
    const NodeIdVk node_id = 11;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createFramebuffers"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createCommandPoolNode"))};
    auto task = std::make_pair("createFramebuffers", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createCommandPoolNode");
    const NodeIdVk node_id = 12;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createCommandPool"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createCommandBufferAllocNode"))};
    auto task = std::make_pair("createCommandPool", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create command buffer allocation node. This is reused in recreating swap
   * chain

    - node id 13
    - node label createCommandBufferAllocNode
    - target node {14: createCommandBuffers}
   */
  {
    const_str nlabel = const_str("createCommandBufferAllocNode");
    const NodeIdVk node_id = 13;
    std::function<vk_output(vk_triapp &)> nf =
        fnmap["createCommandBufferAlloc"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createCommandBuffersNode"))};
    auto task = std::make_pair("createCommandBufferAlloc", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
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
    const_str nlabel = const_str("createCommandBuffersNode");
    const NodeIdVk node_id = 14;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createCommandBuffers"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createSyncObjectsNode"))};
    auto task = std::make_pair("createCommandBuffers", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
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
    const_str nlabel = const_str("createSyncObjectsNode");
    const NodeIdVk node_id = 15;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createSyncObjects"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("windowShouldCloseNode"))};
    auto task = std::make_pair("createSyncObjects", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, true>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    windowShouldClose node:

    - node id 16
    - target node ids {17: drawFrame, 29: cleanUp}
   */
  {
    const_str nlabel = const_str("windowShouldCloseNode");
    const NodeIdVk node_id = 16;
    std::function<vk_output(vk_triapp &)> nf = fnmap["windowShouldClose"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("drawFrameNode")),
        std::make_pair(BranchType::UNCOND, const_str("cleanupSwapChainNode2"))};
    auto task = std::make_pair("windowShouldClose", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    draw frame node:
    - node id 17
    - target node {16: windowShouldClose, 18: recreateSwapChain}
   */
  {
    const_str nlabel = const_str("drawFrameNode");
    const NodeIdVk node_id = 17;
    std::function<vk_output(vk_triapp &)> nf = fnmap["drawFrame"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("windowShouldCloseNode")),
        std::make_pair(BranchType::UNCOND, const_str("recreateSwapChainNode"))};
    auto task = std::make_pair("drawFrame", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    recreate swap chain node:
    - node id 18
    - target node {20: cleanupSwapChain}
  */
  {
    const_str nlabel = const_str("recreateSwapChainNode");
    const NodeIdVk node_id = 18;
    std::function<vk_output(vk_triapp &)> nf = fnmap["recreateSwapChain"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("cleanupSwapChainNode"))};
    auto task = std::make_pair("recreateSwapChain", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    clean up swapchain in recreate swapchain node:
    - node id 20
    - target node id {21: createSwapChain}
   */
  {
    const_str nlabel = const_str("cleanupSwapChainNode");
    const NodeIdVk node_id = 20;
    std::function<vk_output(vk_triapp &)> nf = fnmap["cleanupSwapChain"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createSwapChainNode2"))};
    auto task = std::make_pair("cleanupSwapChain", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createSwapChain in recreate swapchain node:
    - node id 21
    - target node id {22: createImageViews}
   */
  {
    const_str nlabel = const_str("createSwapChainNode2");
    const NodeIdVk node_id = 21;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createSwapChain"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createImageViewsNode2"))};
    auto task = std::make_pair("createSwapChain", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createImageViews in recreate swapchain node:
    - node id 22
    - target node id {23: createRenderPass}
   */
  {
    const_str nlabel = const_str("createImageViewsNode2");
    const NodeIdVk node_id = 22;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createImageViews"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createRenderPassNode2"))};
    auto task = std::make_pair("createImageViews", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createRenderPass in recreate swapchain node:
    - node id 23
    - target node id {24: createGraphicsPipeline}
   */
  {
    const_str nlabel = const_str("createRenderPassNode2");
    const NodeIdVk node_id = 23;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createRenderPass"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createGraphicsPipelineNode2"))};
    auto task = std::make_pair("createRenderPass", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createGraphicsPipeline in recreate swapchain node:
    - node id 24
    - target node id {25: createFramebuffers}
   */
  {
    const_str nlabel = const_str("createGraphicsPipelineNode2");
    const NodeIdVk node_id = 24;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createGraphicsPipeline"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createFramebuffersNode2"))};
    auto task = std::make_pair("createGraphicsPipeline", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createFramebuffers in recreate swapchain node:
    - node id 25
    - target node id {26: createCommandBufferAlloc}
   */
  {
    const_str nlabel = const_str("createFramebuffersNode2");
    const NodeIdVk node_id = 25;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createFramebuffers"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createCommandBufferAllocNode2"))};
    auto task = std::make_pair("createFramebuffers", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
      createCommandBufferAlloc in recreate swapchain node:
      - node id 26
      - target node id {27: createCommandBuffers}
     */
  {
    const_str nlabel = const_str("createCommandBufferAllocNode2");
    const NodeIdVk node_id = 26;
    std::function<vk_output(vk_triapp &)> nf =
        fnmap["createCommandBufferAlloc"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("createCommandBuffersNode2"))};
    auto task = std::make_pair("createCommandBufferAlloc", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createCommandBuffers in recreate swapchain node:
    - node id 27
    - target node id {28: imagesInFlightResize}
   */
  {
    const_str nlabel = const_str("createCommandBuffersNode2");
    const NodeIdVk node_id = 27;
    std::function<vk_output(vk_triapp &)> nf = fnmap["createCommandBuffers"];
    const branch neighbours[] = {std::make_pair(
        BranchType::UNCOND, const_str("imagesInFlightResizeNode"))};
    auto task = std::make_pair("createCommandBuffers", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    imagesInFlightResize in recreateSwapChain node:
    - node id 28
    - target node {16: windowShouldClose}
   */
  {
    const_str nlabel = const_str("imagesInFlightResizeNode");
    const NodeIdVk node_id = 28;
    std::function<vk_output(vk_triapp &)> nf = fnmap["imagesInFlightResize"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("windowShouldCloseNode"))};
    auto task = std::make_pair("imagesInFlightResize", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  // recreate swapchain ends, back to window should close now it
  // is time for cleanup
  /**
    cleanup node start with cleanup swap chain:

    cleanupSwapChain in cleanup node:
    - node id: 29
    - target node {30: destroyAll}
   */
  {
    const_str nlabel = const_str("cleanupSwapChainNode2");
    const NodeIdVk node_id = 29;
    std::function<vk_output(vk_triapp &)> nf = fnmap["cleanupSwapChain"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("destroyAllNode"))};
    auto task = std::make_pair("cleanupSwapChain", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /**
    destroyAll in cleanup node:
    - node id: 30
    - target node: END_NODE

    Once runs this node terminates the application
  */
  {
    const_str nlabel = const_str("destroyAllNode");
    const NodeIdVk node_id = 30;
    std::function<vk_output(vk_triapp &)> nf = fnmap["destroyAll"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("END"))};
    auto task = std::make_pair("destroyAll", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  {
    const_str nlabel = const_str("END");
    const NodeIdVk node_id = 31;
    std::function<vk_output(vk_triapp &)> nf = [](vk_triapp &g) {
      vk_output out;
      out.signal = 0;
      return out;
    };
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("END"))};
    auto task = std::make_pair("end", nf);
    auto vr =
        mkAddNode2<vk_triapp, node_id, false>(ngraph, nlabel, task, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  // end node
  for (auto it = ngraph.nodes.begin(); it != ngraph.nodes.end(); it++) {
    //
    std::cout << it->first.obj() << std::endl;
  }

  // run first edge
  auto snode = const_str("initWindowNode");
  auto enode = const_str("END");
  auto vr = ngraph.run_from_to(triangle, snode, enode);

  std::cout << toString(vr) << std::endl;

  std::cout << "everything runs" << std::endl;

  // start declaring nodes
  return 0;
}
