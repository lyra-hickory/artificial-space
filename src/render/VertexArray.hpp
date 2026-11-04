#pragma once

#include "render/Buffer.hpp"

#include <glad/gl.h>

// Describes how to read vertex data out of a Buffer (an OpenGL vertex array
// object), e.g. "attribute 0 = 3 floats at byte 0, attribute 1 = 3 floats at
// byte 12, each vertex is 24 bytes" (RAII).
//
// Not copyable, movable: same reasoning as Buffer.
class VertexArray {
public:
  VertexArray();
  ~VertexArray();

  VertexArray(const VertexArray &) = delete;
  VertexArray &operator=(const VertexArray &) = delete;
  VertexArray(VertexArray &&other) noexcept;
  VertexArray &operator=(VertexArray &&other) noexcept;

  // Read vertices from `buffer`, one every `stride` bytes.
  void set_vertex_buffer(const Buffer &buffer, GLsizei stride);

  // Shader input `location` = `components` floats, `offset` bytes into each
  // vertex.
  void add_float_attribute(GLuint location, GLint components, GLuint offset);

  void bind() const;
  GLuint id() const { return m_id; }

private:
  GLuint m_id = 0;
};
