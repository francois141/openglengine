uniform sampler2D ShadowMap;

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

    return max(0, shadow);
}


