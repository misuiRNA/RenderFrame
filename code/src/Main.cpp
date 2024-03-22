#include <iostream>
#include "Shader.h"
#include <math.h>
#include "Image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model/Rectangle.h"
#include "model/Cuboid.h"
#include "model/LightSource.h"
#include "Camera.h"
#include <GLFW/glfw3.h>


static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window, CameraControllerFPSStyle& cameraCtrl) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    static float lastFrame = 0.0f; // 上一帧的时间

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraCtrl.goForward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraCtrl.goBack(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraCtrl.goLeft(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraCtrl.goRight(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cameraCtrl.turnRight(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cameraCtrl.turnLeft(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraCtrl.turnUp(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraCtrl.turnDown(deltaTime);
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

    Camera camera;
    CameraControllerFPSStyle cameraCtrl(camera);
    cameraCtrl.setPosition(8.0f, 0.0f, 2.0f);
    cameraCtrl.setAttitude(0.0f, 180.0f);

    LightSource light(-1.0f, 2.0f, 2.0f);
    light.setSize(0.5f, 0.5f, 0.5f);
    // light.setColor(Color(0.33f, 0.42f, 0.18f));

    Rectangle rectangle(1.0f, 1.0f);
    rectangle.setPosition(0.0f, -0.5f);
    rectangle.setScaleRatio(0.5f);
    rectangle.setRotation(glm::radians(-45.0f));
    rectangle.setColor(Color(0.8f, 0.3f, 0.2f));
    rectangle.setImage(GetCurPath() + "/resource/awesomeface.png", true);

    Rectangle rectangle1(1.0f, 0.5f);
    rectangle1.setPosition(-1.0, 0.0f);
    rectangle1.setColor(Color(0.8f, 0.3f, 0.2f));
    // rectangle1.setImage(GetCurPath() + "/resource/container.jpeg");

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
    cuboid.setColor(Color(1.0f, 0.5f, 0.31f));
    cuboid.setScaleRatio(2.0f);
    cuboid.setRotationAxis({ 1.0f, 1.0f, 1.0f });
    // cuboid.addImage(GetCurPath() + "/resource/container.jpeg");
    // cuboid.addImage(GetCurPath() + "/resource/awesomeface.png", true);

    Cubiod cuboid1(1.0f, 1.0f, 1.0f);
    cuboid1.setPosition(1.0f, -3.5f, 0.0f);
    cuboid1.setSize(0.5f, 2.0f, 0.3f);
    cuboid1.setScaleRatio(2.5f);
    cuboid1.setRotationAxis({ 1.0f, 1.0f, 1.0f });
    cuboid1.addImage(GetCurPath() + "/resource/wall.jpeg");
    // cuboid1.addImage(GetCurPath() + "/resource/awesomeface.png", true);

    while(!glfwWindowShouldClose(window))
    {
        ProcessInput(window, cameraCtrl);
        camera.enabel();
        light.enabel();

        cuboid.setRotation((float)glfwGetTime());
        cuboid1.setRotation(-(float)glfwGetTime());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rectangle.show();
        rectangle1.show();
        light.show();
        cuboid.show();
        cuboid1.show();

        for (int index = 0; index < cuboids.size(); ++index) {
            cuboids[index].show();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}
