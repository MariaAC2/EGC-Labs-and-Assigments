#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 flag_position;
in vec3 flag_normal;
in vec2 flag_texture;
in vec3 flag_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    //out_color = vec4(flag_normal, 1);
    out_color = vec4(flag_color, 1);
}
