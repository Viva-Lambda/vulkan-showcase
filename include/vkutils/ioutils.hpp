#pragma once
// input output utility functions

#include <external.hpp>

namespace vtuto {

std::array<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "failed to open file! " << filename << std::endl;
  }

  std::size_t fileSize = static_cast<std::size_t>(file.tellg());
  std::array<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

}; // namespace vtuto
