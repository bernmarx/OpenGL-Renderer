/*
	Camera class uses yaw and pitch angles to calculate
	direction vector
*/
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include "shader.h"

//Direction enums for camera
enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 worldUp;
	glm::vec3 up;
	glm::vec3 right;
	float yaw;
	float pitch;
	std::vector<Shader*> linkedShaders;
public:
	glm::mat4 view;
	float sensitivity;
	float speed;
	bool fly;
	//Initialize camera at a pos with default look direction
	Camera(glm::vec3 pos);
	//Initialize camera at a pos that looks at a center
	Camera(glm::vec3 pos, glm::vec3 center);
	void processMovement(CameraMovement dir);
	void processMouse(float xOffset, float yOffset);
	void update();
	//This function links camera with a shader so that
	//every time Camera updates view matrix it will
	//send it automatically to all linked shaders
	void linkShader(Shader* shader);
	void unlinkShader(Shader* shader);

	
};