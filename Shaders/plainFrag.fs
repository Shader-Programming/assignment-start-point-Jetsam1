#version 410 core

out vec4 FragColor;

in vec3 normal;
in vec2 uv;
in vec3 TanSpacepos;
in vec3 tanLightDirection;
in vec3 tanViewPos;
in vec3 WSPos;

vec3 GetDirectionalLight(vec3 norm,vec3 viewDir,vec3 lightDir,vec2 TexCoords);
vec3 GetPointLight(vec3 norm,vec3 viewDir,vec3 lightDir,vec2 TexCoords);
vec3 GetSpotLight(vec3 norm,vec3 viewDir,vec3 lightDir,vec2 TexCoords);
vec2 parallaxMapping(vec2 uv,vec3 viewDir);
vec2 SteepParallaxMapping(vec2 uv,vec3 viewDir);


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

uniform pointLight pLight;


struct spotLight
{
	vec3 pos;
	vec3 dir;
	vec3 ambCol;
	vec3 diffCol;
	vec3 specCol;
	float kC;
	float lC;
	float qC;

	float inRad;
	float outRad;

	float ambFac;
	float specShine;
	float specStrength;
};

uniform spotLight sLight;




uniform vec3 lightCol;



uniform float PXscale;
uniform sampler2D crateTex;
uniform sampler2D crateSpec;
uniform sampler2D crateNorm;
uniform sampler2D crateDisp;

uniform bool map;

float ambientFactor = 0.2f;
float shine = 200.f;
float specularStrength = 0.1f;
float Brightness=0.015f;
float sharpness =50.f;

//vec3 colour = vec3(0.2f,0.5f,0.6f);
void main()
{   

   vec3 norm =vec3(0.0);
   vec3 viewDir = (normalize(tanViewPos-TanSpacepos));
   vec2 texCoords=	SteepParallaxMapping(uv,viewDir);
   vec3 diffuse = texture(crateTex,texCoords).xyz;
   norm = texture(crateNorm,texCoords).xyz;
   
;
	vec3 result=vec3(0.0);
	//parallaxMapping(uv,viewDir);

	vec3 dirLightRes = GetDirectionalLight(norm,viewDir,tanLightDirection,texCoords);
	vec3 PointLightRes = GetPointLight(norm,viewDir,WSPos,texCoords);
	vec3 spotLightRes = GetSpotLight(norm,viewDir,sLight.dir,texCoords);

	//Rim Lighting
	float dp = dot(norm , viewDir);
	float Rim= (Brightness*pow((1-dp),sharpness));


	result = dirLightRes + PointLightRes + spotLightRes;

   FragColor = vec4(result,1.f);
   }

vec3 GetDirectionalLight(vec3 norm,vec3 viewDir,vec3 lightDir,vec2 TexCoords)
{
	vec3 diffCol= texture(crateTex,TexCoords).rgb;
	vec3 specmapcol = texture(crateSpec,TexCoords).xyz;
    vec3 ambientColour = lightCol * diffCol * ambientFactor;

    float diffuseFactor = dot(norm,-tanLightDirection);
    diffuseFactor = max(diffuseFactor,0.0f);
    vec3 diffuseColour = lightCol*diffCol*diffuseFactor;


   // vec3 reflectDir = reflect(tanLightDirection,norm);
   vec3 halfDir = normalize(tanLightDirection+viewDir);
   //float specularFactor = dot(viewDir,reflectDir);
   float specularFactor=dot(halfDir,norm);
  
  specularFactor = max(specularFactor,0.0);
   specularFactor = pow(specularFactor,shine);


   vec3 SpecColour = lightCol  * specmapcol * specularFactor*specularStrength;

   vec3 result = ambientColour + diffuseColour+ SpecColour;

   return result;
}

