# T0001: Build GLFW from source, link statically

Status: open
Type: tech debt
Raised: 2026-11-03, Lyra, during 001 implementation planning
Related: [001 2-implementation.md §0.5](../features/001-window-loop-triangle/2-implementation.md#05-making-buildrelease-runnable-on-another-machine)

## What and why

Lyra wants `build/release/` to be plug-and-play: rsync it to any machine and
run it with nothing to install. Right now GLFW is linked as a shared library
(`libglfw.so.3`, the only form Debian ships), so the running machine needs
`libglfw3` installed. 001 accepts that for now.

## Notes

- GLFW source would go in `external/glfw/`. Lyra downloads it (the agent has no
  web access). It could be a git submodule or a plain copy. Decide when picked up.
- The VM needs the X11/Wayland dev headers to build GLFW from source
  (roughly `libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
  libwayland-dev libxkbcommon-dev wayland-protocols`. Confirm against GLFW's
  docs at the time).
- Replaces the "GLFW from the system package" decision in 001 planning (§E).
- Doesn't solve glibc: the running machine still needs glibc ≥ the build
  machine's (2.41 on Debian 13). That can't be bundled.

## Done when

`build/release/` runs on a machine that doesn't have `libglfw3` installed
(`ldd build/release/artificial-space` shows no `libglfw`).

## Log

- 2026-11-03: raised. Deferred: 001 keeps GLFW as a runtime requirement.
