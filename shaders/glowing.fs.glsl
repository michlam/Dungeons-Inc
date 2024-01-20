#version 330

uniform float time;

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;

// Output color
layout(location = 0) out  vec4 color;


vec4 color_shift(vec4 in_color) 
{
	in_color += vec4(1, 1, 0, 0)*(0.2*cos(3*time) + 0.3);

	return in_color;
}

void main()
{
	color = vec4(fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	color = color_shift(color);
}