#include "camera.h"
#include "Log.h"
#include <memory>

INCLUDE_LOGGER

Camera::Camera(glm::vec3 pos)
{
	position = pos;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;
	sensitivity = 0.1f;
	speed = 0.05f;
	fly = true;

	this->update();
	logger.infoMsg("Camera successfully initialized");
}
Camera::Camera(glm::vec3 pos, glm::vec3 center)
{
	position = pos;
	glm::vec3 dir = normalize(center - pos);
	pitch = glm::degrees(asin(dir.y));
	yaw = asin(dir.z / cos(glm::radians(pitch)));
	sensitivity = 0.1f;
	speed = 0.01f;
	fly = true;

	this->update();
	logger.infoMsg("Camera successfully initialized");
}

void Camera::update()
{
	//Calculate sines and cosines beforehand
	std::unique_ptr<float> cos_yaw(new float(cos(glm::radians(yaw))));
	std::unique_ptr<float> sin_yaw(new float(sin(glm::radians(yaw))));
	std::unique_ptr<float> cos_pitch(new float(cos(glm::radians(pitch))));
	std::unique_ptr<float> sin_pitch(new float(sin(glm::radians(pitch))));
	
	direction = normalize(glm::vec3((*cos_yaw) * (*cos_pitch), (*sin_pitch), (*sin_yaw) * (*cos_pitch)));
	right = normalize(glm::cross(direction, worldUp));
	up = normalize(glm::cross(right, direction));

	view = glm::lookAt(position, position + direction, up);

	for (auto shader : linkedShaders)
	{
		shader->set("view", view);
		shader->set("viewerPos", position);
	}
}
void Camera::processMovement(CameraMovement dir)
{
	switch (dir)
	{
	case FORWARD:
		//Changing position based on if a fly mode is enabled
		position += (fly ? direction : glm::vec3(direction.x, 0, direction.z)) * speed;
		break;
	case BACKWARD:
		position -= (fly ? direction : glm::vec3(direction.x, 0, direction.z)) * speed;
		break;
	case LEFT:
		position -= right * speed;
		break;
	case RIGHT:
		position += right * speed;
	}
	
	this->update();
}
void Camera::processMouse(float xOffset, float yOffset)
{
	yaw += sensitivity * xOffset;
	pitch -= sensitivity * yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	this->update();
}
void Camera::linkShader(Shader* shader)
{
	linkedShaders.push_back(shader);
	shader->set("view", view);
}
void Camera::unlinkShader(Shader* shader)
{
	for (int i = 0; i < linkedShaders.size(); i++)
	{
		if (linkedShaders[i] == shader)
			linkedShaders.erase(linkedShaders.begin() + i);
		else
			logger.dbgMsg("[WARNING]  Tried to unlink shader from camera that was not linked");
	}
}
