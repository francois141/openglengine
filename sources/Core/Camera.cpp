#include "Camera.h"



Camera::Camera(unsigned int sizeX,unsigned int sizeY)
{
	ratio = (float)sizeX/(float)sizeY;
}

glm::mat4 Camera::getCamera()
{
	if(special_camera)
	{
		return glm::lookAt(special_position,special_position+special_position_front,special_position_up);
	}
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::vec3 Camera::getCameraPosition()
{
	return cameraPos;
}

glm::mat4 Camera::getProjectionMatrix()
{
	if(special_camera)
	{
		return glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 20.0f); 
	}
	return glm::perspective(glm::radians(fov), ratio, 0.01f, 10000.0f);
}

glm::mat4 Camera::getShadowCamera()
{
	float near_plane = 0.00f, far_plane = 40.5f;
	glm::mat4 lightProjection = glm::ortho(-16.0f, 16.0f, -16.0f, 16.0f, near_plane, far_plane);
	glm::vec3 position = glm::vec3(0,0,0);
	position.x = cameraPos.x + 8;
	position.y = 3;
	position.z = cameraPos.z + 8;
	glm::vec3 view     = glm::vec3(position.x-0.5, position.y - 0.5, position.z - 0.5);
	glm::mat4 lightView = glm::lookAt(position, 
                              view,
                              glm::vec3( 0.01f, 1.0f,  0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;  
	return lightSpaceMatrix;
}

glm::vec3 Camera::getShadowCameraPosition()
{
	return glm::vec3(cameraPos.x,5,cameraPos.z);
}

void Camera::setSpecialCamera(glm::vec3 pos_in,unsigned int number)
{
	switch (number)
	{
		case 0:
			special_position_front = glm::vec3(1,0,0);
			special_position_up    = glm::vec3(0,-1,0);
			break;
		case 1:
			special_position_front = glm::vec3(-1,0,0);
			special_position_up    = glm::vec3(0,-1,0);
			break;
		case 2:
			special_position_front = glm::vec3(0,1,0);
			special_position_up    = glm::vec3(0,0,1);
			break;
		case 3:
			special_position_front = glm::vec3(0,-1,0);
			special_position_up    = glm::vec3(0,0,-1);
			break;
		case 4:
			special_position_front = glm::vec3(0,0,1);
			special_position_up    = glm::vec3(0,-1,0);
			break;
		case 5:
			special_position_front = glm::vec3(0,0,-1);
			special_position_up    = glm::vec3(0,-1,0);
			break;
	}
	special_position = pos_in;
	special_camera = true;
}

void Camera::setNormalCamera()
{
	special_camera = false;
}

void Camera::updateCamera(GLFWwindow* window)
{
	processKeyboardInput(window);
}

void Camera::processMouseInput(double xpos, double ypos,bool moving)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	if(!moving) {
		lastX = xpos;
		lastY = ypos;
		return;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
	cameraFront = glm::normalize(front);
}

void Camera::processKeyboardInput(GLFWwindow* window)
{
	if(special_camera)
	{
		return;
	}
	float currentFrame = glfwGetTime();

	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
		

	glm::vec3 customVec = glm::normalize(glm::cross(cameraFront, cameraUp));

	float cameraSpeed = 2.5 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, customVec)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, customVec)) * cameraSpeed;

}

void Camera::processScrollInput(double xoffset, double yoffset)
{
	if(special_camera)
		return;
		
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void Camera::setPosition(glm::vec3 pos)
{
	cameraPos = pos;
}



