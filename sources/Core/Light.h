#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

/**
 * A simple class Light to store the directional Light and point lights 
 */

class Light	{
	
	public:
		// Constructors
		Light(glm::vec3 direction_in);
		// Some wrappers to retrieve the lights
		glm::vec3 getDirection();
		vector<glm::vec3> getPositions();
		vector<glm::vec3> getColors();
		// Add a light
		void addPointLight(glm::vec3,glm::vec3);

	private:
		// Some variables to keep the state of the light
		glm::vec3 direction;
		vector<glm::vec3> positions;
		vector<glm::vec3> colors;
		vector<glm::vec3> constants;
		vector<float> linear;
		vector<float> quadratic;
		
		const unsigned int MAX_POINT_LIGHTS = 15;
};

#endif