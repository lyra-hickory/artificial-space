# 001: Window, loop, triangle (planning)

Status: **signed off 2026-10-30.**

## Goal

The smallest program that proves the whole stack works: open a window, run a
loop, draw one coloured triangle, and close cleanly. Everything else in the
game sits on top of this, so the choices here are really choices about the
project's foundations.

## Requirements

- Opens a window on Lyra's Linux machine.
- Draws a triangle using the GPU (not pixel-by-pixel on the CPU).
- Runs a loop that keeps going until the window is closed (or Escape is pressed).
- Builds with one command from a fresh clone.
- Code layout that can grow (rendering, physics, game) without a rewrite.
- Every library used is small, standard, and understood.

Not in scope: maths library, camera, 3D, input beyond "quit".

## Decisions to make

Each section has the options, the trade-offs, and a recommendation. These
are proposals to discuss, not decisions.

### A. Graphics API: how we talk to the GPU

| Option | For | Against |
|---|---|---|
| **OpenGL 4.x** (recommended) | Decades of tutorials (learnopengl.com), a triangle in ~100 lines, more than enough for this game | Older design, no longer evolving |
| Vulkan | Modern, explicit, teaches how GPUs really work | A triangle takes ~1000 lines. We'd spend months on plumbing before any space |

**Recommendation:** OpenGL 4.x (4.5 if the driver supports it, for its cleaner
"direct state access" style). It gets us to the game fastest while still
being properly low-level: we write our own shaders and manage our own buffers.

Lyra:
So I am happy with us going with OpenGL, does this give us a lot of plumbing? I can keep the 1000 lines for a triangle for when I want to mess with low level stuff myself. I think might be worth discussing what this actually gives us so that I can understand the suggestion and what we're getting.

### B. Window + input library

| Option | For | Against |
|---|---|---|
| **GLFW** (recommended) | Tiny, does one job: window, OpenGL context, keyboard/mouse. Very common with OpenGL | No audio. We'd add a separate small library later if we want sound |
| SDL3 | Also handles audio, gamepads, and more | Bigger, does more than we need right now |
| Raw X11/Wayland | Nothing hidden | Hundreds of lines of platform code that don't make the game better |

**Recommendation:** GLFW.

### C. OpenGL loader

OpenGL functions have to be looked up from the graphics driver at runtime.
A loader generates that lookup code.

**Recommendation:** **glad**: generated once, a single `.c`/`.h` pair checked
into the repo. Nothing to install.

### D. Build system

| Option | For | Against |
|---|---|---|
| **CMake** (recommended) | Industry standard, handles dependencies and IDE/clangd integration well | Its own odd language to learn |
| Plain Makefile | Simple, transparent, matches `fun-with-c` | Gets painful with dependencies and multiple directories |

**Recommendation:** CMake, kept minimal and commented so it stays readable.

### E. Getting dependencies

- **System packages** (`apt install libglfw3-dev`): simplest, but needs a setup step.
- **Vendored** (copy the source into `external/`): self-contained, but bloats the repo.
- **CMake FetchContent** (download at build time): self-contained, but needs network access on the first build.

**Recommendation:** GLFW from the system package, glad vendored (it's two
files). Revisit if we ever want to build on other machines.

### F. C++ version and compiler

**Recommendation:** C++23 (or C++20 if the compiler is older) with GCC or Clang,
warnings turned up high (`-Wall -Wextra -Wpedantic`), and AddressSanitizer
in debug builds to catch memory bugs early.

### G. The game loop

This doesn't matter for the triangle, but it will for physics, so worth knowing
now. The common pattern is a **fixed timestep**: physics advances in fixed
steps (say 1/120 s) whatever the frame rate, and rendering happens as often as
it can. This keeps the simulation deterministic and stable, which matters a
lot for orbits. For 001 we'd just set up the loop's shape. The fixed step
itself comes in feature 005.

### H. Project layout (proposed)

```
artificial-space/
  CMakeLists.txt
  src/
    main.cpp
    platform/   window, input (wraps GLFW)
    render/     OpenGL code, shaders
  shaders/      .vert / .frag files
  external/
    glad/
```

## To check before the implementation doc

Toolchain on Lyra's machine (Lyra to run, since I don't have shell access):

```
g++ --version; clang++ --version; cmake --version
pkg-config --modversion glfw3
glxinfo -B | grep -i "opengl core profile version"
```

### Results (2026-10-28)

- GCC 14.2.0: good C++23 support (including `std::print`). ✅
- clang, cmake, pkg-config, GLFW, glxinfo: not installed.
- To install: `sudo apt install cmake pkg-config libglfw3-dev mesa-utils`
- OpenGL version: still unknown until `glxinfo` is installed.

## Agreed direction (2026-10-30)

Lyra signed off on the proposed stack:

- **OpenGL 4.x** for drawing (4.5 if the driver supports it)
- **GLFW** for the window and input, from the system package
- **glad** as the OpenGL loader, vendored in `external/glad/`
- **CMake** as the build system
- **C++23** on GCC 14, strict warnings, AddressSanitizer in debug
- Loop shaped for a fixed timestep later (feature 005)

How the pieces fit (see the discussion of 2026-10-28):

```
main()
  ├─ set up:  GLFW opens window → glad loads OpenGL → upload triangle
  ├─ loop:    poll input → update → draw → swap buffers
  └─ clean up
```

- **GLFW:** gets a window and input events from the OS.
- **glad:** looks up the OpenGL functions inside the GPU driver at start-up.
- **OpenGL:** the commands we send the GPU (buffers, shaders, draw calls).
  Not an engine: we write the engine.

Still outstanding: install `cmake pkg-config libglfw3-dev mesa-utils` and check
the OpenGL version.

Next: code architecture, then the implementation doc.
