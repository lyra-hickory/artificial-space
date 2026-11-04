#include "render/Renderer.hpp"

#include "render/GlDebug.hpp"

#include <array>
#include <cstddef>
#include <cstdio>
#include <format>
#include <print>
#include <span>
#include <stdexcept>

namespace {

struct Vertex {
  std::array<float, 3> position; // clip space: -1..1, the window's edges
  std::array<float, 3> colour;   // red, green, blue: 0..1
};
// The VertexArray layout below assumes no padding between fields.
static_assert(sizeof(Vertex) == 6 * sizeof(float));

// Built at compile time and baked into the program.
constexpr std::array<Vertex, 3> triangle{{
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.3f, 0.2f}}, // bottom left,  red-ish
    {{0.5f, -0.5f, 0.0f}, {0.2f, 1.0f, 0.4f}},  // bottom right, green-ish
    {{0.0f, 0.5f, 0.0f}, {0.3f, 0.5f, 1.0f}},   // top,          blue-ish
}};

// Shader input locations: must match layout(location = N) in triangle.vert.
constexpr GLuint position_location = 0;
constexpr GLuint colour_location = 1;

} // namespace

int Renderer::load_opengl(GlLoader loader) {
  // glad looks up every OpenGL function by name using GLFW's loader.
  // Returns the version it loaded, or 0 on failure.
  const int version = gladLoadGL(loader);
  if (version == 0) {
    throw std::runtime_error("couldn't load OpenGL (glad)");
  }

  const int major = GLAD_VERSION_MAJOR(version);
  const int minor = GLAD_VERSION_MINOR(version);
  if (major < 4 || (major == 4 && minor < 5)) {
    throw std::runtime_error(std::format(
        "OpenGL 4.5 needed, but the driver gave {}.{}", major, minor));
  }

  // Before any GPU objects exist, so problems creating them get reported too.
  enable_gl_debug_output();

  // Which GPU/driver we ended up on, e.g. "llvmpipe" on the dev VM.
  const auto *renderer =
      reinterpret_cast<const char *>(glGetString(GL_RENDERER));
  std::println(stderr, "OpenGL {}.{} on {}", major, minor, renderer);

  return version;
}

Renderer::Renderer(GlLoader loader, const std::filesystem::path &shader_dir)
    : m_gl_version{load_opengl(loader)} // first: see the note in Renderer.hpp
      ,
      m_program{shader_dir / "triangle.vert", shader_dir / "triangle.frag"},
      m_vertices{std::as_bytes(std::span{triangle})} {
  // m_layout was default-constructed (an empty vertex array). Describe it:
  m_layout.set_vertex_buffer(m_vertices, sizeof(Vertex));
  m_layout.add_float_attribute(position_location, 3,
                               offsetof(Vertex, position));
  m_layout.add_float_attribute(colour_location, 3, offsetof(Vertex, colour));
}

void Renderer::draw(FramebufferSize size) const {
  glViewport(0, 0, size.width,
             size.height); // draw into the whole window, at its current size

  glClearColor(0.02f, 0.02f, 0.05f,
               1.0f); // near-black with a hint of blue: space
  glClear(GL_COLOR_BUFFER_BIT);

  m_program.use();
  m_layout.bind();
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(triangle.size()));
}
