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


    static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    static float lastFrame = 0.0f; // 上一帧的时间

    static float pitch = 0.0f;
    static float yaw = 180.0f;

 
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float cameraSpeed = 2.5f * deltaTime;
    float cameraRollSpeed = 2.5f * deltaTime * 5;

    Camera& camera = Camera::instance();
    std::vector<float> frontVec = camera.getFront();
    glm::vec3 cameraFront = glm::vec3(frontVec[0], frontVec[1], frontVec[2]);
    std::vector<float> posVec = camera.getPosition();
    glm::vec3 cameraPos = glm::vec3(posVec[0], posVec[1], posVec[2]);
    std::vector<float> rightVect = camera.getRight();
    glm::vec3 cameraRight = glm::vec3(rightVect[0], rightVect[1], rightVect[2]);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraRight;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraRight;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        yaw -= cameraRollSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        yaw += cameraRollSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        pitch += cameraRollSpeed;
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        pitch -= cameraRollSpeed;
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }

    camera.setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
    camera.setFront(pitch, yaw);
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

    Camera& camera = Camera::instance();
    camera.setPosition(2.0f, 0.0f, 0.0f);

    Rectangle rectangle(1.0f, 1.0f);
    rectangle.setPosition(0.0f, -0.5f);
    rectangle.setScaleRatio(0.5f);
    rectangle.setRotation(glm::radians(-45.0f));
    rectangle.setColor(Color(0.8f, 0.3f, 0.2f));
    rectangle.setImage(GetCurPath() + "/resource/awesomeface.png", true);

    Rectangle rectangle1(1.0f, 0.5f);
    rectangle1.setPosition(-1.0, 0.0f);
    rectangle1.setColor(Color(0.8f, 0.3f, 0.2f));
    rectangle1.setImage(GetCurPath() + "/resource/container.jpeg");

    float cuboidPositions[10][3] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 2.0f},
        {0.0f, 0.0f, 3.0f},
        {0.0f, 0.0f, 4.0f},
        {0.0f, 0.0f, 5.0f},
        {0.0f, 0.0f, 6.0f},
        {0.0f, 0.0f, 7.0f},
        {0.0f, 0.0f, 8.0f},
        {0.0f, 0.0f, 9.0f}
    };

    std::vector<Cubiod> cuboids;
    for (int index = 0; index < 10; ++index) {
        Cubiod cuboid(1.0f, 1.0f, 1.0f);
        cuboid.setPosition(cuboidPositions[index][0], cuboidPositions[index][1], cuboidPositions[index][2]);
        cuboid.addImage(GetCurPath() + "/resource/container.jpeg");
        cuboid.addImage(GetCurPath() + "/resource/awesomeface.png", true);
        cuboids.push_back(cuboid);
    }

    Cubiod cuboid(1.0f, 1.0f, 1.0f);
    cuboid.setPosition(0.0f, 2.0f, 0.0f);
    // cuboid.setScaleRatio(0.5f);
    cuboid.setRotationAxis({ 1.0f, 1.0f, 1.0f });
    cuboid.addImage(GetCurPath() + "/resource/container.jpeg");
    cuboid.addImage(GetCurPath() + "/resource/awesomeface.png", true);

    Cubiod cuboid1(1.0f, 1.0f, 1.0f);
    cuboid1.setPosition(1.0f, -3.5f, 5.0f);
    cuboid1.setSize(0.5f, 2.0f, 0.3f);
    cuboid1.setScaleRatio(0.5f);
    cuboid1.setRotationAxis({ 1.0f, 1.0f, 1.0f });
    cuboid1.addImage(GetCurPath() + "/resource/container.jpeg");
    cuboid1.addImage(GetCurPath() + "/resource/awesomeface.png", true);

    while(!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        cuboid.setRotation((float)glfwGetTime());
        cuboid1.setRotation(-(float)glfwGetTime());

        ProcessInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rectangle.draw();
        // rectangle1.draw();
        cuboid.draw();
        cuboid1.draw();

        for (int index = 0; index < cuboids.size(); ++index) {
            cuboids[index].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}
