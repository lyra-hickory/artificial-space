#pragma once

#include <glad/gl.h>

#include <filesystem>

// A linked vertex + fragment shader pair (an OpenGL program object) (RAII).
// Reads both files, compiles each, links them. Any failure throws
// std::runtime_error naming the file, with the driver's error log.
//
// Not copyable, movable: same reasoning as Buffer.
class ShaderProgram {
public:
    ShaderProgram(const std::filesystem::path& vertex_path,
                  const std::filesystem::path& fragment_path);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    void use() const;
    GLuint id() const { return m_id; }

private:
    GLuint m_id = 0;
};
