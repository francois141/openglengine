#include "AnimatedObject.h"
	
AnimatedObject::AnimatedObject(Camera* camera_in, Light* light_in, string obj_path, string texture_path,MasterRenderer *renderer_in)
{
	shader = new Shader("../sources/AnimatedObject/Shaders/animatedobject_v.glsl", "../sources/AnimatedObject/Shaders/animatedobject_f.glsl");
	shader_shadow = new Shader("../sources/AnimatedObject/Shaders/shadow_v.glsl","../sources/AnimatedObject/Shaders/shadow_f.glsl");
	shader_lowquality = new Shader("../sources/AnimatedObject/Shaders/animatedobject_low_v.glsl","../sources/AnimatedObject/Shaders/animatedobject_low_f.glsl");
	
	camera = camera_in;
	light = light_in;
	renderer = renderer_in;

	loader = AnimatedMeshLoader(obj_path);
	nbMeshes = loader.getNbMeshes();

	for(int i = 0; i < nbMeshes;i++) {
		meshData.push_back(loader.getData(i));
		meshIndices.push_back(loader.getIndices(i));
		createBuffers(i);
	}

	for(int i = 0; i < loader.getData(0).size();i++)
	{
		int *bonesID = loader.getData(0)[i].m_BoneIDs;
	}
	vector<AnimatedTexture> textures = loader.getTextures();
	for(int i = 0; i < textures.size();i++)
	{
		std::cout << "There is a texture" << std::endl;
		if(textures.at(i).type == "texture_diffuse") {
			textureID = textures.at(i).id;
		}
		if(textures.at(i).type == "texture_specular") {
			specularTexture = textures.at(i).id;
		}
		if(textures.at(i).type == "texture_normal") {
			normalMap = textures.at(i).id;
		}
 	}
}

void AnimatedObject::draw()
{
	for(int i = 0; i < nbMeshes;i++) {
		bind(i);
		passToShader(NORMAL);
		glDrawElements(GL_TRIANGLES,meshIndices[i].size(),GL_UNSIGNED_INT,(void*)0);
	}
	unbind();
}

void AnimatedObject::drawShadow()
{
	for(int i = 0; i < nbMeshes;i++) {
		bind(i);
		passToShader(SHADOW);
		glDrawElements(GL_TRIANGLES,meshIndices[i].size(),GL_UNSIGNED_INT,(void*)0);
	}
	unbind();
}

void AnimatedObject::drawLowQuality() 
{
	for(int i = 0; i < nbMeshes;i++) {
		bind(i);
		passToShader(LOW);
		glDrawElements(GL_TRIANGLES,meshIndices[i].size(),GL_UNSIGNED_INT,(void*)0);
	}
	unbind();
}


void AnimatedObject::createBuffers(int index)
{
	unsigned int VAO,VBO,EBO;
	// Create VAO
	glGenVertexArrays(1, &VAO);
	// Create VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AnimatedVertex) * meshData[index].size(), &meshData[index][0], GL_STATIC_DRAW);
	// Create EBO
	glGenBuffers(1,&EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,meshIndices[index].size()*sizeof(unsigned int), &meshIndices[index][0],GL_STATIC_DRAW);

	VAOS.push_back(VAO);
	VBOS.push_back(VBO);
	EBOS.push_back(EBO);
}

void AnimatedObject::bind(int index) {
	// Bind VAO
	glBindVertexArray(VAOS[index]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[index]);
	// Bind VBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex,Position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex,Normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex,TexCoords));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex,Tangent));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex,Bitangent));
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(5, 4, GL_INT,sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex,m_BoneIDs));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex,m_Weights));
	glEnableVertexAttribArray(6);
	
	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOS[index]);
	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,renderer->depthMapID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,specularTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,normalMap);
}

void AnimatedObject::passToShader(RenderType type)
{
	shader->use();
	shader->passCameraUniformVariables(camera);
	shader->passLightUniformVariables(light);
	shader->passModelMatrix(modelMatrix);

	for(int i = 0; i < 100;i++)
	{
		shader->setMat4("finalBonesMatrices["+std::to_string(i)+"]",transformations[i]);
	}

	shader->setInt("Texture",0);
	shader->setInt("ShadowMap",1);
	shader->setInt("specularMap",2);
	shader->setInt("normalMap",3);
	shader->setInt("RenderType",type);
}

void AnimatedObject::unbind()
{
	// Unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);
}