#ifndef CAMERA_H
#define CAMERA_H


void switchCallBack();

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

/**
 *  This is the camera class of my engine
 *  It contains all wrappers for the matrix transformations 
 *  Not only for the player but also for all special effects (shadow mapping/environmental map)
 */

class Camera {
	
	public:
	
		Camera(unsigned int,unsigned int);

		glm::mat4 getCamera();
		glm::vec3 getCameraPosition();
		glm::mat4 getProjectionMatrix();
		// Camera used for shadow mapping
		glm::mat4 getShadowCamera();
		glm::vec3 getShadowCameraPosition();
		// Cammera used to draw in a texture
		void setSpecialCamera(glm::vec3 position,unsigned int number);
		void setNormalCamera();
		// Some wrappers to update the state of the camera
		void updateCamera(GLFWwindow* window);
		void processMouseInput(double xpos, double ypos,bool moving);
		void processKeyboardInput(GLFWwindow* window);
		void processScrollInput(double xoffset, double yoffset);
		void setPosition(glm::vec3 pos);

	private:

		// camera position
		glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		// special camera
		glm::vec3 special_position;
		glm::vec3 special_position_front;
		glm::vec3 special_position_up;
		// shadow map position
		glm::vec3 shadowMapPosition;
		glm::vec3 shadowMapDirection;

		// somes variables to keep the state of the camera
		bool firstMouse = true;
		float yaw = -90.0f;
		float pitch = 0.0f;
		float fov = 45.0f;
		float lastX = 800.0f / 2.0;
		float lastY = 600.0 / 2.0;
		bool special_camera = false;
		// timing
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

		float ratio;
};

#endif