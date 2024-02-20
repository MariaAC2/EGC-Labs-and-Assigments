#version 330

// Input
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;
uniform int hasTexture;

uniform vec3 colors[4];
uniform vec3 positions[4];

uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float time;
uniform vec3 object_color;
uniform vec3 direct_light_position;
uniform vec3 lighthouse_position;
uniform vec3 lighthouse_color;
uniform int isLight;
uniform int isMoon;
uniform int isOcean;

uniform vec3 direction1;
uniform vec3 direction2;
uniform float cut_off;

// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;

vec3 get_point_light(vec3 position, vec3 object_color, vec3 light_color)
{
	vec3 L = normalize( position - world_position);
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	vec3 R = reflect( L, world_normal );

	vec3 ambient_light = light_color * material_kd * 0.25;
	vec3 diffuse_light = light_color * material_kd * max(dot(L, world_normal), 0);
	vec3 specular_light = light_color * material_ks * pow(max(dot(R, V), 0), material_shininess);

	float aten_fac = 1 / pow(length(position - world_position), 2) * 1.5;
	vec3 light = object_color * (ambient_light + aten_fac * (diffuse_light + specular_light));

	return light;
}

vec3 get_direct_light(vec3 position, vec3 object_color)
{
	vec3 L = normalize(position - world_position);
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	vec3 R = reflect( L, world_normal );

    float ambient_light = material_kd * 0.25;
    float diffuse_light = material_kd * max(dot(L, world_normal), 0);
    float specular_light = material_ks * pow(max(dot(R, V), 0), material_shininess);

	float aten_fac = 1 / pow(length(position - world_position), 2);
	vec3 light = object_color * (ambient_light + diffuse_light + specular_light);

	return light;
}

vec3 get_spot_light(vec3 position, vec3 object_color, vec3 light_color, vec3 direction, float cut_off)
{
    vec3 L = normalize(position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = reflect(L, world_normal);

    float ambient_light = material_kd * 0.25;
    float diffuse_light = material_kd * max(dot(L, world_normal), 0);
    float specular_light = material_ks * pow(max(dot(R, V), 0), material_shininess);

    float spot_light = dot(-L, direction);

    vec3 light;

	float spot_light_limit = cos(cut_off);
    float linear_att = (spot_light - spot_light_limit) / (1.0 - spot_light_limit);
    float light_att_factor = pow(linear_att, 2);
    float aten_fac = 1.0 / pow(length(direction - world_position), 2);

    if (spot_light > cos(cut_off))
    {
        light = light_color * object_color * (ambient_light + (light_att_factor + aten_fac) * (diffuse_light + specular_light));
    }
    else
    {
        light = light_color * ambient_light * object_color;
		light *= 0.1;
    }

    return light;
}

void main() {
    vec4 overall_color;

    if (hasTexture == 1) {
		if (time == -1)
		{
			overall_color = texture2D(texture_1, texcoord);
		}
		else
		{
			vec2 modulated_coords = vec2(sin(world_position.x * 5) + 1) * 0.05;
			overall_color = texture2D(texture_1, texcoord);
			overall_color += vec4(modulated_coords.x, modulated_coords.x, modulated_coords.x, 1);
		}
    }
	else
	{
		overall_color = vec4(object_color, 1);
	}

	vec3 x = overall_color.xyz;

	if (isMoon == 0)
	{
		vec3 light = get_direct_light(direct_light_position, vec3(1, 1, 1));
		overall_color += vec4(light, 1);

		for (int i = 0; i < 4; i++)
		{
			vec3 color = get_point_light(positions[i], colors[i], x);
			overall_color += vec4(color, 1);
		}

		if (isLight == 0)
		{
			vec3 lighthouse = get_point_light(lighthouse_position, overall_color.xyz, lighthouse_color);
			overall_color += vec4(lighthouse, 1);

			vec3 lighthouse1 = get_spot_light(lighthouse_position, overall_color.xyz, lighthouse_color, direction1, cut_off);
			overall_color += vec4(lighthouse1, 1);

			vec3 lighthouse2 = get_spot_light(lighthouse_position, overall_color.xyz, lighthouse_color, direction2, cut_off);
			overall_color += vec4(lighthouse2, 1);
		}
	}

    out_color = overall_color;
}