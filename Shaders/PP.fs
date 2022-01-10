#version 410 core

out vec4 fragColour;

uniform sampler2D image;

in vec2 uv;

void main()
{
	vec3 result=texture(image,uv).rgb;
	result=result/(result+vec3(1.0f));//reinhard
	fragColour=vec4(result,1.0);
}