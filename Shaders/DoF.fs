#version 410 core

out vec4 fragColour

in vec2 uv;

uniform sampler2D image;
uniform sampler2D blur;
uniform sampler2D depth;
float aperture;

void main()
{
	vec2 centreUV=vec2(0.5,0.5);
	vec3 focus=texture(depth,centreUV);
	vec3 pos=texture(depth,uv);
	vec3 depthDist=(focus-pos);
	vec3 blurring=depthDist/2.0;
	vec3 mixed=mix(image,blurring)
	
	
	fragColour=vec4(image,1.0);
}