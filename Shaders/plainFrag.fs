#version 410 core

out vec4 FragColor;

in vec3 normal;

uniform vec3 lightCol;
uniform vec3 lightDirection;
uniform vec3 objectCol;
float ambientFactor = 0.5f;

vec3 colour = vec3(0.2f,0.5f,0.6f);
void main()
{   
    vec3 normalise= normalize(normal);

    vec3 ambientColour=lightCol*objectCol*ambientFactor;

    float diffuseFactor=dot(normalise,-lightDirection);
    diffuseFactor=max(diffuseFactor,0.0f);
    vec3 diffuseColour=lightCol*objectCol*diffuseFactor;

    vec3 result=ambientColour+diffuseColour;
    FragColor = vec4(result,1.f);
}
