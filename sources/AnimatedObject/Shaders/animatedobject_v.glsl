#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 textureCoord;
layout(location = 1) in vec3 aNormal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;

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

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

#define LOW 1 
#define NORMAL 2
#define SHADOW 3

uniform int RenderType;

ADD_MODULE(../sources/Shaders/modules/position.glsl)

void main()
{
	vec4 totalPosition = vec4(0.0f);
	for(int i = 0; i < 4;i++)
	{
		if(boneIds[i] == -1)
		{
			continue;
		}

		if(boneIds[i] >= 100)
		{
			totalPosition = projection * view * model * vec4(aPos.xy,10.0f,1.0f);
			break;
		}
		vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
		totalPosition += localPosition * weights[i];
	}

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

		FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos,1.0);
	}

	gl_Position = computePosition(lightSpaceMatrix,projection,view,model.totalPosition,RenderType);

}

