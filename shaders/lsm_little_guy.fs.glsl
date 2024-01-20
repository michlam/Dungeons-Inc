#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform float fade_factor;
uniform vec2 u_resolution;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	if (gl_FragCoord.y < 100.0*u_resolution.y/800) {
		color = vec4(fcolor, max(0, (gl_FragCoord.y*800/u_resolution.y-40.0)*u_resolution.y/800/(100.0-40.0))) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	} else if (gl_FragCoord.y > 550.0*u_resolution.y/800) {
		color = vec4(fcolor, max(0, (650.0 - gl_FragCoord.y*800/u_resolution.y)*u_resolution.y/800/(650.0 - 550.0))) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	} else {
		color = vec4(fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	}

	color.a *= fade_factor;
}
