#version 410 core

out vec4 fragColor;

uniform sampler2D image;

in vec2 uv;

void main()
{
	fragColor =texture(image,uv);
}