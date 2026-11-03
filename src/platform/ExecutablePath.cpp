#include "platform/ExecutablePath.hpp"

#include <format>
#include <stdexcept>
#include <system_error>

std::filesystem::path executable_dir()
{
    // On Linux, /proc/self/exe is a symlink to the running program's file.
    std::error_code error;
    const std::filesystem::path exe = std::filesystem::read_symlink("/proc/self/exe", error);
    if (error) {
        throw std::runtime_error(
            std::format("couldn't find the executable's location (/proc/self/exe): {}",
                        error.message()));
    }
    return exe.parent_path();
}
