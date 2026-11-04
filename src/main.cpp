// Artificial Space: entry point.
//
// main: init everything, hand it to the loop.
// loop: input -> tick -> render.
// See docs/ARCHITECTURE.md and docs/features/001-window-loop-triangle/.

#include "platform/ExecutablePath.hpp"
#include "platform/GlfwLibrary.hpp"
#include "platform/Window.hpp"
#include "render/Renderer.hpp"

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <print>

namespace {

// main owns everything; the loop only borrows it (by reference).
void run(Window &window, const Renderer &renderer) {
  while (!window.should_close()) {
    // -- input --
    window.poll_events();
    if (window.is_key_down(Key::Escape)) {
      window.request_close();
    }

    // -- tick --
    // nothing yet: fixed timestep arrives in feature 005

    // -- render --
    renderer.draw(window.framebuffer_size());
    window.swap_buffers();
  }
}

} // namespace

int main() {
  try {
    // -- init --
    // Constructed top to bottom. Destroyed bottom to top when this block
    // ends, including if any of them throws partway (RAII).
    GlfwLibrary glfw;
    Window window{{.width = 800, .height = 600, .title = "Artificial Space"}};
    Renderer renderer{window.gl_loader(), executable_dir() / "shaders"};

    // -- pass over to loop --
    run(window, renderer);
  } catch (const std::exception &e) {
    std::println(stderr, "fatal: {}", e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
