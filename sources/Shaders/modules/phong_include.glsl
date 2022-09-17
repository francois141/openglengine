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
