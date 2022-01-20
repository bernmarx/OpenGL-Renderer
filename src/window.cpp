#include "window.h"
#include "defines.h"
#include <iostream>

Window::Window()
{
	lastX = WINDOW_WIDTH / 2;
	lastY = WINDOW_HEIGHT / 2;
	firstMouse = true;
}
void Window::linkCamera(Camera* cam)
{
	linkedCameras.push_back(cam);
}
void Window::unlinkCamera(Camera* cam)
{
	for (int i = 0; i < linkedCameras.size(); i++)
	{
		if (linkedCameras[i] == cam)
			linkedCameras.erase(linkedCameras.begin() + i);
	}
}
void Window::linkObject(std::pair<Scene*, unsigned int> obj)
{
	linkedObjects.push_back(obj);
}
void Window::unlinkObject(std::pair<Scene*, unsigned int> obj)
{
	for (int i = 0; i < linkedObjects.size(); i++)
	{
		if (linkedObjects[i] == obj)
			linkedObjects.erase(linkedObjects.begin() + i);
	}
}
void Window::linkScene(Scene* scene)
{
	linkedScenes.push_back(scene);
}
void Window::unlinkScene(Scene* scene)
{
	for (int i = 0; i < linkedScenes.size(); i++)
	{
		if (linkedScenes[i] == scene)
			linkedScenes.erase(linkedScenes.begin() + i);
	}
}

void Window::updateCameras(CameraMovement dir)
{
	for (auto cam : linkedCameras)
	{
		cam->processMovement(dir);
	}
}
void Window::updateCameras(float xOffset, float yOffset)
{
	for (auto cam : linkedCameras)
	{
		cam->processMouse(xOffset, yOffset);
	}
}

void Window::keyboard_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		updateCameras(FORWARD);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		updateCameras(BACKWARD);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		updateCameras(LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		updateCameras(RIGHT);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		linkedScenes[0]->unload();
}
void Window::mouse_input(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	updateCameras(xOffset, yOffset);
}
void Window::resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}