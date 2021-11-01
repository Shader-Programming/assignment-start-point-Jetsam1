#version 410 core

out vec4 FragColor;

in vec3 normal;
in vec3 WSpos;

vec3 GetDirectionalLight(vec3 norm,vec3 viewDir);
struct pointLight
{
	vec3 position;
	vec3 ambientCol;
	vec3 diffuseCol;
	vec3 specCol;
	float kC;
	float lC;
	float qC;

	float ambFac;
	float specShine;
	float specStrength;
};


uniform vec3 lightCol;
uniform vec3 lightDirection;
uniform vec3 objectCol;
uniform vec3 viewPos;
uniform pointLight pLight;
float ambientFactor = 0.5f;
float shine = 150.f;
float specularStrength = 0.2f;



//vec3 colour = vec3(0.2f,0.5f,0.6f);
void main()
{   
    vec3 norm = normalize(normal);
	vec3 viewDir = (normalize(viewPos-WSpos));
	vec3 result=vec3(0.0);
   result=GetDirectionalLight(norm,viewDir);
   //pLight 

   float dist=length(pLight.position-WSpos);
   float atten = 1.0/( pLight.kC + (pLight.lC * dist) + (pLight.qC * (dist * dist)));
   vec3 pLightDir = normalize( pLight.position - WSpos );

   vec3 ambCol = pLight.ambientCol * objectCol * pLight.ambFac;
   ambCol = ambCol * atten;

    float diffuseFactor = dot( norm , pLightDir );
    diffuseFactor = max( diffuseFactor , 0.0f );
    vec3 diffuseColour = pLight.diffuseCol * objectCol * diffuseFactor;
	diffuseColour = diffuseColour * atten;

	
	vec3 halfDir = normalize( pLightDir + viewDir );
	float specularFactor = dot( halfDir , norm );
	specularFactor = max( specularFactor , 0.0 );
	specularFactor = pow( specularFactor , pLight.specShine );
	vec3 SpecColour = pLight.specCol  * specularFactor * pLight.specStrength;
	SpecColour = SpecColour * atten;

	vec3 pointlightRes= ambCol+diffuseColour+SpecColour;

	result=result+pointlightRes;


   FragColor = vec4(result,1.f);
}

vec3 GetDirectionalLight(vec3 norm,vec3 viewDir)
{
    vec3 ambientColour = lightCol * objectCol * ambientFactor;

    float diffuseFactor = dot(norm,-lightDirection);
    diffuseFactor = max(diffuseFactor,0.0f);
    vec3 diffuseColour = lightCol*objectCol*diffuseFactor;


   // vec3 reflectDir = reflect(lightDirection,norm);
   vec3 halfDir = normalize(lightDirection+viewDir);
   //float specularFactor = dot(viewDir,reflectDir);
   float specularFactor=dot(halfDir,norm);
  
  specularFactor = max(specularFactor,0.0);
   specularFactor = pow(specularFactor,shine);


   vec3 SpecColour = lightCol  * specularFactor * specularStrength;

   vec3 result = ambientColour + diffuseColour+ SpecColour;

   return result;
}
