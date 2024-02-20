#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 flag_position;
out vec3 flag_normal;
out vec2 flag_texture;
out vec3 flag_color;

void main()
{
    // TODO(student): Send output to fragment shader
    flag_position = v_position + vec3(sin(Time), cos(Time), 1.0);
    flag_normal = v_normal;
    flag_texture = v_texture;
    flag_color = v_color + vec3(sin(Time), cos(Time), 1.0);

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(flag_position, 1.0);
}
