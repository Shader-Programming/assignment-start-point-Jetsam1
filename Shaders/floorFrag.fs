#version 410 core

out vec4 FragColor;

in vec3 normal;
in vec3 TanSpacepos;
in vec2 uv;
in vec3 tanLightDirection;

vec3 GetDirectionalLight(vec3 norm,vec3 viewDir);
vec3 GetPointLight(vec3 norm,vec3 viewDir);
vec3 GetSpotLight(vec3 norm,vec3 viewDir);
vec2 parallaxMapping(vec2 uv,vec3 viewDir);


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

uniform vec3 objectCol;
uniform vec3 tanViewPos;
uniform sampler2D floorTex;
uniform sampler2D floorSpec;
uniform sampler2D floorNorm;
uniform sampler2D floorDisp;
uniform float PXscale;

uniform bool map;
float ambientFactor = 0.5f;
float shine = 150.f;
float specularStrength = 0.2f;
float Brightness=0.15f;
float sharpness =300.f;

//vec3 colour = vec3(0.2f,0.5f,0.6f);
void main()
{   
	vec3 norm=vec3(0.0);
	if(map==true)
	{
	
		norm=texture(floorNorm,uv).xyz;
		norm=norm*2 -1;
		norm=normalize(norm);
	}
	else
	{
		vec3 norm = normalize(normal);
	}
	vec3 viewDir = (normalize(tanViewPos-TanSpacepos));
	vec3 result=vec3(0.0);
	parallaxMapping(uv,viewDir);
	vec3 dirLightRes = GetDirectionalLight(norm,viewDir);
	vec3 PointLightRes = GetPointLight(norm,viewDir);
	vec3 spotLightRes = GetSpotLight(norm,viewDir);

	//Rim Lighting
	float dp = dot(norm , viewDir);
	float Rim= (Brightness*pow((1-dp),sharpness));


	result = dirLightRes + PointLightRes + spotLightRes +Rim;

   FragColor = vec4(result,1.f);
}

vec3 GetDirectionalLight(vec3 norm,vec3 viewDir)
{	
	vec3 diffmapcol=texture(floorTex,uv).xyz;
	vec3 specmapcol = texture(floorSpec,uv).xyz;
    vec3 ambientColour = lightCol * diffmapcol * ambientFactor;

    float diffuseFactor = dot(norm,-tanLightDirection);
    diffuseFactor = max(diffuseFactor,0.0f);
    vec3 diffuseColour = lightCol*diffmapcol*diffuseFactor;

	// vec3 reflectDir = reflect(tanLightDirection,norm);
    vec3 halfDir = normalize(tanLightDirection+viewDir);
    //float specularFactor = dot(viewDir,reflectDir);
    float specularFactor=dot(halfDir,norm);

    specularFactor = max(specularFactor,0.0);
    specularFactor = pow(specularFactor,shine);


    vec3 SpecColour = lightCol  * specmapcol * specularFactor;

    vec3 result = ambientColour + diffuseColour+ SpecColour;

    return result;
}



vec3 GetPointLight(vec3 norm,vec3 viewDir)
{
	vec3 diffmapcol=texture(floorTex,uv).xyz;
	vec3 specmapcol = texture(floorSpec,uv).xyz;

   float dist=length(pLight.position-TanSpacepos);
   float atten = 1.0/( pLight.kC + (pLight.lC * dist) + (pLight.qC * (dist * dist)));
   vec3 pLightDir = normalize( pLight.position - TanSpacepos );

   vec3 ambCol = pLight.ambientCol * diffmapcol* pLight.ambFac;
   ambCol = ambCol * atten;

    float diffuseFactor = dot( norm , pLightDir );
    diffuseFactor = max( diffuseFactor , 0.0f );
    vec3 diffuseColour = pLight.diffuseCol * diffmapcol * diffuseFactor;
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

vec3 GetSpotLight(vec3 norm,vec3 viewDir)
{
	vec3 diffmapcol=texture(floorTex,uv).xyz;
	vec3 specmapcol = texture(floorSpec,uv).xyz;

   float dist=length(sLight.pos-TanSpacepos);
   float atten = 1.0/( sLight.kC + (sLight.lC * dist) + (sLight.qC * (dist * dist)));
   vec3 sLightDir = normalize( sLight.pos - TanSpacepos );

   vec3 ambCol = sLight.ambCol * objectCol * sLight.ambFac;
   ambCol = ambCol * atten;

    float diffuseFactor = dot( norm , sLightDir );
    diffuseFactor = max( diffuseFactor , 0.0f );
    vec3 diffuseColour = sLight.diffCol * diffmapcol * diffuseFactor;
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
	float height = texture(floorDisp,uv).r;
	return uv-(viewDir.xy)*(height *PXscale);
}
