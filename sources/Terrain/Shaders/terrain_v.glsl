#version 330 core

#define LOW 1 
#define NORMAL 2
#define SHADOW 3

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec2 textureCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

uniform int RenderType;

ADD_MODULE(../sources/Shaders/modules/position.glsl)

void main()
{
	if(RenderType == LOW || RenderType == NORMAL) {
		textureCoords = textureCoord;
		FragPos = vec3(model * vec4(aPos, 1.0));
		Normal = mat3(transpose(inverse(model))) * aNormal;  
		FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos,1.0);
	} 

	gl_Position = computePosition(lightSpaceMatrix,projection,view,model,aPos,RenderType); 
}


