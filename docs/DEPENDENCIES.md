# Dependencies

Everything Artificial Space needs, why it's there, and how to get it.
For *what* was chosen and how it fits the architecture, see
[ARCHITECTURE.md](ARCHITECTURE.md#stack).

Dependencies are grouped by **where they're needed**:

- **Build:** the machine that compiles the game (the dev VM).
- **Run:** the machine that plays it (e.g. the main PC). Only what's listed
  here has to be installed for `build/release/` to work there.

Last checked: 2026-11-03, Debian 13 (trixie) / LMDE on a Debian 13 base.

---

## Quick install

**Build machine:**

```
sudo apt install g++ cmake libglfw3-dev
```

**Run machine:**

```
sudo apt install libglfw3
```

Plus a working OpenGL 4.5 GPU driver. glibc is already part of the OS.

**Vendored (nothing to install):** glad, which lives in `external/glad/`.

---

## Build tools

### GCC (g++)

| | |
|---|---|
| Version | 14.2 (needs ≥ 14 for C++23 features like `std::print`) |
| Needed for | build |
| Get it | `sudo apt install g++` |
| Licence | GPL-3.0 with the GCC Runtime Library Exception (our program is not affected) |
| Why | Debian's default compiler, with solid C++23 support. 001 planning §F |

Debug builds also use GCC's sanitizer runtimes (`libasan`, `libubsan`). These
are installed automatically with `g++`. Release builds don't use them, so the
run machine doesn't need them.

### CMake

| | |
|---|---|
| Version | 3.31 (needs ≥ 3.28, our `cmake_minimum_required`) |
| Needed for | build |
| Get it | `sudo apt install cmake` |
| Licence | BSD-3-Clause |
| Why | Industry-standard build system. Finds GLFW natively, and works with clangd and IDEs. 001 planning §D |

---

## Libraries

### GLFW

| | |
|---|---|
| Version | 3.4 |
| Needed for | build (`libglfw3-dev`: headers + CMake package) **and** run (`libglfw3`: the shared library) |
| Get it | `sudo apt install libglfw3-dev` (build) / `sudo apt install libglfw3` (run) |
| Licence | zlib/libpng |
| Why | Small library that does one job: window, OpenGL context, keyboard/mouse. 001 planning §B |

It's linked as a shared library because that's the only form Debian ships. That's
why the run machine needs it installed. Making `build/release/` plug-and-play
by building GLFW from source and linking it statically is
[T0001](tickets/T0001-glfw-static.md).

### glad

| | |
|---|---|
| Version | glad 2.0.8, generated 2026-11-03 for `gl:core=4.5`, no extensions, no options |
| Needed for | build only (compiled into the executable) |
| Get it | **Vendored** in `external/glad/`. Generated once, never edited |
| Licence | Generated files: `(WTFPL OR CC0-1.0) AND Apache-2.0` (stated in the `gl.h` header) |
| Why | Looks up OpenGL functions in the GPU driver at start-up. Two files, nothing to install. 001 planning §C |

**How to (re)generate:** at the glad 2 web generator (`gen.glad.sh`), choose
language C/C++, API **gl 4.5**, profile **Core** (not Compatibility, which is
the easy one to miss), no other APIs, no extensions, no options. Unzip into
`external/glad/` so you get `include/glad/gl.h`, `include/KHR/khrplatform.h`
and `src/gl.c`. Don't leave the zip in there. The equivalent command line
(from the generated header) is:

```
pipx run --spec glad2 glad --api gl:core=4.5 --out-path external/glad c
```

Check the top of `gl.h` afterwards: it should say `gl:core=4.5`. Only
needed if the GL version changes. See 001 `2-implementation.md` §2.6.

### OpenGL

| | |
|---|---|
| Version | 4.5 core profile |
| Needed for | run |
| Get it | Comes with the GPU driver (Mesa for AMD/Intel, NVIDIA's own driver, via Mint's Driver Manager) |
| Licence | n/a (it's an API. The driver is the implementation) |
| Why | How we talk to the GPU. 001 planning §A |

Nothing to link against at build time: glad loads it from the driver at
runtime. Check what a machine supports with `glxinfo -B` (from `mesa-utils`)
and look for "Max core profile version".

---

## System runtime

### glibc

| | |
|---|---|
| Version | ≥ 2.41 (the build machine's version) |
| Needed for | run |
| Get it | Part of the OS. Can't be installed separately or bundled |
| Licence | LGPL-2.1+ |
| Why | The C runtime every Linux program uses |

A program runs on a machine with the **same or newer** glibc than it was built
with, never older. Debian 13 and current LMDE both have 2.41.

---

## Optional tools

| Tool | Package | Use |
|---|---|---|
| `glxinfo` | `mesa-utils` | Check a machine's OpenGL version and renderer |
| `pkg-config` | `pkg-config` | Not used by the build (CMake finds GLFW directly). Installed early on, safe to remove |

---

## Adding a dependency

Each new library is discussed before it's added (VISION.md, "Decided").
When one is agreed, add it here with all the table fields, add it to the quick
install commands, and add one line to the Stack list in ARCHITECTURE.md.
