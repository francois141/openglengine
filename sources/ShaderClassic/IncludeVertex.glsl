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