vec3 GetPointLight(vec3 norm,vec3 viewDir,vec3 FragPos,vec2 TexCoords)
{
	vec3 diffCol= texture(crateTex,TexCoords).rgb;

	vec3 specmapcol = texture(crateSpec,TexCoords).xyz;

   float dist=length(pLight.position-FragPos);
   float atten = 1.0/( pLight.kC + (pLight.lC * dist) + (pLight.qC * (dist * dist)));
   vec3 pLightDir = normalize( pLight.position - FragPos );

   vec3 ambCol = pLight.ambientCol * diffCol* pLight.ambFac;
   ambCol = ambCol * atten;

    float diffuseFactor = dot( norm , pLightDir );
    diffuseFactor = max( diffuseFactor , 0.0f );
    vec3 diffuseColour = pLight.diffuseCol * diffCol * diffuseFactor;
	diffuseColour = diffuseColour * atten;

	
	vec3 halfDir = normalize( pLightDir + viewDir );
	float specularFactor = dot( halfDir , norm );
	specularFactor = max( specularFactor , 0.0 );
	specularFactor = pow( specularFactor , pLight.specShine );
	vec3 SpecColour = pLight.specCol  * specmapcol * specularFactor;
	SpecColour = SpecColour * atten;

	vec3 pointlightRes= ambCol+diffuseColour+SpecColour;

	return pointlightRes;
}

vec3 GetSpotLight(vec3 norm,vec3 viewDir,vec3 FragPos,vec2 TexCoords)
{
	vec3 diffCol= texture(crateTex,TexCoords).rgb;
	vec3 specmapcol = texture(crateSpec,TexCoords).xyz;

   float dist=length(sLight.pos-FragPos);
   float atten = 1.0/( sLight.kC + (sLight.lC * dist) + (sLight.qC * (dist * dist)));
   vec3 sLightDir = normalize( sLight.pos - FragPos );

   vec3 ambCol = sLight.ambCol * diffCol * sLight.ambFac;
   ambCol = ambCol * atten;

    float diffuseFactor = dot( norm , sLightDir );
    diffuseFactor = max( diffuseFactor , 0.0f );
    vec3 diffuseColour = sLight.diffCol * diffCol * diffuseFactor;
	diffuseColour = diffuseColour * atten;

	
	vec3 halfDir = normalize( sLight.dir + viewDir );
	float specularFactor = dot( halfDir , norm );
	specularFactor = max( specularFactor , 0.0 );
	specularFactor = pow( specularFactor , sLight.specShine );                  
	vec3 SpecColour = sLight.specCol  * specmapcol * specularFactor;
	SpecColour = SpecColour * atten;
	

	float theta = dot(normalize( sLight.dir) ,-sLightDir );
	float denom = sLight.inRad -sLight.outRad;
	float illum =(theta -sLight.outRad)/denom;
	illum = clamp(illum ,0.0,1.0);
	ambCol=vec3(0);
	diffuseColour=diffuseColour*illum;
	SpecColour=SpecColour*illum;

	vec3 spotLightRes= ambCol+diffuseColour+SpecColour;

	return spotLightRes;
}

vec2 parallaxMapping(vec2 uv,vec3 viewDir)
{
	float height = texture(crateDisp,uv).r;
	return uv-(viewDir.xy)*(height *PXscale);
}

vec2 SteepParallaxMapping(vec2 uv,vec3 viewDir)
{
	float numLayers = 100;
	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;
	vec2 P = viewDir.xy * PXscale;
	vec2 deltaTexCoords = P /numLayers;
	vec2 currentTexCoords = uv;
	float currentDepthMapValue = texture(crateDisp,currentTexCoords).r;
	while(currentLayerDepth<currentDepthMapValue)
	{
		currentTexCoords -=deltaTexCoords;
		currentDepthMapValue = texture(crateDisp,currentTexCoords).r;
		currentLayerDepth+= layerDepth;
	}
	vec2 prevTexCoords= currentTexCoords +deltaTexCoords;

	float postDepth=currentDepthMapValue -currentLayerDepth;
	float preDepth=texture(crateDisp,prevTexCoords).r -currentLayerDepth +layerDepth;

	float weight=postDepth /(postDepth - preDepth);
	vec2 finalCoords= prevTexCoords *weight* currentTexCoords*(1.0 - weight);
	return finalCoords;
}
