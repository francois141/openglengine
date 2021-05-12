#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "Loader/Shader.h"
#include "Core/Camera.h"
#include "Core/Light.h"
#include "Loader/stb_image.h"
#include "Loader/OBJLoader.h"
#include "Loader/TextureLoader.h"
#include "Renderer/MasterRenderer.h"
#include "Drawable/Drawable.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <strstream>

using namespace std;

class MasterRenderer;

class TerrainGenerator : public Drawable{

	enum RenderType {
		LOW = 1,
		NORMAL = 2,
		SHADOW = 3,
	};

	public:
		// Constructor
		TerrainGenerator(Camera* camera_in, Light* light_in,MasterRenderer *renderer_in);

		// Classical draw functions
		void draw();
		void drawShadow();
		void drawLowQuality();
	protected:

		// Setup functions
		void createBuffers();
		void remove();
		// Bind wrappers
		void bind();
		void passToShader(RenderType);
		void unbind();
 
		// Specific functions to build the terrain
		vector<float> generateTerrain();
		void generateCell(int offsetX, int offsetZ,int x, int z);
		void addVecOffset(glm::vec3 data, int offsetX, int offsetZ,int x, int z);
		void addVec(glm::vec2 data);
		void addVec(glm::vec3 data);

		// Multitexturing
		unsigned int texture1;
		unsigned int texture2;
		unsigned int texture3;
		unsigned int texture4;
		unsigned int blendMap;

		// Some variables to build the terrain
		glm::vec3 point1 = glm::vec3(0, 0, 0);
		glm::vec3 point2 = glm::vec3(0, 0, 1);
		glm::vec3 point3 = glm::vec3(1, 0, 0);
		glm::vec3 point4 = glm::vec3(1, 0, 1);
		glm::vec3 normal = glm::vec3(0, 1.0, 0);
		unsigned int SIZE = 255;
		int cellSize = 1;
		vector<float> data;
		vector<float> vertexData;
		vector<vector<double> > heightData;
};

#endif
