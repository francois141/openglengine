#version 330 core

/** FUNCTIONS DEFINITIONS & STRUCT DECLARATIONS **/

vec3 computeDiffuseLight(vec3);
vec3 computeAmbientLight();
vec3 computePointLight();
vec3 retrieveProperty(int textureType,vec2 coords);
float ShadowCalculation(vec4 fragPosLightSpace);

struct Light {
	vec3 lightDirection;
};

struct PointLight {
	vec3 position;
	vec3 color;
};

/** Some definitions **/

#define LOW 1 
#define NORMAL 2
#define SHADOW 3

#define MAX_POINT_LIGHTS 15

#define albedoTextureID 1
#define specularTextureID 2
#define normalTextureID 3

uniform bool hasAlbedoMap;
uniform bool hasSpecularMap;
uniform bool hasNormalMap;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform float specularExponent;

uniform vec3 cameraPosition;

uniform sampler2D albedoMap;
uniform sampler2D ShadowMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform Light light;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int RenderType;
