#version 410 core

//out vec4 FragColor;

in vec3 normal;
in vec2 uv;
in vec3 TanSpacepos;
in vec3 tanLightDirection;
in vec3 tanViewPos;
in vec3 WSPos;
uniform mat4 lightSpaceMatrix;




layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 sceneBrightCol;

struct pointLight
{
	vec3 position;

	vec3 ambientCol;
	vec3 diffuseCol;
	vec3 specCol;

	float kC;
	float lC;
	float qC;

};
#define NR_POINT_LIGHTS 4
uniform pointLight pLight[NR_POINT_LIGHTS];


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


vec3 GetPointLight(pointLight light , vec3 norm , vec3 viewDir , vec3 FragPos  , vec2 texCoords);
vec3 GetDirectionalLight(vec3 norm,vec3 viewDir,vec3 lightDir,vec2 uv,float shadow);
vec3 GetSpotLight(vec3 norm,vec3 viewDir,vec3 FragPos);
vec2 parallaxMapping(vec2 uv,vec3 viewDir);
vec2 SteepParallaxMapping(vec2 uv,vec3 viewDir);
float calcShadow(vec4 lightSpacePos);

uniform vec3 lightCol;
uniform vec3 objectCol;
uniform sampler2D floorTex;
uniform sampler2D floorSpec;
uniform sampler2D floorNorm;
uniform sampler2D floorDisp;
uniform float PXscale;

uniform sampler2D shadowMap;

uniform bool map;
float ambientFactor = 0.5f;
float shine = 10.f;
float specularStrength = 0.05f;
float Brightness=0.015f;
float sharpness =50.f;

//vec3 colour = vec3(0.2f,0.5f,0.6f);
void main()
{   
    vec4 lightSpacePos = lightSpaceMatrix *vec4(WSPos,1.0);
	vec3 norm=vec3(0.0);
	float shadow=calcShadow(lightSpacePos);

	
	norm=texture(floorNorm,uv).xyz;
	
	vec3 viewDir = (normalize(tanViewPos-TanSpacepos));
	vec3 result=vec3(0.0);
	//parallaxMapping(uv,viewDir);
	vec2 texCoords = SteepParallaxMapping(uv,viewDir);
	vec3 dirLightRes = GetDirectionalLight(norm,viewDir,tanLightDirection,texCoords,shadow);
	for(int i=0;i<NR_POINT_LIGHTS;i++)
	{
	 result+= GetPointLight(pLight[i],norm,viewDir,TanSpacepos,texCoords);
	}
	vec3 spotLightRes = GetSpotLight(norm,viewDir,WSPos);

	//Rim Lighting
	float dp = dot(norm , viewDir);
	float Rim= (Brightness*pow((1-dp),sharpness));


	result += dirLightRes + spotLightRes ;

    FragColor = vec4(result,1.f);
   
    float sceneBright=max(max(result.x,result.y),result.z);
    if(sceneBright>0.8)
    {
		  sceneBrightCol=FragColor;
    }
    else
    {
		  sceneBrightCol=vec4(vec3(0.0),1.0);
    }
}

vec3 GetDirectionalLight(vec3 norm,vec3 viewDir,vec3 lightDir,vec2 uv,float shadow)
{	
	vec3 diffmapcol=texture(floorTex,uv).xyz;
	float specmapcol = texture(floorSpec,uv).x;
    vec3 ambientColour = lightCol * diffmapcol * ambientFactor;

    float diffuseFactor = dot(norm,tanLightDirection);
    diffuseFactor = max(diffuseFactor,1.0f);
    vec3 diffuseColour = lightCol*diffmapcol*diffuseFactor;

	// vec3 reflectDir = reflect(tanLightDirection,norm);
    vec3 halfDir = normalize(lightDir+viewDir);
    //float specularFactor = dot(viewDir,reflectDir);
    float specularFactor=dot(halfDir,norm);

    specularFactor = max(specularFactor,0.0);
    specularFactor = pow(specularFactor,shine);

    vec3 SpecColour = lightCol  * specmapcol * specularFactor*specularStrength;

    vec3 result = ambientColour + (1.0-shadow)*(diffuseColour + SpecColour);

    return result;
}



