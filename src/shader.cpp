#include "shader.h"
#include "Log.h"
#include "defines.h"
#include <string>
#include <sstream>
#include <fstream>
#include <gtc/type_ptr.hpp>

INCLUDE_LOGGER

Shader::Shader(const char* vsPath, const char* fsPath)
{
	//Reading shader code from files
	std::ifstream vsFile;
	std::ifstream fsFile;
	std::string vsCodeStr, fsCodeStr;
	vsFile.exceptions(ios::failbit | ios::badbit);
	fsFile.exceptions(ios::failbit | ios::badbit);
	try
	{
		vsFile.open(vsPath);
		fsFile.open(fsPath);
		std::stringstream vsCodeSs, fsCodeSs;
		vsCodeSs << vsFile.rdbuf();
		fsCodeSs << fsFile.rdbuf();
		vsFile.close();
		fsFile.close();
		vsCodeStr = vsCodeSs.str();
		fsCodeStr = fsCodeSs.str();
	}
	catch (std::ifstream::failure f)
	{
		LOGGER.errMsg(FILE_READ_FAILURE, "Failed to read a file when initializing a shader");
	}
	std::unique_ptr<const char*> vsCode(new const char*(vsCodeStr.c_str()));
	std::unique_ptr<const char*> fsCode(new const char* (fsCodeStr.c_str()));

	//Creating and compiling shaders
	GLuint vs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, vsCode.get(), NULL);
	glShaderSource(fs, 1, fsCode.get(), NULL);
	glCompileShader(vs);
	glCompileShader(fs);

	ID = glCreateProgram();
	glAttachShader(ID, vs);
	glAttachShader(ID, fs);
	glLinkProgram(ID);

	glDeleteShader(vs);
	glDeleteShader(fs);
}
Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

//All these set() functions just send data to shader as a uniform variable
void Shader::set(const char* uniName, int val)
{
	glUniform1i(glGetUniformLocation(ID, uniName), val);
}
void Shader::set(const char* uniName, float val)
{
	glUniform1f(glGetUniformLocation(ID, uniName), val);
}
void Shader::set(const char* uniName, glm::vec3 vec)
{
	glUniform3f(glGetUniformLocation(ID, uniName), vec.x, vec.y, vec.z);
}
void Shader::set(const char* uniName, glm::vec2 vec)
{
	glUniform2f(glGetUniformLocation(ID, uniName), vec.x, vec.y);
}
void Shader::set(const char* uniName, glm::mat3 mat)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, uniName), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::set(const char* uniName, glm::mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, uniName), 1, GL_FALSE, glm::value_ptr(mat));
}

bool Shader::operator==(const Shader& sh)
{
	return (this->ID == sh.ID);
}