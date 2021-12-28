
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 perpBisector;
layout (location = 4) in vec2 UVcoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightDirection;
uniform vec3 viewPos;
mat3 invTBN;

out vec3 normal ;
out vec3 TanSpacepos;
out vec3 tanViewPos;
out vec3 tanLightDirection;
out vec3 WSpos;
//out vec3 WSnorm;
out vec2 uv;


void main()
{  
    gl_Position = projection * view * model*vec4(aPos,1.0);
    WSpos=(model * vec4(aPos,1.0)).xyz;
    vec3 T=normalize(vec3(model*vec4(tangent,0.0)));
    vec3 B=normalize(vec3(model*vec4(perpBisector,0.0)));
    vec3 N=normalize(vec3(model*vec4(aNormals,0.0)));
    mat3 TBN =transpose(mat3(T ,B ,N));
 //  normal=normal*TBN;
  TanSpacepos=TBN* vec3(model * vec4(aPos,1.0));
 //TanSpacepos=WSpos;
  tanLightDirection =TBN*lightDirection;
 //
// tanLightDirection =lightDirection;
  tanViewPos=TBN*viewPos;
    uv=UVcoords;
}