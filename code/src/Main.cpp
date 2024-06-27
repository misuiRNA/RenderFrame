#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "model/Rectangle3D.h"
#include "model/Rectangle2D.h"
#include "model/Cuboid.h"
#include "model/LightSource.h"
#include "model/Model3D.h"
#include "Camera.h"
#include "Image.h"
#include "Utils.h"
#include "KeyboardEventHandler.h"


static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

static void SetGlobalLights(const LightSource& parallelLight, const std::vector<LightSource>& pointLights) {
    for (auto itr : ShaderProgram::GetAllShaderProg())
    {
        if (!itr.first) {
            continue;
        }
        ShaderProgram& prog = *itr.first;
        prog.setLight("parallelLight", parallelLight);
        prog.SetUniformList("light", pointLights);
    }
}

static void SetGlobalCamera(const CameraFPS& camera) {
    for (auto itr : ShaderProgram::GetAllShaderProg())
    {
        if (!itr.first) {
            continue;
        }
        ShaderProgram& prog = *itr.first;
        prog.setCamera("camera", camera);
    }
}


// TODO: 优化, 设计模板测试类, 支持任意形状
static void EnableViewMask(Rectangle3D& outlineMask, Rectangle3D& throughMask) {
        glDepthMask(GL_FALSE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0x00);
        outlineMask.show();

        glStencilMask(0xFF);
        throughMask.show();

        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDepthMask(GL_TRUE);
}

static void SortWitDistance(std::vector<Position>& positions, Position centerPos) {
    auto dist = [](Position& a, Position& b) {
        Vector3D vec = b - a;
        double dist = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        return dist;
    };
    std::sort(positions.begin(), positions.end(), [&centerPos, &dist](Position& a, Position& b) {
        double distA = dist(a, centerPos);
        double distB = dist(b, centerPos);
        return distA > distB;
    });
}


