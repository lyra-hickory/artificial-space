# T0004: Consistent code formatting (clang-format)

Status: done
Type: chore
Raised: 2026-11-04, Claude, after Lyra's editor reformatted some 001 files
Related: `.clang-format`, `external/.clang-format`

## What and why

Lyra's editor formats in clang-format's default LLVM style (2-space indent,
80 columns, `const T &`). The agent had written 001 in a 4-space style, so the
files were mixed. Lyra wants everything in the editor's style.

## Notes

- `.clang-format` at the repo root: `BasedOnStyle: LLVM`. It's exactly what
  Lyra's editor already produced (checked: Lyra's reformatted `Key.hpp`,
  `GlfwLibrary.hpp/.cpp` were unchanged by `clang-format --style=LLVM`).
- `external/.clang-format` turns formatting off for generated/vendored code
  (glad), so it never gets touched.
- Format a file: `clang-format -i <file>`. clang-format 19 is installed on the
  dev VM.
- Hand-aligned tables (e.g. the triangle's vertex data) lose their column
  alignment. That's acceptable for now. `// clang-format off` / `on` can protect
  a block if one ever needs it.

## Done when

All of `src/` matches `.clang-format`, and glad is untouched.

## Log

- 2026-11-04: Lyra approved matching the editor's style. Added both
  `.clang-format` files and ran `clang-format -i` on all of `src/` (13 files
  changed). Verified every file is clean and glad unchanged. Both presets
  rebuild with zero warnings.
