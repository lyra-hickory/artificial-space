#include "platform/GlfwLibrary.hpp"

#define GLFW_INCLUDE_NONE   // stop GLFW pulling in the system's old <GL/gl.h>; glad provides GL
#include <GLFW/glfw3.h>

#include <cstdio>
#include <print>
#include <stdexcept>

namespace {

// GLFW calls this whenever something goes wrong, with a description.
// It's called from C code, so it must NEVER throw: an exception passing through
// C is undefined behaviour. std::println can throw (e.g. if stderr is broken),
// so we catch everything and give up quietly.
void on_glfw_error(int code, const char* description)
{
    try {
        std::println(stderr, "glfw error {:#x}: {}", code, description);
    } catch (...) {
    }
}

} // namespace

GlfwLibrary::GlfwLibrary()
{
    // Before glfwInit(), so a failed init gets reported too.
    glfwSetErrorCallback(on_glfw_error);

    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("couldn't initialise GLFW");
    }
}

GlfwLibrary::~GlfwLibrary()
{
    glfwTerminate();
}
