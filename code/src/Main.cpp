#include <iostream>
#include "Shader.h"
#include <math.h>
#include "Image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model/Rectangle.h"
#include "model/Cuboid.h"


static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
GLFWwindow* InitWindows() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
    return window;
}


int main() {
    GLFWwindow* window = InitWindows();
    if (window == nullptr) {
        return -1;
    }
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);

    Rectangle rectangle(1.0f, 1.0f);
    rectangle.setPosition(0.0f, -0.5f);
    rectangle.setScaleRatio(0.5f);
    rectangle.setRotation(glm::radians(-45.0f));
    rectangle.setColor(Color(0.8f, 0.3f, 0.2f));
    rectangle.setImage("/Users/nirui02/workspace/code/MisuiLib/RenderPractice/resource/awesomeface.png", true);

    Rectangle rectangle1(1.0f, 0.5f);
    rectangle1.setPosition(-1.0, 0.0f);
    rectangle1.setColor(Color(0.8f, 0.3f, 0.2f));
    rectangle1.setImage("/Users/nirui02/workspace/code/MisuiLib/RenderPractice/resource/container.jpeg");

    Cubiod cuboid(1.0f, 1.0f, 1.0f);
    cuboid.setPosition(-0.5f, -0.5f, -0.5f);
    // cuboid.setScaleRatio(0.5f);
    cuboid.setRotationAxis({ 0.5f, 1.0f, 0.0f });
    cuboid.addImage("/Users/nirui02/workspace/code/MisuiLib/RenderPractice/resource/container.jpeg");
    cuboid.addImage("/Users/nirui02/workspace/code/MisuiLib/RenderPractice/resource/awesomeface.png", true);

    while(!glfwWindowShouldClose(window))
    {
        cuboid.setRotation((float)glfwGetTime());

        ProcessInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rectangle.draw();
        rectangle1.draw();
        cuboid.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}