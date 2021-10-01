// vulkan buffer object
#pragma once
#include <external.hpp>
#include <stdexcept>

using namespace vtuto;

namespace vtuto {
template <class BufferType> class vulkan_buffer {
public:
  BufferType buffer;

public:
  vulkan_buffer() {}
};

template <class BufferType> class vulkan_buffers {
private:
  std::vector<BufferType> buffers;

public:
  vulkan_buffers() {}
  vulkan_buffers(
      const std::vector<vulkan_buffer<BufferType>> &bs) {
    buffers.clear();
    buffers.resize(bs.size());
    for (std::size_t i = 0; i < bs.size(); i++) {
      buffers[i] = bs[i].buffer;
    }
  }
  void insert(std::size_t i,
              const vulkan_buffer<BufferType> &b) {
    if (i < buffers.size()) {
      buffers[i] = b.buffer;
    }
  }
  BufferType get(std::size_t i) {
    if (i < buffers.size()) {
      return buffers[i];
    } else {
      throw std::runtime_error("index out of bounds");
    }
  }
  std::size_t size() { return buffers.size(); }
  BufferType *data() { return buffers.data(); }
  void resize(std::size_t s) { buffers.resize(s); }
  std::vector<BufferType> to_vec() const { return buffers; }
};
}
