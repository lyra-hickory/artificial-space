# 001: Window, loop, triangle (debrief)

Status: **written 2026-11-04**, ready for Lyra to commit.

What was built, how it fits together, and why. Section numbers (§) point
back to `2-implementation.md`, where each decision's motivation was
recorded before building.

## What we have

`cmake --preset debug && cmake --build --preset debug` builds a program that
opens an 800×600 window titled "Artificial Space" and draws a gradient
triangle (red, green, blue corners) on a near-black background. Escape or
the close button quits. Lyra checked it visually on the dev VM.

## How it fits together

```
main()                                  src/main.cpp
  GlfwLibrary glfw        GLFW on       platform/   (talks to the OS via GLFW)
  Window window           window + GL 4.5 context, VSync
  Renderer renderer       glad loads GL, triangle uploaded   render/ (talks to the GPU)
  run(window, renderer)   input → tick → render, once per monitor refresh
  } ← destroyed in reverse: renderer, window, glfw
```

- **`platform/`** is the only code that includes GLFW. **`render/`** is the only
  code that calls OpenGL. They meet through `WindowTypes.hpp`: two plain types,
  the window size and the GL loader function.
- **Shaders** (`shaders/`) are copied next to the executable at build time and
  found relative to it, so `build/release/` can be copied anywhere.

## Why it's built this way

| Decision | Why | § |
|---|---|---|
| Each start-up step is a local in `main` (RAII) | The compiler runs destructors in reverse order on every exit path, including exceptions. We only write *what* cleanup does | 1.2 |
| `run()` is a free function in `main.cpp` | ~20 lines. A class would be structure ahead of need | 1.3 |
| VSync on | Paces the loop to the monitor, with no wasted 100% CPU/GPU and no tearing | 1.3 |
| Window size asked for every frame | No callbacks, no shared state. Data flows platform → render | 1.3 |
| Escape polled in `run()` | Just enough input for 001. Lyra's semantic-action Input is its own feature, next | 1.4 |
| Exceptions for start-up failures, one `catch` in `main` | Fits RAII: whatever was built gets cleaned up | 3.1 |
| Callbacks print, never throw | GLFW and the driver are C, and exceptions can't cross C code | 3.2 |
| Debug + release presets | Sanitizers while developing, a fast clean build to play | 0.4 |
| GLFW from apt, required at runtime | Simple for now. Plug-and-play is T0001 | 0.5 |

`render/` (GPU objects, shaders, glad) is deliberately a black box for now.
Lyra chose to focus on the parts above. Its design is in Part 2 of the
implementation doc, and the code is commented, for whenever that's of interest.

## What turned out differently

- **Small additions the doc didn't spell out**, listed in the implementation
  doc's *Build notes*: `WindowTypes.hpp`, `ExecutablePath`, `Renderer` taking
  the shader folder, `draw()` being `const`, a start-up line naming the GPU.
- **glad was first generated as Compatibility, not Core.** It was easy to miss in
  the web form. It's now noted in DEPENDENCIES.md.
- **Leak reports:** LeakSanitizer reports ~101 KB from the Mesa driver at
  exit, so the Debug build exits with code 1. A suppression file didn't
  work cleanly, so Lyra chose to treat it as a known baseline (Build notes).
- **Editor tooling** came up during review: `.clangd` (so the editor sees the
  include paths) and `.clang-format` (T0004, LLVM style to match Lyra's editor).

## What was learned

- RAII is the same mechanism as `std::vector` and friends. The compiler (not
  the preprocessor) inserts destructor calls and exception unwind tables.
  `~Name()` is the destructor, and we write what it does.
- `#version 450 core` picks the shader language version and drops deprecated
  features.
- `glfwSwapInterval(1)`: double buffering, and the swap waits for the monitor,
  which is what paces the loop. That's why physics (005) needs its own timestep.
- Debugging leaks: ASan's default short stack traces hide where allocations
  came from. `fast_unwind_on_malloc=0` gives full traces.

## Checklist (Part 4)

- [x] Both presets build with zero warnings
- [x] Window, triangle and colours correct (Lyra, Proxmox console)
- [x] Escape / close quits. Resizing doesn't crash
- [x] Starts from any working directory
- [x] Error checks: missing shader, shader typo, GL 4.6 request, bad GL call
- [~] Sanitizers: no ASan/UBSan errors. Leak report = known driver baseline
- [ ] **`build/release/` on the main PC**: not done yet (Lyra)

(ARCHITECTURE.md's Layout updated 2026-11-04 to list the new `platform/` files.)

## Follow-ups

- **Next feature:** Input (Lyra's semantic actions + hooks), probably 002.
  VISION's build order would shift by one.
- Tickets: T0001 (static GLFW), T0002 (window size/fullscreen), T0003 (GL
  handle template).
