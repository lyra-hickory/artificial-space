# T0003: Refactor GL wrappers into a generic handle template

Status: open
Type: tech debt
Raised: 2026-11-03, Claude, agreed with Lyra in 001 implementation planning
Related: [001 2-implementation.md §2.2](../features/001-window-loop-triangle/2-implementation.md#22-the-raii-wrapper-pattern)

## What and why

001 hand-writes the RAII pattern (owns a `GLuint`, deletes it in the
destructor, can't be copied, can be moved) in `Buffer`, `VertexArray` and
`ShaderProgram`. That's deliberate, so the pattern sinks in. Once the
repetition gets annoying (more GL object types arrive with textures and
framebuffers), pull the shared part into one template, e.g.
`GlHandle<DeleteFn>`, and use it as a C++ templates lesson.

## Notes

- Only the ownership part is shared. Each class keeps its own constructor
  and methods.
- `ShaderProgram` deletes with `glDeleteProgram(id)`, while buffers and vertex
  arrays use `glDelete*(1, &id)`. The template needs to handle both shapes.

## Done when

The move/delete/no-copy logic exists in one place, and the wrappers use it.

## Log

- 2026-11-03: raised. 001 hand-writes all three.
