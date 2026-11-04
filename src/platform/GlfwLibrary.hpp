#pragma once

// Owns GLFW being initialised: glfwInit() in the constructor, glfwTerminate()
// in the destructor (RAII). Create exactly one, before any Window.
//
// Neither copyable nor movable: GLFW is one global thing per program, so there
// is exactly one of these and it stays put.
class GlfwLibrary {
public:
  GlfwLibrary(); // throws std::runtime_error if GLFW can't start
  ~GlfwLibrary();

  GlfwLibrary(const GlfwLibrary &) = delete;
  GlfwLibrary &operator=(const GlfwLibrary &) = delete;
  GlfwLibrary(GlfwLibrary &&) = delete;
  GlfwLibrary &operator=(GlfwLibrary &&) = delete;
};
