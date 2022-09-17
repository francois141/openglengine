#version 330 core

ADD_MODULE(../sources/Shaders/modules/phong_include.glsl)

/** DATA STRUCTURES **/

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

in vec2 textureCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec3 Tangent;
in vec3 Bitangent;
in mat3 TBN;


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

ADD_MODULE(../sources/Shaders/modules/phong.glsl)
ADD_MODULE(../sources/Shaders/modules/shadow.glsl)


void main()
{
    if (texture(albedoMap, textureCoords).a < 1.0)
        discard;
    
    if(RenderType == SHADOW)
        return;
        
    vec4 outputColor = getPhongColor();

    FragColor = outputColor;
    BrightColor = getBrightColor(outputColor);
}

vec3 getAmbientColor() {
    return _retrieveProperty(albedoTextureID, textureCoords);
}

vec3 getDiffuseColor() {
    return _retrieveProperty(albedoTextureID, textureCoords); 
}

vec3 getSpecularColor() {
    return _retrieveProperty(specularTextureID, textureCoords);
}

vec3 _retrieveProperty(int textureType, vec2 coords)
{
    switch (textureType) {
        case albedoTextureID:
            return hasAlbedoMap ? texture(albedoMap, coords).xyz : ambientColor;
        case specularTextureID:
            return hasSpecularMap ? texture(specularMap, coords).xyz : vec3(specularExponent, specularExponent, specularExponent);
        default:
            return vec3(0,0,0);
    }
}
