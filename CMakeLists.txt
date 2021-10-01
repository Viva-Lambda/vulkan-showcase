cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project("Vulkan Tutorial")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_COMPILER "/home/kaan/miniconda3/envs/dev-cpp/bin/clang++")

set(CMAKE_CXX_FLAGS "-std=c++17")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -ggdb -Wall -Wextra -o2 -pedantic")

set(AbsPathPrefix 
    "/home/kaan/GitProjects")

# executables
add_executable(
    vulkantuto.out 
    "src/vulkantuto.cpp"
)

include_directories("./include/")

# libs and linking etc

# vulkan
set(VulkanPath "${AbsPathPrefix}/vulkan-1.2/x86_64")
# find_package(Vulkan REQUIRED)
add_library(VulkanLib SHARED IMPORTED)
set_target_properties(VulkanLib PROPERTIES IMPORTED_LOCATION
    "${VulkanPath}/lib/libvulkan.so"
)
message("Vulkan Include Path: ${VulkanPath}/include/")
message("Vulkan Library Path: ${VulkanPath}/lib/libvulkan.so")
include_directories("${VulkanPath}/include/")

target_link_libraries(vulkantuto.out VulkanLib)

# glfw
# find_package(glfw3 REQUIRED)
add_library(glfwLib SHARED IMPORTED)
set_target_properties(glfwLib PROPERTIES IMPORTED_LOCATION
    "${AbsPathPrefix}/glfw-3.3.4/bin/lib/libglfw.so")
include_directories("${AbsPathPrefix}/glfw-3.3.4/bin/include/")
target_link_libraries(vulkantuto.out glfwLib)

# glm
# find_package(glm REQUIRED)
include_directories("${AbsPathPrefix}/glm/glm/")
# target_link_libraries(ex01.out ${glm_LIBRARY_DIRS})


add_library(
    AssimpLib 
    SHARED 
    IMPORTED
)
set_target_properties(
    AssimpLib PROPERTIES IMPORTED_LOCATION 
    "${AbsPathPrefix}/assimp-5.0.1/build/code/libassimp.so.5"
)
include_directories("${AbsPathPrefix}/assimp-5.0.1/build/include/")

target_link_libraries(vulkantuto.out AssimpLib)

# libs

install(TARGETS vulkantuto.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/")

