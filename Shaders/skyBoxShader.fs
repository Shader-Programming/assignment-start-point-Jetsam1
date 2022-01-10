#version 410 core
out vec4 FragColour;

in vec3 texCoords;

uniform samplerCube skybox;
//only needs to sample from cube map
void main()
{
	FragColour=texture(skybox,texCoords); 
}