#pragma once

#include "platform/Key.hpp"
#include "platform/WindowTypes.hpp"

#include <string>

// Forward declaration: we only store a pointer to GLFW's window type, so we
// don't need GLFW's header here. Keeps GLFW out of everything that includes us.
struct GLFWwindow;

struct WindowSettings {
    int width = 800;
    int height = 600;
    std::string title = "Artificial Space";
};

// The OS window and its OpenGL 4.5 core context (RAII).
// Constructor creates both and turns on VSync. Destructor destroys them.
//
// Not copyable (two copies would destroy the same window), but movable
// (ownership can be handed over; the source is left empty).
class Window {
public:
    explicit Window(const WindowSettings& settings);   // throws std::runtime_error
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    bool should_close() const;
    void request_close();
    void poll_events();                       // process OS events (keys, resize, close button)
    void swap_buffers();                      // show the finished frame; waits for VSync
    bool is_key_down(Key key) const;
    FramebufferSize framebuffer_size() const;
    GlLoader gl_loader() const;               // handed to Renderer so it can load OpenGL

private:
    GLFWwindow* m_handle = nullptr;
};
