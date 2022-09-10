#version 330 core

#define LOW 1
#define NORMAL 2
#define SHADOW 3

uniform bool hasAlbedoMap;
uniform bool hasSpecularMap;
uniform bool hasNormalMap;

uniform vec3 albedoColor;
uniform float specularValue;

uniform vec3 cameraPosition;
uniform sampler2D albedoMap;
uniform sampler2D ShadowMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 textureCoord;
layout(location = 1) in vec3 aNormal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec2 textureCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out vec3 Tangent;
out vec3 Bitangent;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform int RenderType;

void main()
{

	if(RenderType == LOW || RenderType == NORMAL) {
		Tangent = tangent;
		Bitangent = bitangent;

		vec3 T = normalize(vec3(model * vec4(Tangent,0.0)));
		vec3 B = normalize(vec3(model * vec4(Bitangent,0.0)));
		vec3 N = normalize(vec3(model * vec4(aNormal,0.0)));
		TBN = mat3(T,B,N);

		textureCoords = textureCoord;
		FragPos = vec3(model * vec4(aPos, 1.0));
		Normal = mat3(transpose(inverse(model))) * aNormal;  
		gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
		FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos,1.0);
		return;
	}
	else {
		gl_Position = lightSpaceMatrix * model * vec4(aPos,1.0);
	}

}

