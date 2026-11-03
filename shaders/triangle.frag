#version 450 core

// Runs once per pixel inside the triangle.
// v_colour arrives already blended between the three corners' colours.
in vec3 v_colour;

layout(location = 0) out vec4 out_colour;

void main()
{
    out_colour = vec4(v_colour, 1.0);  // alpha 1 = fully opaque
}
