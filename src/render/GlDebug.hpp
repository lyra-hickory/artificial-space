#pragma once

// Ask the driver to report OpenGL errors to us as readable messages
// (glDebugMessageCallback), printed to stderr.
// Does nothing unless the context is a debug context, which Window only
// requests in Debug builds. So it's safe to call unconditionally.
void enable_gl_debug_output();
