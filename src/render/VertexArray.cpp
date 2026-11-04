#include "render/VertexArray.hpp"

#include <utility>

namespace {

// OpenGL lets a vertex array read from several buffers through numbered
// "binding points". We only ever use one buffer per array, so: binding 0.
constexpr GLuint binding_index = 0;

} // namespace

VertexArray::VertexArray() { glCreateVertexArrays(1, &m_id); }

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_id); // 0 (moved from) is silently ignored
}

VertexArray::VertexArray(VertexArray &&other) noexcept
    : m_id{std::exchange(other.m_id, 0)} {}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
  if (this != &other) {
    glDeleteVertexArrays(1, &m_id);
    m_id = std::exchange(other.m_id, 0);
  }
  return *this;
}

void VertexArray::set_vertex_buffer(const Buffer &buffer, GLsizei stride) {
  glVertexArrayVertexBuffer(m_id, binding_index, buffer.id(), 0, stride);
}

void VertexArray::add_float_attribute(GLuint location, GLint components,
                                      GLuint offset) {
  glEnableVertexArrayAttrib(m_id, location);
  glVertexArrayAttribFormat(m_id, location, components, GL_FLOAT, GL_FALSE,
                            offset);
  glVertexArrayAttribBinding(m_id, location, binding_index);
}

void VertexArray::bind() const { glBindVertexArray(m_id); }
