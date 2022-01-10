#version 410 core

out vec4 fragColour;



in vec2 uv;

uniform sampler2D image;
uniform sampler2D bloomBlur;
void main()
{
	vec3 hdrColour=texture(image,uv).rgb;
	vec3 bloomCol=texture(bloomBlur,uv).rgb;
	hdrColour+=bloomCol;

	vec3 rein=hdrColour/(hdrColour+vec3(1.0));//reinhard
	fragColour=vec4(rein,1.0);

}