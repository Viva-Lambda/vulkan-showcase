#pragma once
// external libraries
#include <vulkan/vulkan.hpp>
//
#include <GLFW/glfw3.h>
//
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//

// stb image
#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb_image.h>
//
// tiny obj loader
#define TINYOBJLOADER_IMPLEMENTATION
#include <thirdparty/tiny_obj_loader.h>
//

//
#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
