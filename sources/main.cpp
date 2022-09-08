#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Loader/Shader.h"
#include "Loader/stb_image.h"
#include "Core/Camera.h"
#include "Object/Object.h"
#include "Core/Light.h"
#include "Terrain/TerrainGenerator.h"
#include "Skybox/Skybox.h"
#include "Renderer/MasterRenderer.h"
#include "AnimatedObject/AnimatedObject.h"
#include "AnimatedObject/Animation.h"
#include "AnimatedObject/Animator.h"
#include "UI/UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include "Loader/MeshLoader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback_detached(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLFWwindow* setupDisplay();


unsigned int sizeX = 1500;
unsigned int sizeY = 1000;

Camera camera = Camera(sizeX,sizeY);
GLFWwindow* window = setupDisplay();

int main()
{
	MasterRenderer renderer = MasterRenderer(sizeX,sizeY);

	Light light(glm::vec3(0.5, 0.5, 0.5));

	TerrainGenerator terrain = TerrainGenerator(&camera,&light,&renderer);
	Skybox skybox = Skybox(&camera);

	renderer.addSkybox(&skybox);
	renderer.addTerrain(&terrain);

	AnimatedObject sac = AnimatedObject(&camera,&light,"Data/model.dae","Data/diffuse.png",&renderer);
	Animation animation("Data/model.dae",&sac);
	Animator animator(&animation);

	renderer.addDrawable(&sac);

	Object object = Object(&camera,&light,"Data/backpack.obj",&renderer);
	object.setPosition(glm::vec3(0,5,0));

	renderer.addDrawable(&object);


	bool blackScreen = false;
	int counter = 0;
	float lastFrame;
	UI ui = UI(window);

	animator.UpdateAnimation(0);
	auto transforms = animator.GetFinalBoneMatrices();
	sac.transformations = transforms;

	while (!glfwWindowShouldClose(window)) 
	{
		camera.processKeyboardInput(window);
		float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		animator.UpdateAnimation(deltaTime);
		auto transforms = animator.GetFinalBoneMatrices();
		sac.transformations = transforms;
		
		renderer.render();
		ui.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

GLFWwindow* setupDisplay()
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	GLFWwindow* window = glfwCreateWindow(sizeX,sizeY, "3D Graphics Engine", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	gladLoadGL();
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_FRAMEBUFFER_SRGB); 
	return window;
}

bool windowGrab = true;
void switchCallBack(){
	if(windowGrab) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window,mouse_callback_detached);
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window,mouse_callback);
	}
	windowGrab =  !windowGrab;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.processMouseInput(xpos, ypos,true);
}

void mouse_callback_detached(GLFWwindow* window, double xpos, double ypos) 
{
	camera.processMouseInput(xpos, ypos,false);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processScrollInput(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		switchCallBack();
}
