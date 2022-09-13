#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Init/init.h"
#include "Shaders/Shader.h"
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
#include "Loader/MeshLoader.h"
#include "UI/UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const unsigned int sizeX = 1500;
const unsigned int sizeY = 1000;

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

	Object object = Object(&camera,&light,"Data/backpack.obj",&renderer);
	object.setPosition(glm::vec3(0,5,0));

	renderer.addDrawable(&object);


	bool blackScreen = false;
	int counter = 0;
	float lastFrame;
	UI ui = UI(window);

	while (!glfwWindowShouldClose(window)) 
	{
		camera.processKeyboardInput(window);
		float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		renderer.render();
		ui.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}