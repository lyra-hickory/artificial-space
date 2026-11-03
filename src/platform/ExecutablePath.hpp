#pragma once

#include <filesystem>

// The folder the running executable lives in, e.g. ~/artificial-space/.
// Used to find shaders/ next to it, so the build folder works wherever it's
// copied. OS-specific (Linux: /proc/self/exe), hence it lives in platform/.
// Throws std::runtime_error if it can't be determined.
std::filesystem::path executable_dir();
