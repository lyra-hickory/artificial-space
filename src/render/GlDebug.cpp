#include "render/GlDebug.hpp"

#include <glad/gl.h>

#include <cstdio>
#include <print>
#include <string_view>

namespace {

std::string_view source_name(GLenum source) {
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    return "api";
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    return "window-system";
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    return "shader-compiler";
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    return "third-party";
  case GL_DEBUG_SOURCE_APPLICATION:
    return "application";
  default:
    return "other";
  }
}

std::string_view type_name(GLenum type) {
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    return "error";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    return "deprecated";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    return "undefined-behaviour";
  case GL_DEBUG_TYPE_PORTABILITY:
    return "portability";
  case GL_DEBUG_TYPE_PERFORMANCE:
    return "performance";
  default:
    return "other";
  }
}

std::string_view severity_name(GLenum severity) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    return "HIGH";
  case GL_DEBUG_SEVERITY_MEDIUM:
    return "medium";
  case GL_DEBUG_SEVERITY_LOW:
    return "low";
  default:
    return "info";
  }
}

// Called by the driver (C code). Like GLFW's callback, it must NEVER throw:
// print only, and swallow any exception from printing itself.
// GLAD_API_PTR is the calling convention OpenGL expects (empty on Linux).
void GLAD_API_PTR on_gl_message(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei /*length*/,
                                const GLchar *message,
                                const void * /*user_data*/) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
    return; // chatty info, not problems
  }
  try {
    std::println(stderr, "gl {} {} ({}, id {}): {}", severity_name(severity),
                 type_name(type), source_name(source), id, message);
  } catch (...) {
  }
}

} // namespace

void enable_gl_debug_output() {
  GLint flags = 0;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0) {
    return; // not a debug context (Release build)
  }

  glEnable(GL_DEBUG_OUTPUT);
  // Synchronous: the callback fires inside the GL call that caused the
  // problem, so a debugger's stack trace points straight at our line.
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(on_gl_message, nullptr);
}
