#include "Light.h"

Light::Light(glm::vec3 direction_in)
{
	direction = direction_in;
	// The engine stats with two point lights;
	positions = { glm::vec3(-3.0,2.0,0.0), glm::vec3(3.0,2.0,0.0)};
	colors    = { glm::vec3(0.0,0.4,1.0), glm::vec3(1.0,0.8,0.0) };
}

glm::vec3 Light::getDirection()
{
	return direction;
}

vector<glm::vec3> Light::getPositions()
{
	return positions;
}

vector<glm::vec3> Light::getColors()
{
	return colors;
}

void Light::addPointLight(glm::vec3 position,glm::vec3 color)
{
	if(positions.size() >= MAX_POINT_LIGHTS)
	{
		return;
	}
	positions.push_back(position);
	colors.push_back(color);
}

