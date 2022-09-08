#ifndef MASTER_RENDERER_H
#define MASTER_RENDERER_H

#include "Object/Object.h"
#include "Drawable/Drawable.h"
#include "Core/Camera.h"
#include "Core/Light.h"
#include "Loader/Shader.h"
#include "Skybox/Skybox.h"
#include "Terrain/TerrainGenerator.h"
#include "Drawable/Drawable.h"
#include "AnimatedObject/AnimatedObject.h"
#include <iostream>
#include <vector>

using namespace std;

class TerrainGenerator;
class Object;
class Drawable;
class AnimatedObject;

/**
 * This is the most important class in the engine.
 * The master renderer is reponsible for rendering the data on the screen
 * All special effects (blur,shadow,..) are managed within this class
 */

class MasterRenderer
{
	public:
		// Constructor
		MasterRenderer(unsigned int, unsigned int);
		// Render function for the mainloop
		void render();
		// Add objects to render
		void addDrawable(Drawable *object);
		void addSkybox(Skybox *skybox_in);
		void addTerrain(TerrainGenerator *terrain_in);

		void draw();
		void drawLowQuality();
		// Some important data
		unsigned int framebuffer;
		unsigned int depthMapFBO;
		unsigned int depthMapID;

		// Render with or without colors
		void setBlackScreen(bool);
		bool isBlackScreen();



	private:
		// For shadow mappping
		const unsigned int SHADOW_WIDTH = 4*1024;
		const unsigned int SHADOW_HEIGHT = 4*1024;
		void createDepthMapFBO();

		// For blur effects
		void createFrammeBuffer();
		
		// Some attributes for the last draw
		unsigned int quadVAO, quadVBO;
		unsigned int textureColorbuffer;
		unsigned int colorBuffers[2];
		Shader screenShader;
		float quadVertices[4*6] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};

		// Some attributes to store the objects in the renderer
		Skybox* skybox;
		vector<Drawable*> objects;

		unsigned int sizeX;
		unsigned int sizeY;

		bool renderWithNoColors = false;
};

#endif