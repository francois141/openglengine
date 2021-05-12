in vec2 textureCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec3 Tangent;
in vec3 Bitangent;
in mat3 TBN;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main()
{
	if(RenderType == NORMAL) {
		if(texture(albedoMap,textureCoords).a < 1.0)
			discard;

		vec3 normal;
		if(hasNormalMap) {
			normal = texture(normalMap, textureCoords).xyz;
			normal = normal * 2.0 - 1.0;   
			normal = normalize(TBN * normal);
		}
		else 
		{
			normal = Normal;
		}

		float shadow = ShadowCalculation(FragPosLightSpace);
		shadow = max(0,shadow);

		shadow = 0;
		
		FragColor = vec4(computeAmbientLight() + (1-shadow)*computeDiffuseLight(normal) + computePointLight(),1.0);

		float brightness = dot(FragColor.xyz, vec3(0.2126, 0.7152, 0.0722));
		if(brightness > 0.7) {
			BrightColor = vec4(FragColor.xyz,1);
		} else {
			BrightColor = vec4(0,0,0,1);
		}
	}
	else if(RenderType == LOW){
		FragColor = vec4(computeAmbientLight(),1.0);
		return;
	}
	else if(RenderType == SHADOW) {
		return;
	}
}




