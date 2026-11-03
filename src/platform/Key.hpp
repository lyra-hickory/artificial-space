#pragma once

// Our own key names. GLFW's key codes (GLFW_KEY_ESCAPE, ...) never leave
// platform/, so the coming Input feature only has to change things here.
// `enum class` (not plain `enum`): the names are scoped (Key::Escape) and
// don't silently convert to int.
enum class Key {
    Escape,
};
