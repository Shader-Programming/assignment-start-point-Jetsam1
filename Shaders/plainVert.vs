#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 perpBisector;
layout (location = 4) in vec2 UVcoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightDirection;



out vec3 normal ;


out mat3 TBN;
out vec3 WSPos;
out vec2 uv;


void main()
{  
    gl_Position = projection * view * model*vec4(aPos,1.0);
    WSPos= (model * vec4(aPos,1.0)).xyz; 
    normal=(model*vec4(aNormals,0.0)).xyz;
    vec3 T=(model*vec4(tangent,0.0)).xyz;
    vec3 B=(model*vec4(perpBisector,0.0)).xyz;
    TBN =mat3(T ,B ,normal);
   TBN = transpose(TBN);


    uv=UVcoords;
}