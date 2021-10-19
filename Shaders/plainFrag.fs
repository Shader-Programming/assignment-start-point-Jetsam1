#version 410 core

out vec4 FragColor;

in vec3 normal;
in vec3 WSpos;


uniform vec3 lightCol;
uniform vec3 lightDirection;
uniform vec3 objectCol;
uniform vec3 viewPos;
float ambientFactor = 0.5f;
float shine = 150.f;
float specularStrength = 0.2f;

//vec3 colour = vec3(0.2f,0.5f,0.6f);
void main()
{   
    vec3 norm = normalize(normal);

    vec3 ambientColour = lightCol * objectCol * ambientFactor;

    float diffuseFactor = dot(norm,-lightDirection);
    diffuseFactor = max(diffuseFactor,0.0f);
    vec3 diffuseColour = lightCol*objectCol*diffuseFactor;

   vec3 viewDir = (normalize(viewPos-WSpos));
   // vec3 reflectDir = reflect(lightDirection,norm);
   vec3 halfDir = normalize(lightDirection+viewDir);
   //float specularFactor = dot(viewDir,reflectDir);
   float specularFactor=dot(halfDir,norm);
  
  specularFactor = max(specularFactor,0.0);
   specularFactor = pow(specularFactor,shine);


   vec3 SpecColour = lightCol  * specularFactor * specularStrength;

   vec3 result = ambientColour + diffuseColour+ SpecColour;
   FragColor = vec4(result,1.f);
}
