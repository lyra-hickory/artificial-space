# Tickets

Small things to do later: TODOs, small tasks, tech debt, ideas. Anything
bigger becomes a feature in `docs/features/`.

## How tickets work

- **One file per ticket:** `T0001-short-name.md`. Numbers go up from `T0001` and
  are never reused (room for 9999).
- **Status** lives inside the file and in the index below. Files never move.
  Status values: `open` · `in progress` · `done` · `won't do`.
- **Type:** `tech debt` · `idea` · `bug` · `chore`.
- **Process:** a ticket skips the three-doc feature process. Discuss it,
  Lyra gives the go-ahead, do it, and record what was decided and why in the
  ticket's Log. If a ticket turns out to need real design work, it's promoted
  to a feature: mark it `done`, and link the feature.
- **Either Lyra or the agent can raise one.** The agent says so when it does.
- **Keep this index up to date** whenever a ticket is created or changes status.

## Template

```markdown
# T0000: Title

Status: open
Type: chore
Raised: YYYY-MM-DD, who, during what
Related: links to docs / features

## What and why

## Notes

## Done when

## Log
```

## Index

| Ticket | Title | Status | Type |
|---|---|---|---|
| [T0001](T0001-glfw-static.md) | Build GLFW from source, link statically | open | tech debt |
| [T0002](T0002-window-size-fullscreen.md) | Window size and fullscreen | open | idea |
| [T0003](T0003-gl-handle-template.md) | Refactor GL wrappers into a generic handle template | open | tech debt |
| [T0004](T0004-clang-format.md) | Consistent code formatting (clang-format) | done | chore |
