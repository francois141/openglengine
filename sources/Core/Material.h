#ifndef MATERIAL_H
#define MATERIAL_H


class Material {
	public:
	Material()
	{
		hasSpecular = false;
		hasAlbedo = false;
		hasNormal = false;

		ambientColor = glm::vec3(0,0,0);
		diffuseColor = glm::vec3(0,0,0);
		specularColor = glm::vec3(0,0,0);

		specularExponent = 0;

		albedoTextureID = 0;
		specularTextureID = 0;
		normalTextureID = 0;

	}

	bool hasSpecular = false;
	bool hasAlbedo = false;
	bool hasNormal = false;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularExponent;

	unsigned int albedoTextureID;
	unsigned int specularTextureID;
	unsigned int normalTextureID;
};

#endif