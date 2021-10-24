#include <external.hpp>
#include <vkapp/vktriapp.hpp>
#include <vkgraph/vkgraph.hpp>
#include <vkgraph/vkgraphmaker.hpp>
#include <vkgraph/vknode.hpp>
#include <vkresult/debug.hpp>

using namespace vtuto;

int main() {
  // declare graph
  vk_graph<vk_triapp, NodeIdVk> graph;
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
    const char nlabel[] = "initWindowNode";
    const NodeIdVk node_id = 1;
    std::function<vk_output(vk_triapp &)> nf = fnmap["initWindow"];
    const branch neighbours[] = {
        std::make_pair(BranchType::UNCOND, const_str("createInstanceNode"))
    };
    auto vr =
        mkAddNode2<vk_triapp, node_id, true>(ngraph, nlabel, nf, neighbours);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 1 with end node 2";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

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
    auto vr = mkAddNode<vk_triapp, 12, true, 1, 13>(graph,
                                                    fnmap["createCommandPool"]);
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
      std::string str = "node creation failed for node 13 with end node 14";
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
    auto vr = mkAddNode<vk_triapp, 15, true, 1, 16>(graph,
                                                    fnmap["createSyncObjects"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 15 with end node 16";
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
    auto vr = mkAddNode<vk_triapp, 16, false, 1, 17, 2, 29>(
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
    - target node {16: windowShouldClose, 18: recreateSwapChain}
   */
  {
    auto vr = mkAddNode<vk_triapp, 17, false, 1, 16, 2, 18>(graph,
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
    - target node {20: cleanupSwapChain}
  */
  {
    auto vr = mkAddNode<vk_triapp, 18, false, 1, 20>(
        graph, fnmap["recreateSwapChain"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 18 with end node {20}";
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
    auto vr = mkAddNode<vk_triapp, 20, false, 1, 21>(graph,
                                                     fnmap["cleanupSwapChain"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 20 with end node {21}";
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
    auto vr =
        mkAddNode<vk_triapp, 21, false, 1, 22>(graph, fnmap["createSwapChain"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 21 with end node {22}";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }

  } /**
       createImageViews in recreate swapchain node:
       - node id 22
       - target node id {23: createRenderPass}
      */
  {
    auto vr = mkAddNode<vk_triapp, 22, false, 1, 23>(graph,
                                                     fnmap["createImageViews"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 22 with end node {23}";
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
    auto vr = mkAddNode<vk_triapp, 23, false, 1, 24>(graph,
                                                     fnmap["createRenderPass"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 23 with end node {24}";
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
    auto vr = mkAddNode<vk_triapp, 24, false, 1, 25>(
        graph, fnmap["createGraphicsPipeline"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 24 with end node {25}";
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
    auto vr = mkAddNode<vk_triapp, 25, false, 1, 26>(
        graph, fnmap["createFramebuffers"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 25 with end node {26}";
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
    auto vr = mkAddNode<vk_triapp, 26, false, 1, 27>(
        graph, fnmap["createCommandBufferAlloc"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 26 with end node {27}";
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
    auto vr = mkAddNode<vk_triapp, 27, false, 1, 28>(
        graph, fnmap["createCommandBuffers"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 26 with end node {27}";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }

  } /**
       imagesInFlightResize in recreateSwapChain node:
       - node id 28
       - target node {16: windowShouldClose}
      */
  {
    auto vr = mkAddNode<vk_triapp, 28, false, 1, 16>(
        graph, fnmap["imagesInFlightResize"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 26 with end node {16}";
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
    auto vr = mkAddNode<vk_triapp, 29, false, 1, 30>(graph,
                                                     fnmap["cleanupSwapChain"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 28 with end node {29}";
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
    auto vr =
        mkAddNode<vk_triapp, 30, false, 1, 31>(graph, fnmap["destroyAll"]);
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 29 with end node {30}";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }
  {
    auto vr = mkAddNode<vk_triapp, 31, false, 1, 32>(graph, [](vk_triapp &myg) {
      Result_Vk vr;
      vr.status = FAIL_OP;
      vk_output out;
      out.result_info = vr;
      out.signal = 1;
      return out;
    });
    if (vr.status != SUCCESS_OP) {
      std::string str = "node creation failed for node 31 with end node {32}";
      UPDATE_RESULT_VK(vr, str);
      std::cerr << toString(vr) << std::endl;
      return EXIT_FAILURE;
    }
  }

  // run first edge
  auto vr = graph.run_from_to(triangle, 1, 31);

  std::cout << toString(vr) << std::endl;

  std::cout << "everything runs" << std::endl;

  // start declaring nodes
  return 0;
}
