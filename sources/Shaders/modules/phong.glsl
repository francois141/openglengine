
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

