#version 330 core

ADD_MODULE(../sources/Shaders/modules/phong_include.glsl)
ADD_MODULE(../sources/Shaders/modules/classic_definitions.glsl)

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
    switch (textureType) 
    {
        case albedoTextureID:
            return hasAlbedoMap ? texture(albedoMap, coords).xyz : ambientColor;
        case specularTextureID:
            return hasSpecularMap ? texture(specularMap, coords).xyz : vec3(specularExponent, specularExponent, specularExponent);
        default:
            return vec3(0,0,0);
    }
}