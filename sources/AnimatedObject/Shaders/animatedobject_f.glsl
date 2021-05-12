#version 330 core

/** PREPROCESSING **/

#define MAX_POINT_LIGHTS 15

/** IN & OUT VARIABLES **/

in vec2 textureCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec3 Tangent;
in vec3 Bitangent;
in mat3 TBN;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

#define LOW 1 
#define NORMAL 2
#define SHADOW 3

/** FUNCTIONS DEFINITIONS & STRUCT DECLARATIONS **/

vec3 computeDiffuseLight(vec3);
vec3 computeAmbientLight();
vec3 computePointLight();
float ShadowCalculation(vec4 fragPosLightSpace);

struct Light {
	vec3 lightDirection;
};

struct PointLight {
	vec3 position;
	vec3 color;
};

/** UNIFORMS VARIABLES **/

uniform vec3 cameraPosition;
uniform sampler2D Texture;
uniform sampler2D ShadowMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform int RenderType;

uniform Light light;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform float transparency;

/** MAIN FUNCTION **/

void main()
{
	if(RenderType == SHADOW) {
		return;
	}

	if(texture(Texture,textureCoords).a < 1.0)
		discard;

	vec3 normal = texture(normalMap, textureCoords).xyz;
	normal = normal * 2.0 - 1.0;   
	normal = normalize(TBN * normal);

	FragColor = vec4(normal,1.0);

	float shadow = ShadowCalculation(FragPosLightSpace);
	shadow = max(0,shadow);

	shadow = 0;
	
	FragColor = vec4(computeAmbientLight() + (1-shadow)*computeDiffuseLight(normal) /*+ computePointLight()*/,1.0);

	float brightness = dot(FragColor.xyz, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.7) {
		BrightColor = vec4(FragColor.xyz,1);
	} else {
		BrightColor = vec4(0,0,0,1);
	}
}

/** OTHER FUNCTIONS **/

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(ShadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	
	float shadow = 0.0; 
	float bias = 0.0006;

	if(dot(Normal,light.lightDirection) <= 0) {
		return 1.0;
	}
	
	if(projCoords.x < 0 || projCoords.x > 1){
		return 0.0f;
	} else if (projCoords.y < 0 || projCoords.y > 1) {
		return 0.0f;
	} else if (projCoords.z < 0 || projCoords.z > 1) {
		return 0.0f;
	}

	shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0; 

    return shadow;
}

vec3 computeAmbientLight()
{
	float ambientLight = 0.05;
	return ambientLight*texture(Texture,textureCoords).xyz;
}

vec3 computeDiffuseLight(vec3 normal)
{
	float diffuseLight = 0.2*max(dot(normalize(normal),normalize(light.lightDirection)),0.0);

	float specularStrength = 0.5;

	vec3 viewDir = normalize(cameraPosition - FragPos);
	vec3 reflectDir = normalize(reflect(-light.lightDirection, normal));

	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), 28);

	return diffuseLight* texture(Texture,textureCoords).xyz + spec * texture(specularMap,textureCoords).xyz;
}

vec3 computePointLight()
{
	vec3 outColor = vec3(0,0,0);

	for(int i = 0; i < 15;i++)
	{
		vec3 lightPosition = pointLights[i].position;
		vec3 direction = (lightPosition - FragPos);
	
		// Compute directional Light
		float directional = max(dot(normalize(direction),Normal),0.0);
		// Compute specular Light
		vec3 viewDir = normalize(cameraPosition - FragPos);
		vec3 reflectDir = normalize(reflect(-direction, Normal));
		float spec =  pow(max(dot(viewDir, reflectDir), 0.0), 28);

		// Compute attenuation
		float abstand =  length(direction);
		float attenuation = 1.0 / (1.0 + 0.7*abstand + 0.4*abstand*abstand);

		vec3 lightColor = pointLights[i].color;

		outColor += attenuation*(directional+1.5*spec)*lightColor;
	}

	return outColor * texture(Texture,textureCoords).xyz;
}
