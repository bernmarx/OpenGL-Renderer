/*
	This class basically provides an interface
	to use easylogging++ library
*/
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <easylogging++.h>

#define INCLUDE_LOGGER extern Log logger;
#define LOGGER logger

enum errors { WINDOW_CREATION_FAILURE, GLAD_INIT_FAILURE, FILE_READ_FAILURE, DEBUG_SETUP_FAILURE };

using namespace std;

class Log {
private:
	el::Configurations* config;
public:
	Log();
	Log(const char* configPath);
	~Log();
	void errMsg(int errorCode, const char* msg);
	void dbgMsg(const char* msg);
	void infoMsg(const char* msg);
};

void error_callback(int errorCode, const char* description);
void GLAPIENTRY glDebugOutput(GLenum src, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar* msg, const void* usrParam);