vec3 GetPointLight(pointLight light,vec3 norm,vec3 viewDir,vec3 FragPos,vec2 texCoords)
{
	vec3 diffmapcol=texture(floorTex,texCoords).xyz;
	vec3 specmapcol = texture(floorSpec,texCoords).xyz;

    float dist=length(light.position-FragPos);
    float atten = 1.0/( light.kC + (light.lC * dist) + (light.qC * (dist * dist)));

    vec3 pLightDir = normalize(light.position - FragPos );


    float diffuseFactor = dot( norm , pLightDir );
    diffuseFactor = max( diffuseFactor , 0.0f );
    // vec3 diffuseColour = pLight.diffuseCol * diffmapcol * diffuseFactor;
    //diffuseColour = diffuseColour * atten;
    
    
    vec3 halfDir = normalize( pLightDir + viewDir );
    float specularFactor = dot( halfDir , norm );
    specularFactor = max( specularFactor , 0.0 );
    specularFactor = pow( specularFactor , shine );
    //vec3 SpecColour = pLight.specCol  * specmapcol * specularFactor;
    //SpecColour = SpecColour * atten;
    
    vec3 ambient = light.ambientCol *diffmapcol;
    vec3 diffuse = light.diffuseCol*diffuseFactor*diffmapcol;
    vec3 specular = light.specCol*specularFactor*specmapcol;
    
    ambient*=atten;
    diffuse*=atten;
    specular*=atten;
    vec3 pointlightRes= ambient+diffuse+specular;
    return pointlightRes;
}

vec3 GetSpotLight(vec3 norm,vec3 viewDir,vec3 FragPos)
{
	vec3 diffmapcol=texture(floorTex,uv).xyz;
	float specmapcol = texture(floorSpec,uv).x;

    float dist=length(sLight.pos-FragPos);
    float atten = 1.0/( sLight.kC + (sLight.lC * dist) + (sLight.qC * (dist * dist)));
    vec3 sLightDir = normalize( sLight.pos - FragPos );

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

vec2 SteepParallaxMapping(vec2 uv,vec3 viewDir)
{
	float numLayers = 10;
	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;
	vec2 P = viewDir.xy * PXscale;
	vec2 deltaTexCoords = P /numLayers;
	vec2 currentTexCoords = uv;
	float currentDepthMapValue = texture(floorDisp,currentTexCoords).r;
	while(currentLayerDepth<currentDepthMapValue)
	{
		currentTexCoords -=deltaTexCoords;
		currentDepthMapValue = texture(floorDisp,currentTexCoords).r;
		currentLayerDepth+= layerDepth;
	}
	vec2 prevTexCoords= currentTexCoords +deltaTexCoords;

	float postDepth=currentDepthMapValue -currentLayerDepth;
	float preDepth=texture(floorDisp,prevTexCoords).r -currentLayerDepth +layerDepth;

	float weight=postDepth /(postDepth - preDepth);
	vec2 finalCoords= prevTexCoords *weight+ currentTexCoords*(1.0 - weight);
	return finalCoords;
}


float calcShadow(vec4 lightSpacePos)
{
	vec3 projCoords = lightSpacePos.xyz/lightSpacePos.w;

	projCoords=projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap,projCoords.xy).r;

	float currentDepth = projCoords.z;

	float shadow =0;
	if(currentDepth>closestDepth) shadow=1;
	float bias=0.015;
	vec2 texelSize=1.0/textureSize(shadowMap,0);
	for(int i=-1;i<3;i++)
	{
		for(int j=-1;j<3;j++)
		{
			float pcf =texture(shadowMap,projCoords.xy+vec2(i,j)*texelSize).r;
			if(currentDepth-bias>pcf)
			{
				shadow+=1;
			}
		}
	}
	shadow=shadow/16;
	if(projCoords.z>1.0)
	{
		shadow =0.0;
	}
	return shadow;
}