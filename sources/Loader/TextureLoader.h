#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <strstream>

#include "stb_image.h"

/**
 *  This is a simple texture loader wrapper
 */

class TextureLoader	{
	
	public:

		/**
		 *	1) Create an object with the path
		 *  2) Load the data using loadTexture 
	 	 */

		TextureLoader(std::string texture_path);
		int loadTexture();

	private:
		unsigned int texture;
		std::string path;
};

#endif