int main() {
    GLFWwindow* window = InitWindows();
    if (window == nullptr) {
        return -1;
    }
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_ZERO, GL_KEEP, GL_REPLACE);

    KeyboardEventHandler keyboardEventHandler([window](int keyCode, int eventCode){ return glfwGetKey(window, keyCode) == eventCode; });
    keyboardEventHandler.registerObserver(GLFW_KEY_ESCAPE, GLFW_PRESS, [window]() { glfwSetWindowShouldClose(window, true); });

    CameraFPS cameraFPS;
    cameraFPS.setPosition(5.0f, 2.0f, 2.0f);
    cameraFPS.setAttitude(0.0f, 180.0f);

    // TODO 优化, 键盘事件不依赖帧率
    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间
    keyboardEventHandler.registerObserver(GLFW_KEY_W, GLFW_PRESS, [&cameraFPS, &deltaTime]() { cameraFPS.goForward(deltaTime); });
    keyboardEventHandler.registerObserver(GLFW_KEY_S, GLFW_PRESS, [&cameraFPS, &deltaTime]() { cameraFPS.goBack(deltaTime); });
    keyboardEventHandler.registerObserver(GLFW_KEY_A, GLFW_PRESS, [&cameraFPS, &deltaTime]() { cameraFPS.goLeft(deltaTime); });
    keyboardEventHandler.registerObserver(GLFW_KEY_D, GLFW_PRESS, [&cameraFPS, &deltaTime]() { cameraFPS.goRight(deltaTime); });
    keyboardEventHandler.registerObserver(GLFW_KEY_RIGHT, GLFW_PRESS, [&cameraFPS, &deltaTime]() { cameraFPS.turnRight(deltaTime); });
    keyboardEventHandler.registerObserver(GLFW_KEY_LEFT, GLFW_PRESS, [&cameraFPS, &deltaTime]() { cameraFPS.turnLeft(deltaTime); });
    keyboardEventHandler.registerObserver(GLFW_KEY_UP, GLFW_PRESS, [&cameraFPS, &deltaTime]() { cameraFPS.turnUp(deltaTime); });
    keyboardEventHandler.registerObserver(GLFW_KEY_DOWN, GLFW_PRESS, [&cameraFPS, &deltaTime]() { cameraFPS.turnDown(deltaTime); });


    LightSource parallelLight(true);
    parallelLight.setDirection({-1.0f, 1.0f, -1.0f});
    // parallelLight.setColor({1.0f, 0.0f, 0.0f});

    std::vector<LightSource> pointLights;
    pointLights.emplace_back(false);
    pointLights.emplace_back(false);

    LightSource& light = pointLights[0];
    light.setPosition({-1.0f, 2.0f, 2.0f});
    light.setSize({0.5f, 0.5f, 0.5f});
    light.setDirection(Position(0.0f, 0.0f, 0.0f) - light.getPosition());
    // light.setColor(Color(0.33f, 0.42f, 0.18f));
    light.setColor(Color(1.0f, 0.0f, 0.0f));
    light.setSpotFacor(45.0f);
    light.setReach(50.0f);

    LightSource& light1 = pointLights[1];
    light1.setPosition({1.0f, -2.0f, 2.0f});
    light1.setSize({0.5f, 0.5f, 0.5f});
    light1.setDirection(Position(0.0f, 0.0f, 2.0f) - light1.getPosition());
    light1.setColor(Color(0.0f, 1.0f, 0.0f));
    light1.setSpotFacor(12.0f);
    light1.setReach(100.0f);


    LocalImage wallImage(GetCurPath() + "/resource/wall.jpeg");
    LocalImage awesomefaceImage(GetCurPath() + "/resource/awesomeface.png");
    LocalImage containerImage(GetCurPath() + "/resource/container.jpeg");
    LocalImage containerImage2(GetCurPath() + "/resource/container2.png");
    // LocalImage containerImage2_specular(GetCurPath() + "/resource/container2_specular.png");
    LocalImage containerImage2_specular(GetCurPath() + "/resource/lighting_maps_specular_color.png");
    LocalImage matrixImage(GetCurPath() + "/resource/matrix.jpeg");
    LocalImage grassImage(GetCurPath() + "/resource/grass.png");
    LocalImage windowImage(GetCurPath() + "/resource/blending_transparent_window.png");

    Rectangle3D rectangle({0.6f, 0.6f});
    rectangle.setPosition({0.0f, 0.0f});
    // rectangle.setColor(Color(0.8f, 0.3f, 0.2f));
    rectangle.setColor(Color(0.0f, 0.0f, 0.0f));
    rectangle.setImage(awesomefaceImage);
    rectangle.getAttituedeCtrl()
             .setFront({1.0f, 0.0f, 0.0f});

    Rectangle3D rectangle1({1.0f, 1.0f});
    rectangle1.setPosition({-1.0, 0.0f});
    rectangle1.setColor(Color(0.8f, 0.3f, 0.2f));
    // rectangle1.setImage(containerImage);
    // rectangle1.setPosition({0.0f, 0.0f});

    Rectangle3D grass({1.0f, 1.0f});
    grass.setPosition({0.0f, 0.0f});
    grass.setImage(grassImage);
    grass.getAttituedeCtrl()
         .setFront({1.0f, 0.0f, 0.0f});
    std::vector<Position> grassPositions = {
        {-5.0f,  1.5f,  0.0f},
        {-4.8f, -1.5f,  0.0f},
        {-6.0f,  1.9f,  0.0f},
        {-6.5f,  2.5f,  0.0f},
        {-4.0f,  1.5f,  0.0f},
        {-4.0f,  5.5f,  0.0f},
        {-5.0f,  3.0f,  0.0f},
        {-4.0f, -1.5f,  0.0f},
        {-1.5f, -2.5f,  0.0f},
        {-3.0f, -4.5f,  0.0f},
        {-4.0f,  1.5f,  0.0f},
        {-9.0f,  2.0f,  0.0f},
        {-8.0f,  1.5f,  0.0f},
        {-4.0f,  1.8f,  0.0f},
        {-7.0f,  1.0f,  0.0f}
    };

    Rectangle3D transparentWindow({2.0f, 2.0f});
    transparentWindow.setPosition({0.0f, 0.0f});
    transparentWindow.setImage(windowImage);
    transparentWindow.getAttituedeCtrl()
                     .setFront({1.0f, 0.0f, 0.0f});
    std::vector<Position> windowPositions = {
        {1.1f, 1.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 3.0f, 0.0f},
        {5.0f, 4.0f, 0.0f},
    };

    PaintImage mirrorCanva(800, 600);
    mirrorCanva.setBackgroundColor({0.3f, 0.2f, 0.3f});

    Rectangle2D mirror(0.8f, 0.6f);
    mirror.setPosition({-0.6f, 0.7f});
    mirror.setColor(Color(0.8f, 0.3f, 0.2f));
    mirror.setImage(mirrorCanva);

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

    std::vector<Cuboid> cuboids;
    cuboids.reserve(10);
    for (int index = 0; index < 10; ++index) {
        cuboids.emplace_back(Vector3D(1.0f, 1.0f, 1.0f));
        Cuboid& cuboid = cuboids.back();
        cuboid.setPosition(cuboidPositions[index]);
        // cuboid.addImage(mirrorCanva);
        cuboid.addImage(awesomefaceImage);
        cuboid.addImage(containerImage);
        // cuboid.addImage(matrixImage);
    }

    ShaderMaterial material(containerImage2.getTexture(ImageWrapMode::Repeat), containerImage2_specular.getTexture(ImageWrapMode::Repeat));

    Cuboid cuboid({2.0f, 2.0f, 2.0f});
    cuboid.setPosition({0.0f, 2.0f, 0.0f});
    // cuboid.setColor(Color(1.0f, 0.5f, 0.31f));
    cuboid.setColor(Color(1.0f, 1.0f, 1.0f));
    // cuboid.addImage(containerImage);
    // cuboid.addImage(awesomefaceImage);
    cuboid.setMaterial(material);

    Cuboid cuboid1({1.0f, 1.0f, 1.0f});
    cuboid1.setPosition({1.0f, -3.5f, 0.0f});
    cuboid1.setSize({1.25f, 5.0f, 0.75f});
    cuboid1.addImage(wallImage);
    // cuboid1.setMaterial(material);
    // cuboid1.getAttituedeCtrl().setFront({0.0f, 1.0f, 0.0f});

    Model3D nanosuit(GetCurPath() + "/resource/models/nanosuit/nanosuit.obj");
    nanosuit.setSize({0.1, 0.1, 0.1});
    nanosuit.setPosition({0.0f, 1.5f, 1.5f});
    nanosuit.getAttituedeCtrl()
            .setFront({1.0f, 0.0f, 0.0f});

    Model3D airplan(GetCurPath() + "/resource/models/Airplane/11803_Airplane_v1_l1.obj");
    airplan.setSize({0.001, 0.001, 0.001});
    airplan.setPosition({0.0f, 5.0f, 1.5f});
    airplan.getAttituedeCtrl()
           .setUp({0.0f, 1.0f, 0.0f})
           .setFront({0.0f, 0.0f, 1.0f});


    float lastX = 0.0f;
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        keyboardEventHandler.proc();
        SetGlobalLights(parallelLight, pointLights);
        SetGlobalCamera(cameraFPS);

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

        // l3DModel.setPosition({x, y, 1.5f});
        // l3DModel.setFront({x, y, 0.0f});
        // airplan.setFront({0.0f, y, x});
        rectangle.setPosition({x, y, z});
        rectangle1.setPosition({x - 1.0f, y, z});

        cuboid1.getAttituedeCtrl().setFront({x, 0.0f, y});

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

        // EnableViewMask(rectangle1, rectangle);

        // TODO: 优化, 封装到RenderData中, 渲染元素级别控制面剔除方式
        glEnable(GL_CULL_FACE);
        // glFrontFace(GL_CW);

        rectangle1.show();
        rectangle.show();

        light.show();
        light1.show();
        cuboid.show();
        cuboid1.show();

        nanosuit.show();
        airplan.show();

        for (int index = 0; index < cuboids.size(); ++index) {
            cuboids[index].show();
            nanosuit.setPosition({-index + 0.1f, 1.5f, 1.5f});
            nanosuit.show();
        }

        // TODO: 优化, 抽取透明元素的绘制流程pip
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (int index = 0; index < grassPositions.size(); ++index) {
            grass.setPosition(grassPositions[index]);
            grass.show();
        }

        // TODO: 优化, 受混合+深度测试影响 透明物体需要按顺序绘制, 需要提供一个排序工具
        SortWitDistance(windowPositions, ((const ShaderCamera&)cameraFPS).getPosition());
        for (int index = 0; index < windowPositions.size(); ++index) {
            transparentWindow.setPosition(windowPositions[index]);
            transparentWindow.show();
        }
        glDisable(GL_BLEND);


        auto painter = [&]() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            const ShaderCamera& cam = cameraFPS;
            CameraFPS mirrorCameraFPS = cameraFPS;
            mirrorCameraFPS.setPosition(cam.getPosition().x + 5, cam.getPosition().y, 15.0f);
            mirrorCameraFPS.setAttitude(-80.0f, 180.0f);
            SetGlobalCamera(mirrorCameraFPS);


            rectangle1.show();
            rectangle.show();

            light.show();
            light1.show();
            cuboid.show();
            cuboid1.show();

            nanosuit.show();
            airplan.show();

            for (int index = 0; index < cuboids.size(); ++index) {
                cuboids[index].show();
                nanosuit.setPosition({-index + 0.1f, 1.5f, 1.5f});
                nanosuit.show();
            }

            // TODO: 优化, 抽取透明元素的绘制流程pip
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            for (int index = 0; index < grassPositions.size(); ++index) {
                grass.setPosition(grassPositions[index]);
                grass.show();
            }

            // TODO: 优化, 受混合+深度测试影响 透明物体需要按顺序绘制, 需要提供一个排序工具
            SortWitDistance(windowPositions, ((const ShaderCamera&)mirrorCameraFPS).getPosition());
            for (int index = 0; index < windowPositions.size(); ++index) {
                transparentWindow.setPosition(windowPositions[index]);
                transparentWindow.show();
            }
            glDisable(GL_BLEND);

            SetGlobalCamera(cameraFPS);
        };
        mirrorCanva.paint(painter);

        mirror.show();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
