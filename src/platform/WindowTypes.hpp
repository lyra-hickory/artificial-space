#pragma once

// Plain types that cross from platform/ to render/.
// No GLFW and no OpenGL in here, so render/ can include it without depending
// on either.

// The window's drawable area in pixels. Can differ from the window size on
// high-DPI screens, which is why we ask for this one.
struct FramebufferSize {
  int width = 0;
  int height = 0;
};

// A function that, given an OpenGL function's name, returns its address in
// the driver. GLFW provides one (glfwGetProcAddress), and glad uses it to load
// OpenGL. Reads as: "pointer to a function taking const char* and returning
// a pointer to a function taking nothing and returning nothing".
using GlProc = void (*)();
using GlLoader = GlProc (*)(const char *name);
