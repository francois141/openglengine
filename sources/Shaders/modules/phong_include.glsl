/** Implemented functions **/
vec4 getPhongColor();
vec4 getBrightColor(vec4 baseColor);
vec3 computePointLight();

float computeAmbientIntensity();
float computeDiffuseIntensity(vec3 normal);
float computeSpecularIntensity(vec3 normal);

float ShadowCalculation(vec4 fragPosLightSpace);
vec3 getNormalVector();

/** Functions that must be implemened in the drawable **/
vec3 getAmbientColor();
vec3 getDiffuseColor();
vec3 getSpecularColor();
vec3 _retrieveProperty(int textureType, vec2 coords);

// Constants
#define LOW 1 
#define NORMAL 2
#define SHADOW 3

#define MAX_POINT_LIGHTS 15

#define albedoTextureID 1
#define specularTextureID 2
#define normalTextureID 3

// Light data structure
struct Light {
	vec3 lightDirection;
};

struct PointLight {
	vec3 position;
	vec3 color;
};

uniform Light light;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

// Uniform declarations
uniform bool hasAlbedoMap;
uniform bool hasSpecularMap;
uniform bool hasNormalMap;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;


uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform float specularExponent;

