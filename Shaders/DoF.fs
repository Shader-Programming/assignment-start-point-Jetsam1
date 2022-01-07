#version 410 core

out vec4 fragColour;

in vec2 uv;

uniform sampler2D image;
uniform sampler2D blur;
uniform sampler2D depth;
float aperture;

void main()
{
	vec2 centreUV=vec2(0.5,0.5);
	vec3 focus=texture(depth,centreUV).xyz;
	vec3 pos=texture(depth,uv).xyz;
	vec3 depthDist=(focus-pos);
	float dist=pow((depthDist.x+depthDist.y+depthDist.z),0.5);
//	vec3 mixed=mix(image,blur)
	
	
	fragColour=vec4(focus,1.0);
}