# Architecture

Architecture Document for keeping track of the arhcitectural decisions and shape of the application.

This is a living document and prone to change as decisions are made.

## Stack

C++23

### compiling
- GCC 14
- CMake

### rendering and input
- OpenGL 4.5: how we talk to the GPU
- GLFW: window, OpenGL context, keyboard/mouse
- glad: loads OpenGL functions from the driver (vendored)

Versions, install steps and licences: [DEPENDENCIES.md](DEPENDENCIES.md).

## Layout

```
artificial-space/
  CMakeLists.txt        build definition
  CMakePresets.json     named build configs: debug, release
  .gitignore            build/, *.swp
  docs/
    VISION.md           premise, decisions, open questions
    ARCHITECTURE.md     this document
    DEPENDENCIES.md     every dependency: version, why, how to install
    features/NNN-name/  1-planning, 2-implementation, 3-debrief
    tickets/            small TODOs: README.md (index) + Txxxx-name.md
  src/
    main.cpp            init + loop
    platform/           OS-facing: GlfwLibrary, Window, Input
    render/             GPU-facing: Renderer, ShaderProgram, Buffer, VertexArray
  shaders/              .vert / .frag, loaded at runtime
  external/
    glad/               vendored loader (generated, not edited)
  build/                CMake output
    debug/              dev build: sanitizers on, runs on the dev VM
    release/            optimised build, self-contained folder
      artificial-space    the executable
      shaders/            copied in at build time, found next to the executable
```

### Dev Machines

- **Dev VM** (Debian 13): where the code is written and built. It's
  reached over SSH. It has no real GPU (software rendering), so it's only for
  building and quick checks. Design decisions are not based on its specs.
- **Main PC** (Linux Mint Debian Edition, kept up to date, powerful GPU):
  where the game runs. Same Debian 13 base as the dev VM.

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
- OpenGL 4.5 core. No raw GL calls outside `render/`. The wrappers are the
  only GL-aware code, so dropping to 4.1 (e.g. for macOS) stays a contained
  change. Agreed 2026-11-03.

## Notes

- Input direction (Lyra): semantic actions (e.g. `forward-down`) that callers
  register hooks on; actions map onto keybindings internally. Not designed yet.
  001 only needs Escape → quit.
- Tick will become fixed-timestep, decoupled from render rate (feature 005).
- Layout: folders split by what they talk to. `platform/` = OS (via GLFW),
  `render/` = GPU (via OpenGL), future `sim/` = neither (testable headless).
  Each class is a `.hpp` + `.cpp` pair side by side, no `include/` dir.
  Shaders are copied next to the executable at build time and found relative
  to it (`/proc/self/exe`, wrapped in `platform/`), so `build/release/` works
  wherever it's copied. This replaced an earlier plan to bake in an absolute
  path. Details in 001 `2-implementation.md` §0.5. The dev VM renders on
  llvmpipe, in software: never tune for it.

