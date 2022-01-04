#version 410 core

out vec4 fragColour;

uniform sampler2D image;

in vec2 uv;

void main()
{
	fragColour =texture(image,uv);
}