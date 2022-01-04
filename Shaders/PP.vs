#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoords;


out vec2 uv;

void main()
{
	uv=texCoords;
	gl_Position=vec4(aPos,1.0);
}