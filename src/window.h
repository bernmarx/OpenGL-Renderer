#pragma once
#include <vector>
#include "camera.h"
#include "scene.h"

class Window {
private:
	std::vector<Camera*> linkedCameras;
	std::vector<std::pair<Scene*, unsigned int>> linkedObjects;
	std::vector<Scene*> linkedScenes;
	double lastX, lastY;
	bool firstMouse;
public:
	Window();
	void linkCamera(Camera* cam);
	void unlinkCamera(Camera* cam);
	void updateCameras(CameraMovement dir);
	void updateCameras(float xOffset, float yOffset);
	void keyboard_input(GLFWwindow* window);
	void mouse_input(GLFWwindow* window, double xpos, double ypos);
	void resize(GLFWwindow* window, int width, int height);

	void linkObject(std::pair<Scene*, unsigned int> obj);
	void unlinkObject(std::pair<Scene*, unsigned int> obj);
	void updateObjects();

	void linkScene(Scene* scene);
	void unlinkScene(Scene* scene);
	void updateScenes();
};