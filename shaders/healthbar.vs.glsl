#version 330

// Input attributes
in vec3 in_position;
in vec2 in_texcoord;

// Passed to fragment shader
out vec2 texcoord;


// Application data
uniform float z_value;
uniform mat3 transform;
uniform mat3 projection;
uniform float health;


void main()
{
	texcoord = in_texcoord;
	texcoord.y += (6.f - health) / 7.f;

	vec3 pos = projection * transform * vec3(in_position.xy, 1.0);
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}