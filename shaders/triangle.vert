#version 450 core

// Runs once per corner of the triangle.
// Inputs come from the vertex buffer, laid out as described by the VertexArray.
layout(location = 0) in vec3 in_position;  // clip space: -1..1 on each axis
layout(location = 1) in vec3 in_colour;    // red, green, blue: 0..1

// Passed on to the fragment shader. The GPU blends it across the triangle.
out vec3 v_colour;

void main()
{
    v_colour = in_colour;
    gl_Position = vec4(in_position, 1.0);
}
