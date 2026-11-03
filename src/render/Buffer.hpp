#pragma once

#include <glad/gl.h>

#include <cstddef>
#include <span>

// A block of GPU memory (an OpenGL buffer object), filled once at creation (RAII).
// Fixed size: created with glNamedBufferStorage, so the driver knows it will
// never be resized.
//
// Not copyable (two copies would delete the same GPU buffer), movable (the
// source is left holding 0, and deleting 0 is a no-op in OpenGL).
class Buffer {
public:
    explicit Buffer(std::span<const std::byte> data);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

    GLuint id() const { return m_id; }

private:
    GLuint m_id = 0;
};
