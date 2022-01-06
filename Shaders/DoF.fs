#version 410 core

out vec4 fragColour

in vec2 uv;

uniform sampler2D image;
uniform sampler2D blur;
uniform sampler2D depth;

void main()
{
	vec2 centreUV=vec2(0.5,0.5);
	vec3 focus=texture(depth,centreUV);
	vec3 pos=texture(depth,uv);
	vec3 depthDist=focus-pos;
	vec3 mixed=mix(image,blur,depthDist)
	
	
	fragColour=vec4(mixed,1.0);
}