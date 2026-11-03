# Artificial Space: build definition.
# Build with the presets in CMakePresets.json (see README):
#   cmake --preset debug   && cmake --build --preset debug
#   cmake --preset release && cmake --build --preset release

cmake_minimum_required(VERSION 3.28)
project(artificial-space LANGUAGES C CXX)   # C because glad is written in C

# --- language -----------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)         # fail rather than silently fall back to older C++
set(CMAKE_CXX_EXTENSIONS OFF)               # -std=c++23, not GCC's -std=gnu++23
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)       # compile_commands.json, so clangd/editors understand the code

# Plain `cmake -B somewhere` without a preset still gets a Debug build.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# --- dependencies -------------------------------------------------------------
# GLFW ships its own CMake package, so no pkg-config needed. Gives us the `glfw` target.
find_package(glfw3 3.4 REQUIRED)

# glad: generated OpenGL loader, vendored in external/glad. Built as its own small
# library so our strict warning flags never apply to generated code.
# SYSTEM marks its headers as third-party, so they don't trigger our warnings either.
add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad SYSTEM PUBLIC external/glad/include)

# --- the game -----------------------------------------------------------------
add_executable(artificial-space
    src/main.cpp
    src/platform/ExecutablePath.cpp
    src/platform/GlfwLibrary.cpp
    src/platform/Window.cpp
    src/render/Buffer.cpp
    src/render/GlDebug.cpp
    src/render/Renderer.cpp
    src/render/ShaderProgram.cpp
    src/render/VertexArray.cpp
)

# Lets us write #include "platform/Window.hpp" from anywhere in src/.
target_include_directories(artificial-space PRIVATE src)
target_link_libraries(artificial-space PRIVATE glad glfw)

# Strict warnings on our code only.
target_compile_options(artificial-space PRIVATE
    -Wall -Wextra -Wpedantic
    -Wconversion    # silent narrowing, e.g. double -> float, size_t -> int
    -Wshadow        # a variable hiding another with the same name
)

# Debug builds: AddressSanitizer + UndefinedBehaviourSanitizer.
# Must be on both compile and link. Frame pointers give readable stack traces.
set(SANITIZERS -fsanitize=address,undefined)
target_compile_options(artificial-space PRIVATE
    $<$<CONFIG:Debug>:${SANITIZERS} -fno-omit-frame-pointer>)
target_link_options(artificial-space PRIVATE
    $<$<CONFIG:Debug>:${SANITIZERS}>)

# --- shaders ------------------------------------------------------------------
# Copy shaders/ next to the executable on every build, so the build folder is
# self-contained and can be copied to another machine. A separate target (ALL)
# means editing a shader is picked up even when no C++ changed.
add_custom_target(shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:artificial-space>/shaders
    COMMENT "Copying shaders next to the executable"
)
add_dependencies(artificial-space shaders)
