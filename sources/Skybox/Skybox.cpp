#include "Skybox.h"

Skybox::Skybox(Camera *camera_in)
{
	camera = camera_in;
	
	skyboxShader = new Shader("../sources/Skybox/Shaders/skybox_v.glsl","../sources/Skybox/Shaders/skybox_f.glsl");
	
	loadTexture();

	glGenVertexArrays(1, &skyBoxVAO);
	glGenBuffers(1, &skyBoxVBO);
	glBindVertexArray(skyBoxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Skybox::draw()
{
	// Disable depth testing (we want to write the whole skybox)
	glDepthMask(GL_FALSE);

	// Setup & fire in the hole !
	skyboxShader->use();
	skyboxShader->setMat4("projection",camera->getProjectionMatrix());
	skyboxShader->setMat4("view",glm::mat4(glm::mat3(camera->getCamera())));
	glBindVertexArray(skyBoxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Enable again depth testing
	glDepthMask(GL_TRUE);
}

void Skybox::loadTexture()
{
	// Generate textures
	glGenTextures(1,&cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

	// Get path of the textures
	std::vector<std::string> textures_faces = { "../sources/Skybox/images/right.jpg","../sources/Skybox/images/left.jpg","../sources/Skybox/images/top.jpg",
									  "../sources/Skybox/images/bottom.jpg","../sources/Skybox/images/front.jpg","../sources/Skybox/images/back.jpg" };

	// Load them 
	int width, height, nrChannels;
	for (unsigned int i = 0; i < textures_faces.size(); i++)
	{
		auto data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
	}

	// Setup last parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}