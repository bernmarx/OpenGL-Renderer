#pragma once

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1400
#define ASPECT (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT
#define APPLICATION_NAME "OpenGL Rendering"
#define VERTEX_SHADER_PATH "shaders/shader.vert"
#define FRAGMENT_SHADER_PATH "shaders/shader.frag"
#define CAMERA_POS glm::vec3(0.0f)
#define INCLUDE_CAMERA extern Camera camera;