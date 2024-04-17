#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "model/Rectangle.h"
#include "model/Cuboid.h"
#include "model/LightSource.h"
#include "Camera.h"
#include "Image.h"


static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window, CameraFPS& cameraFPS) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    static float lastFrame = 0.0f; // 上一帧的时间

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraFPS.goForward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraFPS.goBack(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraFPS.goLeft(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraFPS.goRight(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cameraFPS.turnRight(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cameraFPS.turnLeft(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraFPS.turnUp(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraFPS.turnDown(deltaTime);
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

// TODO: 优化, 抽取到类中
void SetCameraAndLightUniform(const CameraFPS& camera, const LightSource& light1, const LightSource& light2, const ShaderParallelLight& parallelLight) {
    std::vector<LightSource> lights;
    lights.push_back(light1);
    lights.push_back(light2);

    for (auto itr : ShaderProgram::getAllShaderProg())
    {
        if (!itr.first) {
            continue;
        }
        ShaderProgram& prog = *itr.first;
        prog.setCamera("camera", camera);
        prog.setParallelLight("parallelLight", parallelLight);
        prog.setLight("light", light1);
        SetUniforms(prog, "light", lights);
    }
}


int main() {
    GLFWwindow* window = InitWindows();
    if (window == nullptr) {
        return -1;
    }
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);

    CameraFPS cameraFPS;
    cameraFPS.setPosition(1.0f, 2.0f, 2.0f);
    cameraFPS.setAttitude(0.0f, 180.0f);

    LightSource light({-1.0f, 2.0f, 2.0f});
    light.setSize({0.5f, 0.5f, 0.5f});
    light.setDirection(Position(0.0f, 0.0f, 0.0f) - light.getPosition());
    // light.setColor(Color(0.33f, 0.42f, 0.18f));
    light.setColor(Color(1.0f, 0.0f, 0.0f));
    light.setSpotFacor(45.0f);
    light.setReach(50.0f);

    LightSource light1({1.0f, -2.0f, 2.0f});
    light1.setSize({0.5f, 0.5f, 0.5f});
    light1.setDirection(Position(0.0f, 0.0f, 2.0f) - light1.getPosition());
    light1.setColor(Color(0.0f, 1.0f, 0.0f));
    light1.setSpotFacor(12.0f);
    light1.setReach(100.0f);


    // TODO: 优化, 进一步封装 ShaderParallelLight, 逻辑上对齐 LightSource
    ShaderParallelLight parallelLight;
    // parallelLight.setColor({1.0f, 0.0f, 0.0f});
    parallelLight.setDirection({-1.0f, 1.0f, -1.0f});

    Image wallImage(GetCurPath() + "/resource/wall.jpeg");
    Image awesomefaceImage(GetCurPath() + "/resource/awesomeface.png", true);
    Image containerImage(GetCurPath() + "/resource/container.jpeg");
    Image containerImage2(GetCurPath() + "/resource/container2.png");
    // Image containerImage2_specular(GetCurPath() + "/resource/container2_specular.png");
    Image containerImage2_specular(GetCurPath() + "/resource/lighting_maps_specular_color.png", true);
    Image matrixImage(GetCurPath() + "/resource/matrix.jpeg");

    Rectangle rectangle(1.0f, 1.0f);
    rectangle.setPosition({0.0f, -0.5f});
    rectangle.setScaleRatio(0.5f);
    rectangle.setRotation(glm::radians(-45.0f));
    rectangle.setColor(Color(0.8f, 0.3f, 0.2f));
    rectangle.setImage(awesomefaceImage);

    Rectangle rectangle1(1.0f, 0.5f);
    rectangle1.setPosition({-1.0, 0.0f});
    rectangle1.setColor(Color(0.8f, 0.3f, 0.2f));
    // rectangle1.setImage(containerImage);

    Position cuboidPositions[10] = {
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
        cuboid.setPosition(cuboidPositions[index]);
        cuboid.addImage(awesomefaceImage);
        cuboid.addImage(containerImage);
        // cuboid.addImage(matrixImage);
        cuboids.push_back(cuboid);
    }

    ShaderMaterial material(containerImage2, containerImage2_specular);

    Cubiod cuboid(1.0f, 1.0f, 1.0f);
    cuboid.setPosition({0.0f, 2.0f, 0.0f});
    // cuboid.setColor(Color(1.0f, 0.5f, 0.31f));
    cuboid.setColor(Color(1.0f, 1.0f, 1.0f));
    cuboid.setScaleRatio(2.0f);
    cuboid.setRotationAxis({ 1.0f, 1.0f, 1.0f });
    // cuboid.addImage(containerImage);
    // cuboid.addImage(awesomefaceImage);
    cuboid.setMaterial(material);

    Cubiod cuboid1(1.0f, 1.0f, 1.0f);
    cuboid1.setPosition({1.0f, -3.5f, 0.0f});
    cuboid1.setSize({0.5f, 2.0f, 0.3f});
    cuboid1.setScaleRatio(2.5f);
    cuboid1.setRotationAxis({ 1.0f, 1.0f, 1.0f });
    cuboid1.addImage(wallImage);
    // cuboid1.setMaterial(material);

    float lastX = 0.0f;
    while(!glfwWindowShouldClose(window))
    {
        ProcessInput(window, cameraFPS);
        SetCameraAndLightUniform(cameraFPS, light, light1, parallelLight);

        // cuboid.setRotation((float)glfwGetTime());
        // cuboid1.setRotation(-(float)glfwGetTime());
        float ratio = sin((float)glfwGetTime());
        
        float x = ratio * 3;
        float y = sqrt(9.0f - x * x) * (lastX - x >=0.0f ? -1.0f : 1.0f);
        float z = ratio * 3;
        lastX = x;
        // light.setPosition({x, y, 3.0f});
        // light.setPosition({3.0f, 2.0f, z + 3.0f});
        // light.setDirection(Position(0.0f, 0.0f, z + light.getPosition().z) - light.getPosition());
        // light.setColor(Color(ratio * 2.0f, (1.0f - ratio) * 0.3f, (0.5 + ratio) * 1.7f));

        // light1.setPosition({x, y, 1.0f});
        // light1.setDirection(Position(0.0f, 0.0f, z + light1.getPosition().z) - light1.getPosition());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rectangle.show();
        // rectangle1.show();
        light.show();
        light1.show();
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
