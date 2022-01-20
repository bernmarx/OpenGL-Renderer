/*
	Shader class deals with, well, shaders and drawing
*/
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

class Shader {
private:
	GLuint ID;
public:
	Shader(const char* vsPath, const char* fsPath);
	~Shader();
	void use();
	void set(const char* uniName, int val);
	void set(const char* uniName, float val);
	void set(const char* uniName, glm::vec2 vec);
	void set(const char* uniName, glm::vec3 vec);
	void set(const char* uniName, glm::mat3 mat);
	void set(const char* uniName, glm::mat4 mat);
	bool operator==(const Shader& sh);
};