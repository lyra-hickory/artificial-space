#include "render/Buffer.hpp"

#include <utility>

Buffer::Buffer(std::span<const std::byte> data) {
  glCreateBuffers(1, &m_id);
  // DSA style: edit the buffer by its id. No "bind first" step.
  // Flags 0: we never write to or read from it from the CPU again.
  glNamedBufferStorage(m_id, static_cast<GLsizeiptr>(data.size()), data.data(),
                       0);
}

Buffer::~Buffer() {
  glDeleteBuffers(1, &m_id); // m_id == 0 (moved from) is silently ignored
}

Buffer::Buffer(Buffer &&other) noexcept : m_id{std::exchange(other.m_id, 0)} {}

Buffer &Buffer::operator=(Buffer &&other) noexcept {
  if (this != &other) {
    glDeleteBuffers(1, &m_id);
    m_id = std::exchange(other.m_id, 0);
  }
  return *this;
}
