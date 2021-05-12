#version 330 core

/** LIST OF PREPROCESSING STUFF **/
#define LOW 1 
#define NORMAL 2
#define SHADOW 3

#define MAX_POINT_LIGHTS 15
vec3  blendMaps();

/** FUNCTIONS DECLARATIONS **/

float computeDiffuseLight();
float computeAmbientLight();
vec3  computePointLight();

float ShadowCalculation(vec4 fragPosLightSpace);

/** STRUCT DECLARATIONS **/

struct Light {
	vec3 lightDirection;
};

struct PointLight {
	vec3 position;
	vec3 color;
};

/** IN & OUT VARIABLES **/ 

in vec2 textureCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

/** UNIFORM VARIABLES **/

uniform vec3 cameraPosition;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D blendmap;
uniform sampler2D ShadowMap;

uniform Light light;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int RenderType;

/** MAIN FUNCTION**/

void main()
{	
	if(RenderType == NORMAL || RenderType == LOW) 
	{
		float shadow = ShadowCalculation(FragPosLightSpace);
		FragColor = vec4(computeDiffuseLight() * (1 - shadow)* blendMaps() + computeAmbientLight() * blendMaps() + computePointLight() * blendMaps(),1.0);
		BrightColor = vec4(0,0,0,1); // No bloom on the terrain
	}
	else if(RenderType == SHADOW) {

	}
}

/** OTHER FUNCTIONS **/

vec3 blendMaps()
{
	float value1 = texture(blendmap,textureCoords).x;
	float value2 = texture(blendmap,textureCoords).y;
	float value3 = texture(blendmap,textureCoords).z;
	float value4 = max(1 - value1 - value2 - value3,0);

	vec3 color_out = vec3(0,0,0);

	vec2 textureCoordsTiles = 40 * textureCoords;

	color_out += texture(texture1,textureCoordsTiles).xyz;
	color_out += value2*texture(texture2,textureCoordsTiles).xyz;
	color_out += value3*texture(texture3,textureCoordsTiles).xyz;
	color_out += value4*texture(texture4,textureCoordsTiles).xyz;

	return color_out;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.x < 0 || projCoords.x > 1){
		return 0.0f;
	} else if (projCoords.y < 0 || projCoords.y > 1) {
		return 0.0f;
	} else if (projCoords.z < 0 || projCoords.z > 1) {
		return 0.0f;
	}

    float closestDepth = texture(ShadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

	float shadow = currentDepth - 0.003 >= closestDepth ? 1.0 : 0.0; 

    return shadow;
}

float computeDiffuseLight()
{
	float diffuseLight = 0.1*max(dot(normalize(Normal),normalize(light.lightDirection)),0.0);
	return diffuseLight;
}

float computeAmbientLight()
{
	float ambientLight = 0.1;
	return ambientLight;
}

vec3 computePointLight()
{
	vec3 outColor = vec3(0,0,0);

	for(int i = 0; i < 15;i++)
	{
		vec3 lightPosition = pointLights[i].position;
		vec3 direction = (lightPosition - FragPos);
	
		float intensity = max(dot(normalize(direction),Normal),0.0);
		float abstand =  length(direction);

		float attenuation = 1.0 / (1.0 + 0.7*abstand + 0.4*abstand*abstand);

		vec3 lightColor = pointLights[i].color;

		outColor += attenuation*intensity*lightColor;
	}

	return outColor;
}