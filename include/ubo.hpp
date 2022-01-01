#pragma once
// ubo buffer objec
#include <external.hpp>

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};
