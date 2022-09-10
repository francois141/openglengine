#ifndef OBJECT_H
#define OBJECT_H

#include "Drawable/Drawable.h"
#include "Loader/Shader.h"
#include "Core/Camera.h"
#include "Core/Light.h"
#include "Loader/stb_image.h"
#include "Loader/OBJLoader.h"
#include "Loader/TextureLoader.h"
#include "Renderer/MasterRenderer.h"
#include "Loader/MeshLoader.h"
#include "Core/Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <strstream>	

using namespace std;
class Drawable;
class MasterRenderer;

class Object : public Drawable {
	
	enum RenderType {
		LOW = 1,
		NORMAL = 2,
		SHADOW = 3,
	};

	public:
		// Constructors 
		Object(Camera* camera_in, Light* light_in, string obj_path,MasterRenderer *renderer_in);

		// Draw functions 
		void draw();
		void drawShadow();
		void drawLowQuality();
		
	private:
		// Setup function 
		void createBuffers();
		void createBuffers(int index);
		// Bind wrappers
		void passToShader(RenderType);
		void unbind();

		void bind(int index);
        void _draw(RenderType TYPE);

		// Indices
		std::vector<unsigned int> VAOS;
		std::vector<unsigned int> VBOS;
		std::vector<unsigned int> EBOS;

		std::vector<std::vector<float> > meshData;
		std::vector<std::vector<unsigned int> > meshIndices;
		std::vector<Material> materials;

		// Number meshes
		unsigned int nbMeshes;
		unsigned int normalMap;
		unsigned int specularTexture;
};

#endif