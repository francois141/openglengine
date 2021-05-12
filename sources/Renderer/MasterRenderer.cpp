#include "MasterRenderer.h"


MasterRenderer::MasterRenderer(unsigned int sizeX_in,unsigned int sizeY_in)
{
	objects = vector<Object*>();
	animatedsobjects = vector<AnimatedObject*>();
	screenShader = Shader("../sources/Renderer/renderer_v.glsl", "../sources/Renderer/renderer_f.glsl");
	
	sizeX = sizeX_in;
	sizeY = sizeY_in;

	createFrammeBuffer();
	createDepthMapFBO();
}

void MasterRenderer::render()
{
	/** Draw to the depth map **/
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	for(auto o:objects)
	{
		o->drawShadow();
	}
	for(auto o:animatedsobjects) 
	{
		o->drawShadow();
	}
	if(terrain != NULL) {
		terrain->drawShadow();
	}

	glViewport(0, 0,sizeX,sizeY);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/** Draw  **/
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	draw();


	/**** LAST PASS POSTPROCESSING ****/
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenShader.use();
	screenShader.setInt("screenTexture", 0);
	screenShader.setInt("brightTexture", 1); 
	screenShader.setBool("blackScreen",renderWithNoColors);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MasterRenderer::addObject(Object *object)
{
	objects.push_back(object);
}

void MasterRenderer::addObject(AnimatedObject *object)
{
	animatedsobjects.push_back(object);
}

void MasterRenderer::addSkybox(Skybox *skybox_in)
{
	skybox = skybox_in;
}

void MasterRenderer::addTerrain(TerrainGenerator *terrain_in)
{
	terrain = terrain_in;
}

void MasterRenderer::draw()
{
	if (skybox != NULL) {
		skybox->draw();
	}
	if (terrain != NULL) {
		terrain->draw();
	}

	for (auto object : objects)
	{
		object->draw();
	}

	for(auto object : animatedsobjects)
	{
		object->draw();
	}
}

void MasterRenderer::drawLowQuality()
{
	if (skybox != NULL) {
		skybox->draw();
	}
	if (terrain != NULL) {
		terrain->drawLowQuality();
	}

	for (auto object : objects)
	{
		object->drawLowQuality();
	}
	for(auto object : animatedsobjects)
	{
		object->drawLowQuality();
	}
}

void MasterRenderer::setBlackScreen(bool in)
{
	renderWithNoColors = in;
}

bool MasterRenderer::isBlackScreen()
{
	return renderWithNoColors;
}

void MasterRenderer::createDepthMapFBO()
{
	// Create FBO
	glGenFramebuffers(1,&depthMapFBO);
	// Create Texture
	glGenTextures(1, &depthMapID);
	glBindTexture(GL_TEXTURE_2D, depthMapID);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Bind Texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	 	std::cout << "Not Complete" << std::endl; 
}


void MasterRenderer::createFrammeBuffer()
{
	/** CREATE THE VAO & VBO FOR THE QUAD **/
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO); 
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	/** CREATE THE FRAMEBUFFER **/
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	/** CREATE & ATTACH A TEXTURE **/
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2;i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, sizeX, sizeY, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}

	/** ADD A RENDER BUFFER OBJECT **/
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	////
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	////

	/** CHECK FRAMEBUFFER **/
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


