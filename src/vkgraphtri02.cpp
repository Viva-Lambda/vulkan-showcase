#include <external.hpp>
#include <vkapp/vktriapp.hpp>
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vkgraphmaker.hpp>
#include <vkgraph/vknode.hpp>
#include <vkresult/debug.hpp>

using namespace vtuto;

int main() {
  // declare graph
  vk_triapp triangle;
  std::unordered_map<std::string, std::function<vk_output(vk_triapp &)>> fnmap =
      vk_triAppFns();
  vk_graph2<vk_triapp> ngraph;

  /** init window node:
    - node id 1,
    - node label: initWindowNode
    - target node ids: {2}
    - target node label: {"createInstanceNode"}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 1, true>(
        ngraph, fnmap,
        const_str("initWindowNode"),     // node label
        const_str("createInstanceNode"), // neighbour_label
        "initWindow"                     // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
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
    auto pr = mkAddNodeRuntime<vk_triapp, 2, true>(
        ngraph, fnmap,
        const_str("createInstanceNode"),      // node label
        const_str("setupDebugMessengerNode"), // neighbour_label
        "createInstance"                      // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** vulkan setup debug messenger node:
    - node id 3
    - target nodes: {4: createSurfaceNode, }
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 3, true>(
        ngraph, fnmap,
        const_str("setupDebugMessengerNode"), // node label
        const_str("createSurfaceNode"),       // neighbour_label
        "setupDebugMessenger"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** vulkan create surface node:
    - node id 4
    - target nodes: {5: pickPhysicalDevice}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 4, true>(
        ngraph, fnmap,
        const_str("createSurfaceNode"),      // node label
        const_str("pickPhysicalDeviceNode"), // neighbour_label
        "createSurface"                      // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** vulkan pick physical device node:
    - node id 5
    - target nodes: {6: createLogicalDevice}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 5, true>(
        ngraph, fnmap,
        const_str("pickPhysicalDeviceNode"),  // node label
        const_str("createLogicalDeviceNode"), // neighbour_label
        "pickPhysicalDevice"                  // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** vulkan create logical device node:
    - node id 6
    - target nodes: {7: createSwapChain}
   */

  {
    auto pr = mkAddNodeRuntime<vk_triapp, 6, true>(
        ngraph, fnmap,
        const_str("createLogicalDeviceNode"), // node label
        const_str("createSwapChainNode"),     // neighbour_label
        "createLogicalDevice"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /** vulkan create swap chain node:
    - node id 7
    - target nodes: {8: createImageViews}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 8, false>(
        ngraph, fnmap,
        const_str("createSwapChainNode"),  // node label
        const_str("createImageViewsNode"), // neighbour_label
        "createSwapChain"                  // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create swap chain image views node:
    - node id 8
    - target nodes {9: createRenderPass}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 8, false>(
        ngraph, fnmap,
        const_str("createImageViewsNode"), // node label
        const_str("createRenderPassNode"), // neighbour_label
        "createImageViews"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create render pass for all the images node:
    - node id 9
    - target nodes {10: createGraphicsPipeline}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 9, false>(
        ngraph, fnmap,
        const_str("createRenderPassNode"),       // node label
        const_str("createGraphicsPipelineNode"), // neighbour_label
        "createRenderPass"                       // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /** create graphics pipeline node:
    - node id 10
    - target nodes {11: createFramebuffers}

    It accesses IO. It can probably be refactored to access it only one time.
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 10, false>(
        ngraph, fnmap,
        const_str("createGraphicsPipelineNode"), // node label
        const_str("createFramebuffersNode"),     // neighbour_label
        "createGraphicsPipeline"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
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
    auto pr = mkAddNodeRuntime<vk_triapp, 11, false>(
        ngraph, fnmap,
        const_str("createFramebuffersNode"), // node label
        const_str("createCommandPoolNode"),  // neighbour_label
        "createFramebuffers"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
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
    auto pr = mkAddNodeRuntime<vk_triapp, 12, false>(
        ngraph, fnmap,
        const_str("createCommandPoolNode"),    // node label
        const_str("createCommandBufferAllocNode"), // neighbour_label
        "createCommandPool"                    // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
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
    auto pr = mkAddNodeRuntime<vk_triapp, 13, false>(
        ngraph, fnmap,
        const_str("createCommandBufferAllocNode"), // node label
        const_str("createCommandBuffersNode"),     // neighbour_label
        "createCommandBufferAlloc"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
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
    auto pr = mkAddNodeRuntime<vk_triapp, 14, false>(
        ngraph, fnmap,
        const_str("createCommandBuffersNode"), // node label
        const_str("createSyncObjectsNode"),    // neighbour_label
        "createCommandBuffers"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createSyncObjects node:
    - node id 15
    - target node id {16: windowShouldClose}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 15, false>(
        ngraph, fnmap,
        const_str("createSyncObjectsNode"), // node label
        const_str("windowShouldCloseNode"), // neighbour_label
        "createSyncObjects"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    windowShouldClose node:

    - node id 16
    - target node ids {17: drawFrame, 29: cleanUp}
   */
  {
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("drawFrameNode")),
        std::make_pair(BranchType::UNCOND, const_str("cleanupSwapChainNode2"))};
    auto pr = mkAddNodeRuntime<vk_triapp, 16, false>(
        ngraph, fnmap,
        const_str("windowShouldCloseNode"), // node label
        neighbours,                         // neighbour_label
        "windowShouldClose"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    draw frame node:
    - node id 17
    - target node {16: windowShouldClose, 18: recreateSwapChain}
   */
  {
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("windowShouldCloseNode")),
        std::make_pair(BranchType::UNCOND, const_str("recreateSwapChainNode"))};
    auto pr = mkAddNodeRuntime<vk_triapp, 17, false>(
        ngraph, fnmap,
        const_str("drawFrameNode"), // node label
        neighbours,                 // neighbour_label
        "drawFrame"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    recreate swap chain node:
    - node id 18
    - target node {20: cleanupSwapChain}
  */

  {
    auto pr = mkAddNodeRuntime<vk_triapp, 18, false>(
        ngraph, fnmap,
        const_str("recreateSwapChainNode"), // node label
        const_str("cleanupSwapChainNode"),  // neighbour_label
        "recreateSwapChain"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    clean up swapchain in recreate swapchain node:
    - node id 20
    - target node id {21: createSwapChain}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 20, false>(
        ngraph, fnmap,
        const_str("cleanupSwapChainNode"), // node label
        const_str("createSwapChainNode2"), // neighbour_label
        "cleanupSwapChainNode"             // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createSwapChain in recreate swapchain node:
    - node id 21
    - target node id {22: createImageViews}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 21, false>(
        ngraph, fnmap,
        const_str("createSwapChainNode2"),  // node label
        const_str("createImageViewsNode2"), // neighbour_label
        "createSwapChain"                   // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createImageViews in recreate swapchain node:
    - node id 22
    - target node id {23: createRenderPass}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 22, false>(
        ngraph, fnmap,
        const_str("createImageViewsNode2"), // node label
        const_str("createRenderPassNode2"), // neighbour_label
        "createImageViews"                  // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createRenderPass in recreate swapchain node:
    - node id 23
    - target node id {24: createGraphicsPipeline}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 23, false>(
        ngraph, fnmap,
        const_str("createRenderPassNode2"),       // node label
        const_str("createGraphicsPipelineNode2"), // neighbour_label
        "createRenderPass"                        // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /**
    createGraphicsPipeline in recreate swapchain node:
    - node id 24
    - target node id {25: createFramebuffers}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 24, false>(
        ngraph, fnmap,
        const_str("createGraphicsPipelineNode2"), // node label
        const_str("createFramebuffersNode2"),     // neighbour_label
        "createGraphicsPipeline"                  // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    createFramebuffers in recreate swapchain node:
    - node id 25
    - target node id {26: createCommandBufferAlloc}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 25, false>(
        ngraph, fnmap,
        const_str("createFramebuffersNode2"),       // node label
        const_str("createCommandBufferAllocNode2"), // neighbour_label
        "createFramebuffers"                        // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
      createCommandBufferAlloc in recreate swapchain node:
      - node id 26
      - target node id {27: createCommandBuffers}
     */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 26, false>(
        ngraph, fnmap,
        const_str("createCommandBufferAllocNode2"), // node label
        const_str("createCommandBuffersNode2"),     // neighbour_label
        "createCommandBufferAlloc"                  // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }

  /**
    createCommandBuffers in recreate swapchain node:
    - node id 27
    - target node id {28: imagesInFlightResize}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 27, false>(
        ngraph, fnmap,
        const_str("createCommandBuffersNode2"), // node label
        const_str("imagesInFlightResizeNode"),  // neighbour_label
        "createCommandBuffers"                  // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  /**
    imagesInFlightResize in recreateSwapChain node:
    - node id 28
    - target node {16: windowShouldClose}
   */
  {
    auto pr = mkAddNodeRuntime<vk_triapp, 28, false>(
        ngraph, fnmap,
        const_str("imagesInFlightResizeNode"), // node label
        const_str("windowShouldCloseNode"),    // neighbour_label
        "imagesInFlightResize"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
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
    auto pr = mkAddNodeRuntime<vk_triapp, 29, false>(
        ngraph, fnmap,
        const_str("cleanupSwapChainNode2"), // node label
        const_str("destroyAllNode"),        // neighbour_label
        "cleanupSwapChain"                  // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
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
    auto pr = mkAddNodeRuntime<vk_triapp, 30, false>(
        ngraph, fnmap,
        const_str("destroyAllNode"), // node label
        const_str("END"),            // neighbour_label
        "destroyAll"                 // task name
    );
    if (pr.first.status != SUCCESS_OP) {
      UPDATE_RESULT_VK(pr.first, pr.second);
      std::cerr << toString(pr.first) << std::endl;
      return EXIT_FAILURE;
    }
  }
  // repeat end node out of caution
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
