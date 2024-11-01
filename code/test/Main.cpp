#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Image.h"
#include "KeyboardEventHandler.h"
#include "TestActivity.h"


static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

std::vector<KeyboardEventHandler*> keyboardEventHandlers;
void RegisterKeyboardHandler(KeyboardEventHandler* keyboard) {
    keyboardEventHandlers.emplace_back(keyboard);
}


void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


GLFWwindow* InitWindows() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);    // 窗口支持透明
    glfwWindowHint(GLFW_SAMPLES, 4);    // 抗锯齿 4x MSAA

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        window = nullptr;
        return nullptr;
    }
    // glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_MULTISAMPLE);    // 抗锯齿
    return window;
}


int main() {
    GLFWwindow* window = InitWindows();
    if (window == nullptr) {
        return -1;
    }
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;


    int keybordIndex = 0;
    int keybordIndexLastFrame = keybordIndex;
    KeyboardEventHandler switchKeyboardEventHandler([window](int keyCode, int eventCode){ return glfwGetKey(window, keyCode) == eventCode; });
    switchKeyboardEventHandler.registerObserver(GLFW_KEY_TAB,      GLFW_PRESS, [&keybordIndexLastFrame, &keybordIndex]() { 
        if (keybordIndexLastFrame == keybordIndex) {
                keybordIndex = (keybordIndex + 1) % keyboardEventHandlers.size();
            }
    });
    switchKeyboardEventHandler.registerObserver(GLFW_KEY_TAB,      GLFW_RELEASE, [&keybordIndexLastFrame, &keybordIndex]() {
        keybordIndexLastFrame = keybordIndex;
    });


    KeyboardEventHandler cameraKeyboardEventHandler([window](int keyCode, int eventCode){ return glfwGetKey(window, keyCode) == eventCode; });
    cameraKeyboardEventHandler.registerObserver(GLFW_KEY_ESCAPE, GLFW_PRESS, [window]() { glfwSetWindowShouldClose(window, true); });
    cameraKeyboardEventHandler.registerObserver(GLFW_KEY_K,      GLFW_PRESS, []() { Screenshot(GetCurPath() + "/resource/screenshot.png", WINDOW_WIDTH, WINDOW_HEIGHT); });
    RegisterKeyboardHandler(&cameraKeyboardEventHandler);

    TestActivity activity(cameraKeyboardEventHandler);

    while(!glfwWindowShouldClose(window)) {
        switchKeyboardEventHandler.exrcute();
        keyboardEventHandlers[keybordIndex]->exrcute();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

        activity.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
