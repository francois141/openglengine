#version 330 core

#define LOW 1 
#define NORMAL 2
#define SHADOW 3


layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 textureCoord;
layout(location = 1) in vec3 aNormal;

out vec2 textureCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

uniform int RenderType;

void main()
{
	if(RenderType == LOW || RenderType == NORMAL) {
		textureCoords = textureCoord;
		FragPos = vec3(model * vec4(aPos, 1.0));
		Normal = mat3(transpose(inverse(model))) * aNormal;  
		gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
		FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos,1.0);
	} else if(RenderType == SHADOW) {
		gl_Position = lightSpaceMatrix * model * vec4(aPos,1.0);
	}

}

