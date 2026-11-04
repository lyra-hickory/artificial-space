#include "platform/Window.hpp"

#define GLFW_INCLUDE_NONE // stop GLFW pulling in the system's old <GL/gl.h>;
                          // glad provides GL
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <utility>

namespace {

int to_glfw_key(Key key) {
  switch (key) {
  case Key::Escape:
    return GLFW_KEY_ESCAPE;
  }
  // Every Key is handled above. C++23's std::unreachable() tells the
  // compiler so (no "missing return" warning). In Debug builds, UBSan
  // reports it if we ever get here anyway.
  std::unreachable();
}

} // namespace

Window::Window(const WindowSettings &settings) {
  // Hints apply to the next window GLFW creates.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE); // no deprecated old-style GL
#ifndef NDEBUG
  glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif

  GLFWmonitor *Monitor =
      nullptr; // → an ordinary window, not fullscreen (see T0002).
  m_handle = glfwCreateWindow(settings.width, settings.height,
                              settings.title.c_str(), Monitor, nullptr);
  if (m_handle == nullptr) {
    throw std::runtime_error(
        "couldn't create window (is OpenGL 4.5 core available?)");
  }

  glfwMakeContextCurrent(m_handle);
  glfwSwapInterval(1);
}

Window::~Window() {
  if (m_handle != nullptr) { // nullptr when this Window was moved from
    glfwDestroyWindow(m_handle);
  }
}

// Move: take other's handle and leave other empty (nullptr), so only one
// destructor ever destroys the window. std::exchange(a, b) sets a to b and
// returns a's old value, all in one step.
Window::Window(Window &&other) noexcept
    : m_handle{std::exchange(other.m_handle, nullptr)} {}

Window &Window::operator=(Window &&other) noexcept {
  if (this != &other) { // moving into ourselves would destroy our own window
    if (m_handle != nullptr) {
      glfwDestroyWindow(m_handle);
    }
    m_handle = std::exchange(other.m_handle, nullptr);
  }
  return *this;
}

bool Window::should_close() const {
  return glfwWindowShouldClose(m_handle) == GLFW_TRUE;
}

void Window::request_close() { glfwSetWindowShouldClose(m_handle, GLFW_TRUE); }

void Window::poll_events() { glfwPollEvents(); }

void Window::swap_buffers() { glfwSwapBuffers(m_handle); }

bool Window::is_key_down(Key key) const {
  return glfwGetKey(m_handle, to_glfw_key(key)) == GLFW_PRESS;
}

FramebufferSize Window::framebuffer_size() const {
  FramebufferSize size;
  glfwGetFramebufferSize(m_handle, &size.width, &size.height);
  return size;
}

GlLoader Window::gl_loader() const {
  // glfwGetProcAddress has exactly the GlLoader signature, so we hand the
  // function itself over as a value.
  return &glfwGetProcAddress;
}
