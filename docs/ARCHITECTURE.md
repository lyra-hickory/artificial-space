# Architecture

Architecture Document for keeping track of the arhcitectural decisions and shape of the application.

This is a living document and prone to change as decisions are made.

## Stack

C++23

### compiling
- GCC 14
- CMake

### rendering and input
- OpenGL 4.5
- GLFW
- glad

## Layout

```
artificial-space/
  CMakeLists.txt        build definition
  .gitignore            build/, *.swp
  docs/                 vision, architecture, feature docs
  src/
    main.cpp            init + loop
    platform/           OS-facing: GlfwLibrary, Window, Input
    render/             GPU-facing: Renderer, ShaderProgram, Buffer, VertexArray
  shaders/              .vert / .frag, loaded at runtime
  external/
    glad/               vendored loader (generated, not edited)
  build/                CMake output, never committed
```

## MAIN

This section describes the shape of our programme, and the main loop

### main
```
// -- init --
// -- pass over to loop --
```

### loop
```
// -- input --
// -- tick --
// -- render --
```

## MODULES

TBD

### Input
Input needs to be determined, I am thinking a handler that wraps raw input matching to our set keys

We might need to set up an API that registers hooks, so that will allow us to register a hook for <forward-down> and then when the hook fires it triggers all of the registered callers and in this case it would engage thrusters. 

So then the actual hooks are semantic actions, and those internally map onto our keybindings.

### Tick
TBD

### Render
TBD

# ---- AI Yaps ----

Maintainer:

    This section is for more yapping, what is written above is as determined by me, this section is for helpful things for AIs. Do not contradict what is written above this section, and aim to keep this maintained if it drifts.

## Rules

- RAII for every resource (GLFW, window, GPU objects). Non-copyable, movable.
- Exceptions for fatal start-up errors only, caught in `main`. None in the loop.
- `main` owns everything; the loop borrows it by reference.
- Data flows one way: input → tick → render. Render never mutates state.
- No structure ahead of need.

## Notes

- Input direction (Lyra): semantic actions (e.g. `forward-down`) that callers
  register hooks on; actions map onto keybindings internally. Not designed yet.
  001 only needs Escape → quit.
- Tick will become fixed-timestep, decoupled from render rate (feature 005).
- Layout: folders split by what they talk to. `platform/` = OS (via GLFW),
  `render/` = GPU (via OpenGL), future `sim/` = neither (testable headless).
  Each class is a `.hpp` + `.cpp` pair side by side, no `include/` dir.
  CMake passes the absolute `shaders/` path to the program as a compile
  definition, so it runs from any working directory.

