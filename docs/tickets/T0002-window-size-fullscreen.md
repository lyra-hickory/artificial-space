# T0002: Window size and fullscreen

Status: open
Type: idea
Raised: 2026-11-03, Claude, from Lyra's comment in 001 implementation planning
Related: [001 2-implementation.md §1.1](../features/001-window-loop-triangle/2-implementation.md#11-maincpp)

## What and why

001 opens a fixed 800×600 window (Lyra: "keep it small for now"). Later we'll
want a bigger default, and a choice of fullscreen or windowed.

## Notes

- GLFW goes fullscreen when `glfwCreateWindow` is given a monitor
  (`glfwGetPrimaryMonitor()`). "Borderless fullscreen" means using the monitor's
  current video mode.
- Switching at runtime: `glfwSetWindowMonitor`.
- Probably wants a settings/config file eventually. Don't hard-code it twice.

## Done when

The window size and fullscreen/windowed choice can be changed without editing
`main.cpp`.

## Log

- 2026-11-03: raised. 001 uses a hard-coded 800×600 window.
