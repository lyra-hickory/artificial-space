# Artificial Space

A "vibe-coded" game written in C++.

## Documentation

For further detail you can drill down into the various documents

- [Vision](docs/VISION.md)
- [Architecture](docs/ARCHITECTURE.md)
- [Dependencies](docs/DEPENDENCIES.md)
- [Tickets](docs/tickets/README.md)

## Building

Install the build dependencies (Debian 13 or similar, details in
[Dependencies](docs/DEPENDENCIES.md)):

```
sudo apt install g++ cmake libglfw3-dev
```

Then build one of the two configurations:

```
cmake --preset debug   && cmake --build --preset debug     # → build/debug/
cmake --preset release && cmake --build --preset release   # → build/release/
```

- **debug**: sanitizers (ASan/UBSan) on, slower. For development.
- **release**: optimised. Pre-alpha, no versioning yet.

## Running

```
./build/release/artificial-space     # or ./build/debug/artificial-space
```

`build/release/` is self-contained (the executable plus its `shaders/`
folder), so it can be copied to another machine and run there. That machine
needs:

- `libglfw3` (GLFW 3.4): `sudo apt install libglfw3`
- glibc ≥ 2.41 (Debian 13 / current LMDE)
- a GPU driver with OpenGL 4.5

Press **Escape** to quit.

## Some thoughts

The main idea of this is to try my hand at a few different ways of working with AI, and how I am handling working in a language I am not super familiar with. I am aiming to own every architectural decision, and actually understand everything we do. None of this is meant to be vibe coded without looking at the code. The aim is to have a polished game, and build something I really enjoy.

