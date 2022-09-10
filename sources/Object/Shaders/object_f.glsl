#version 330 core

/** FUNCTIONS DEFINITIONS & STRUCT DECLARATIONS **/

vec3 computeDiffuseLight(vec3);
vec3 computeAmbientLight();
vec3 computePointLight();
vec3 retrieveProperty(int textureType, vec2 coords);
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
    if (RenderType == NORMAL) {
        if (texture(albedoMap, textureCoords).a < 1.0)
        discard;

        vec3 normal;
        if (hasNormalMap) {
            normal = texture(normalMap, textureCoords).xyz;
            normal = normal * 2.0 - 1.0;
            normal = normalize(TBN * normal);
        }
        else
        {
            normal = Normal;
        }

        float shadow = ShadowCalculation(FragPosLightSpace);
        shadow = max(0, shadow);

        shadow = 0;

        FragColor = vec4(computeAmbientLight() + (1-shadow)*computeDiffuseLight(normal) + computePointLight(), 1.0);

        float brightness = dot(FragColor.xyz, vec3(0.2126, 0.7152, 0.0722));
        if (brightness > 0.7) {
            BrightColor = vec4(FragColor.xyz, 1);
        } else {
            BrightColor = vec4(0, 0, 0, 1);
        }
    }
    else if (RenderType == LOW){
        FragColor = vec4(computeAmbientLight(), 1.0);
        return;
    }
    else if (RenderType == SHADOW) {
        return;
    }
}


/** Compute if Object is in the shadow right now **/
float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    float bias = 0.0006;

    if (dot(Normal, light.lightDirection) <= 0) {
        return 1.0;
    }

    if (projCoords.x < 0 || projCoords.x > 1){
        return 0.0f;
    } else if (projCoords.y < 0 || projCoords.y > 1) {
        return 0.0f;
    } else if (projCoords.z < 0 || projCoords.z > 1) {
        return 0.0f;
    }

    shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

/** Return the ambient Light **/
vec3 computeAmbientLight()
{
    float ambientLight = 0.05;
    return ambientLight*retrieveProperty(albedoTextureID, textureCoords);
}

/** Return the diffuse Liht **/
vec3 computeDiffuseLight(vec3 normal)
{
    float diffuseLight = 0.2*max(dot(normalize(normal), normalize(light.lightDirection)), 0.0);

    float specularStrength = 0.5;

    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = normalize(reflect(-light.lightDirection, normal));

    float spec =  pow(max(dot(viewDir, reflectDir), 0.0), 28);

    return diffuseLight * retrieveProperty(albedoTextureID, textureCoords) + spec * retrieveProperty(specularTextureID, textureCoords);
}

/** Compute the values of the points lights **/
vec3 computePointLight()
{
    vec3 outColor = vec3(0, 0, 0);

    for (int i = 0; i < 15;i++)
    {
        vec3 lightPosition = pointLights[i].position;
        vec3 direction = (lightPosition - FragPos);

        // Compute directional Light
        float directional = max(dot(normalize(direction), Normal), 0.0);
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

    return outColor * retrieveProperty(albedoTextureID, textureCoords);//texture(albedoMap,textureCoords).xyz;
}

/** Retrieve the color of a given property **/
vec3 retrieveProperty(int textureType, vec2 coords)
{
    vec3 color = vec3(0, 0, 0);
    if (textureType == albedoTextureID) {
        if (hasAlbedoMap)
        {
            color = texture(albedoMap, coords).xyz;
        }
        else
        {
            color = ambientColor;// Change for ALBEDO COLOR SOON
        }
    }
    else if (textureType == specularTextureID) {
        if (hasSpecularMap)
        {
            color = texture(specularMap, coords).xyz;
        }
        else
        {
            color = vec3(specularExponent, specularExponent, specularExponent);
        }
    }

    return color;
}