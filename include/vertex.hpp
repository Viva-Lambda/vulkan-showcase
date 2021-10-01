#pragma once
// vertex object
#include <external.hpp>
#include <sstream>

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription
  getBindingDescription() {
    VkVertexInputBindingDescription description{};
    description.binding = 0;
    description.stride = sizeof(Vertex);
    description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return description;
  }
  static std::array<VkVertexInputAttributeDescription, 3>
  getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3>
        attributes{};

    // set position
    attributes[0].binding = 0;
    attributes[0].location = 0;
    attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributes[0].offset = offsetof(Vertex, pos);

    // color
    attributes[1].binding = 0;
    attributes[1].location = 1;
    attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributes[1].offset = offsetof(Vertex, color);

    // texture coordinates
    attributes[2].binding = 0;
    attributes[2].location = 2;
    attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributes[2].offset = offsetof(Vertex, texCoord);

    return attributes;
  }
  bool operator==(const Vertex &o) const {
    auto c1 = o.pos.x == pos.x && o.pos.y == pos.y &&
              o.pos.z == pos.z;
    auto c2 = o.color.x == color.x &&
              o.color.y == color.y && o.color.z == color.z;
    auto c3 = o.texCoord.x == texCoord.x &&
              o.texCoord.y == texCoord.y;
    return c1 && c2 && c3;
  }
};

std::size_t glm_vec_hash(glm::vec2 v) {
  auto h1x_h = std::hash<float>{}(v.x);
  auto h1y_h = std::hash<float>{}(v.y);
  auto h1xy_h = h1x_h ^ (h1y_h << 1);
  return h1xy_h;
}

std::size_t glm_vec_hash(glm::vec3 v) {
  auto h1xy_h = glm_vec_hash(glm::vec2(v.x, v.y));
  auto h1 = h1xy_h ^ (std::hash<float>()(v.z) << 1);
  return h1 >> 1;
}

inline std::ostream &operator<<(std::ostream &out,
                                const Vertex &v) {
  return out << "vertex position: x: " << v.pos.x
             << " y: " << v.pos.y << std::endl
             << " vertex color: r: " << v.color.x
             << " g: " << v.color.y << " b: " << v.color.z
             << " vertex texCoord x" << v.texCoord.x
             << " y: " << v.texCoord.y << std::endl;
}

namespace std {
template <> struct hash<Vertex> {
  size_t operator()(Vertex const &v) const {
    //
    stringstream ss;
    ss << v;
    string sv = ss.str();
    return hash<string>()(sv);
  }
};
};
