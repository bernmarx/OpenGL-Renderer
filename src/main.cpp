/*
    My first OpenGL rendering application
    This program uses:
        1. Opengl 4.6 
        2. GLFW             - for easier OpenGL context management and input processing
        3. GLAD             - for loading OpenGL 4.6 and its functions
        4. stb_image        - for loading image data
        5. easylogging++    - for logging
        6. Assimp           - for importing models
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "defines.h"
#include "Log.h"
#include "input.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "window.h"
#include "scene.h"

INITIALIZE_EASYLOGGINGPP

//Initializing log for the entire application
Log logger("log/config.log");

//Functions that serve as an intermediary between GLFWwindow and Window class
void keyboard_func(GLFWwindow* window);
void mouse_func(GLFWwindow* window, double xpos, double ypos);
void resize_func(GLFWwindow* window, int width, int height);

int main()
{
    //Initializing GLFW and setting up an OpenGL context
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    //Creating window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APPLICATION_NAME, NULL, NULL);
    Window* wndw = new Window();
    if (window == NULL)
    {
        logger.errMsg(WINDOW_CREATION_FAILURE, "Failed to create a window");
        return -1;
    }
    else
        logger.infoMsg("Window successfully created");

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    //Checking if GLAD is successfully initialized
    {
        logger.errMsg(GLAD_INIT_FAILURE, "Failed to initialize GLAD");
        return -1;
    }
    else
        logger.infoMsg("GLAD successfully initialized");

    //Setting up debugging
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        logger.infoMsg("Debugging set up");
    }
    else
        logger.errMsg(DEBUG_SETUP_FAILURE, "Failed to initialize debugging");

    glfwSetWindowUserPointer(window, wndw);
    
    //Setting up callback functions
    glfwSetErrorCallback(error_callback);;
    glfwSetFramebufferSizeCallback(window, resize_func);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_func);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    //Enabling z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    std::cout << "Press ESC to exit\n";
    std::cout << "Use mouse to look around and keyboard to move\n";
    std::cout << "You can unload this first scene by pressing 1";

    //Initializing shader
    Shader shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    shader.use();
    {
        //Set up projection matrix once and then delete everything
        //because there is no need to change it once it is set
        std::unique_ptr<glm::mat4> projection(new glm::mat4(glm::perspective(glm::radians(45.0f), ASPECT, 0.1f, 100.0f)));
        shader.set("projection", *projection);
    }
    //Setting up a camera
    Camera camera(CAMERA_POS);
    camera.linkShader(&shader); //This makes camera update the view matrix automatically
    wndw->linkCamera(&camera);

    Model zuk("resources/zuk/zuk.obj");
    Model terrain("resources/mountain/mountain.obj");

    const glm::vec3 zukLocation = glm::vec3(0.0f, 0.0f, -3.0f);
    //Default ambient, diffuse and specular components of a white light
    const glm::vec3 defLightParam[3] = { glm::vec3(0.1f), glm::vec3(0.9f), glm::vec3(1.0f) };

    //Setting up a scene
    Scene scene;
    wndw->linkScene(&scene);
    scene.addModel(zuk, zukLocation);
    scene.addModel(terrain);
    scene.moveModel(zuk, glm::vec3(0.0f, -1.5f, 0.0f));

    //Setting up lighting (currently only directional lighting works properly
    //Point lights don't have a model, i.e. they are invisible
    //Spotlights don't work at all (will fix)
    DirLight light1(glm::vec3(-1.0f, -1.0f, -1.0f), defLightParam[0], defLightParam[1], defLightParam[2]);
    scene.addDirectionalLight(light1);

    //Main loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Catching input
        keyboard_func(window);

        scene.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void keyboard_func(GLFWwindow* window)
{
    static_cast<Window*>(glfwGetWindowUserPointer(window))->keyboard_input(window);
}
void mouse_func(GLFWwindow* window, double xpos, double ypos)
{
    static_cast<Window*>(glfwGetWindowUserPointer(window))->mouse_input(window, xpos, ypos);
}
void resize_func(GLFWwindow* window, int width, int height)
{
    static_cast<Window*>(glfwGetWindowUserPointer(window))->mouse_input(window, width, height);
}