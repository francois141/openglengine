#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

#include "Drawable/Drawable.h"
#include "Loader/Shader.h"
#include "Core/Camera.h"
#include "Core/Light.h"
#include "Loader/stb_image.h"
#include "Loader/OBJLoader.h"
#include "Loader/TextureLoader.h"
#include "Renderer/MasterRenderer.h"
#include "Loader/AnimatedMeshLoader.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <strstream>	

using namespace std;
class Drawable;
class MasterRenderer;

class AnimatedObject : public Drawable {

	enum RenderType {
		LOW = 1,
		NORMAL = 2,
		SHADOW = 3,
	};

	public:
		// Constructors 
		AnimatedObject(Camera* camera_in, Light* light_in, string obj_path, string texture_path,MasterRenderer *renderer_in);

		// Draw functions 
		void draw();
		void drawShadow();
		void drawLowQuality();

		
		AnimatedMeshLoader loader;

		std::vector<glm::mat4> transformations;
		
	private:
		// Setup function 
		void createBuffers(int index);
		// Bind wrappers

		void bind(int index);

		void unbind();

		void passToShader(RenderType);

		// Indices
		std::vector<unsigned int> VAOS;
		std::vector<unsigned int> VBOS;
		std::vector<unsigned int> EBOS;

		std::vector<std::vector<AnimatedVertex> > meshData;
		std::vector<std::vector<unsigned int> > meshIndices;

		// Number meshes
		unsigned int nbMeshes;
		unsigned int normalMap;
		unsigned int specularTexture;

};

#endif