#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 flag_color;
flat in int HP;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // TODO(student): Write pixel out color
    //out_color = vec4(flag_color, 1);

    if (HP == 3 || HP == -1)
    {
        out_color = vec4(flag_color, 1);
    }
    else
    {
        int deform = 3 - HP;
        out_color = vec4(flag_color - vec3(0.2 * deform), 1);
    }
}
