
/** Function that sum all components together **/
vec4 getPhongColor() 
{
    vec3 phongColor = vec3(0,0,0);

    float shadowReductionFactor = 1-ShadowCalculation(FragPosLightSpace);
    vec3 normal = getNormalVector();

    phongColor += computeAmbientIntensity() * getAmbientColor();
    phongColor += computeDiffuseIntensity(normal) * getDiffuseColor() * shadowReductionFactor;
    phongColor += computeSpecularIntensity(normal) * getSpecularColor() * shadowReductionFactor;

    phongColor += computePointLight();

    return vec4(phongColor,1.0); 
}

/** Get the bright color **/ 
vec4 getBrightColor(vec4 baseColor) 
{
    return dot(baseColor.xyz, vec3(0.2126, 0.7152, 0.0722)) > 0.7 ? baseColor : vec4(0,0,0,1);
}

/** Compute the values of the points lights **/
vec3 computePointLight()
{
    vec3 outColor = vec3(0, 0, 0);

    for (int i = 0; i < 15;i++)
    {
        // Get distance & direction
        vec3 lightPosition = pointLights[i].position;
        vec3 direction = (lightPosition - FragPos);
        float distance =  length(direction);

        // Compute directional Light
        float directionalIntensity = max(dot(normalize(direction), Normal), 0.0);

        // Compute specular Light
        vec3 viewDir = normalize(cameraPosition - FragPos);
        vec3 reflectDir = normalize(reflect(-direction, Normal));
        float specularIntensity =  pow(max(dot(viewDir, reflectDir), 0.0), 28);

        // Compute attenuation
        float attenuation = 1.0 / (1.0 + 0.7*distance + 0.4*distance*distance);
        vec3 lightColor = pointLights[i].color;

        outColor += attenuation*(directionalIntensity*getDiffuseColor()+1.5*specularIntensity*getSpecularColor())*lightColor;
    }

    return outColor;
}

/** Return the ambient Light **/
float computeAmbientIntensity()
{
    float ambientLight = 0.05;
    return ambientLight;
}

/** Return the diffuse Liht **/
float computeDiffuseIntensity(vec3 normal)
{
    float diffuseLight = 0.2*max(dot(normalize(normal), normalize(light.lightDirection)), 0.0);

    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = normalize(reflect(-light.lightDirection, normal));

    return diffuseLight;
}

/** Return the specular intensity **/
float computeSpecularIntensity(vec3 normal)
{
    float specularStrength = 0.5;

    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = normalize(reflect(-light.lightDirection, normal));

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 28);
    return spec;
}

/** Return the normal vector **/
vec3 getNormalVector() 
{
    vec3 normal;
    if (hasNormalMap) 
    {
        normal = texture(normalMap, textureCoords).xyz;
        normal = normal * 2.0 - 1.0;
        normal = normalize(TBN * normal);
    }
    else
    {
        normal = Normal;
    }

    return normal;
}
 
