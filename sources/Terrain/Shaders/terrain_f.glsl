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

/** UNIFORM VARIABLES **/
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D blendmap;

/** FUNCTIONS DECLARATION **/
vec3 blendMaps();

ADD_MODULE(../sources/Shaders/modules/phong.glsl)
ADD_MODULE(../sources/Shaders/modules/shadow.glsl)

/** MAIN FUNCTION**/

void main()
{	
	if(RenderType == SHADOW)
		return;

	FragColor = getPhongColor();
	BrightColor = vec4(0,0,0,1); // No bloom on the terrain
}

/** OTHER FUNCTIONS **/

vec3 blendMaps()
{
	float value1 = texture(blendmap,textureCoords).x;
	float value2 = texture(blendmap,textureCoords).y;
	float value3 = texture(blendmap,textureCoords).z;
	float value4 = max(1 - value1 - value2 - value3,0);

	vec3 color_out = vec3(0,0,0);

	vec2 textureCoordsTiles = 40 * textureCoords;

	color_out += texture(texture1,textureCoordsTiles).xyz;
	color_out += value2*texture(texture2,textureCoordsTiles).xyz;
	color_out += value3*texture(texture3,textureCoordsTiles).xyz;
	color_out += value4*texture(texture4,textureCoordsTiles).xyz;

	return color_out;
}

vec3 getAmbientColor() 
{
    return blendMaps();
}

vec3 getDiffuseColor() 
{
    return blendMaps(); 
}

vec3 getSpecularColor() 
{
    return blendMaps();
}