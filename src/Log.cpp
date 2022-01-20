#include "log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

Log::Log()
{
	config = new el::Configurations();
	config->setToDefault();
	el::Loggers::reconfigureAllLoggers(*config);
	LOG(INFO) << "Initialized log";
}
Log::Log(const char* configPath)
{
	config = new el::Configurations(configPath);
	el::Loggers::reconfigureAllLoggers(*config);
	LOG(INFO) << "Initialized log with config from \"" << configPath << "\"";
}
Log::~Log()
{
	LOG(INFO) << "Finished logging\n";
	config->clear();
	delete config;
}

void Log::errMsg(int errorCode, const char* msg)
{
	LOG(ERROR) << "[ERROR]	0x" << hex << errorCode << " : " << msg;
}
void Log::dbgMsg(const char* msg)
{
	LOG(DEBUG) << "[DEBUG]	" << msg;
}
void Log::infoMsg(const char* msg)
{
	LOG(INFO) << "[INFO]	" << msg;
}

extern Log logger;

void error_callback(int errorCode, const char* description)
{
	logger.errMsg(errorCode, description);
}
//This functions is for OpenGL debugging
void GLAPIENTRY glDebugOutput(GLenum src, GLenum type, GLuint id,
						  GLenum severity, GLsizei length,
						  const GLchar* msg, const void* usrParam)
{
    std::string smsg = "\n";
    switch (src)
    {
    case GL_DEBUG_SOURCE_API:             smsg += "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   smsg += "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: smsg += "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     smsg += "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     smsg += "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           smsg += "Source: Other"; break;
    } 
    smsg += '\n';
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               smsg += "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: smsg += "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  smsg += "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         smsg += "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         smsg += "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              smsg += "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          smsg += "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           smsg += "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               smsg += "Type: Other"; break;
    }
    smsg += '\n';
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         smsg += "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       smsg += "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          smsg += "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: smsg += "Severity: notification"; break;
    }
    smsg += '\n';
    smsg += msg;
    logger.errMsg(id, smsg.c_str());
}