#pragma once

#include "platform/WindowTypes.hpp"
#include "render/Buffer.hpp"
#include "render/ShaderProgram.hpp"
#include "render/VertexArray.hpp"

#include <filesystem>

// The only part of render/ that main sees. Loads OpenGL, owns the triangle's
// GPU objects, draws a frame.
//
// No destructor written: the compiler-generated one destroys the members
// (in reverse order), and each member cleans up its own GPU object.
class Renderer {
public:
    // Throws std::runtime_error if OpenGL can't be loaded, is older than 4.5,
    // or the shaders fail.
    Renderer(GlLoader loader, const std::filesystem::path& shader_dir);

    // const: drawing reads state, never changes it ("render never mutates").
    void draw(FramebufferSize size) const;

private:
    static int load_opengl(GlLoader loader);

    // ORDER MATTERS. Members are constructed in the order they're declared
    // here, and all of them before the constructor body runs. OpenGL must be
    // loaded before any GPU object is created, so this comes first.
    int m_gl_version;

    ShaderProgram m_program;
    Buffer m_vertices;
    VertexArray m_layout;
};
