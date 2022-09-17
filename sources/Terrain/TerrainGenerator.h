#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "Shaders/Shader.h"
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
		TerrainGenerator(Camera* camera_in, Light* light_in,MasterRenderer *renderer_in,const unsigned int size);

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

        void _draw(const RenderType RENDER_TYPE);
 
		// Specific functions to build the terrain
		vector<float> generateTerrain(const unsigned int size);
		void generateCell(int offsetX, int offsetZ,int x, int z,const unsigned int size);
		void addVecOffset(glm::vec3 data, int offsetX, int offsetZ,int x, int z);
		void addVec(glm::vec2 data);
		void addVec(glm::vec3 data);
		void addEmptyVec3();

		// Multitexturing
		vector<unsigned int> textures;
		unsigned int texture1;
		unsigned int texture2;
		unsigned int texture3;
		unsigned int texture4;
		unsigned int blendMap;

		vector<float> data;
		vector<float> vertexData;
		vector<vector<double> > heightData;

        const string BASE_PATH_RESOURCES = "../sources/Terrain/Resources/";
        const string BASE_PATH_SHADERS = "../sources/Terrain/Shaders/";
};

#endif
