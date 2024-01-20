#version 330 core

in vec2 texcoord;

uniform sampler2D text;
uniform vec4 fcolor;

layout(location = 0) out vec4 color;

void main() 
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texcoord).r);
    color = vec4(fcolor.xyz, 1.0) * sampled;
}
