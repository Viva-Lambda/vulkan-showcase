#pragma once

#include <external.hpp>

std::vector<char> read_shader_file(const std::string &fpath) {
  std::ifstream file;
  file.open(fpath, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("file can not be opened");
  }
  file.seekg(0, file.end);
  int length = file.tellg();
  std::vector<char> content(length);
  file.seekg(0);
  file.read(content.data(), length);
  file.close();
  return content;
}

#define CHECK_VK2(call, msg)                                                   \
  do {                                                                         \
    VkResult res = call;                                                       \
    if (res != VK_SUCCESS) {                                                   \
      std::stringstream ss;                                                    \
      ss << "Error in: " << __FILE__ << " :: " << __LINE__ << " :: " << #call  \
         << " :: " << msg << std::endl;                                        \
      std::string s = ss.str();                                                \
      throw std::runtime_error(s);                                             \
    }                                                                          \
  } while (0)

template <typename T> void print_vector(const std::vector<T> &vs) {
  std::cout << std::endl;
  for (const auto &v : vs) {
    std::cout << v << ",";
  }
  std::cout << std::endl;
}
