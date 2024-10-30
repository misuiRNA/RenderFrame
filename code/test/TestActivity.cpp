#include "TestActivity.h"
#include <math.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>


extern RenderShape cubeShape;
extern RenderShape tetrahedronShape;
extern RenderShape rectShape;
extern RenderShape circleShape;

constexpr float MOVE_SPEED = 2.5f;
constexpr float TURN_SPEED = 10.0f;


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



// TODO: 优化, 设计模板测试类, 支持任意形状
static void EnableViewMask(ColorTex3DShader& outlineMask, ColorTex3DShader& throughMask) {
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



TestActivity::TestActivity(KeyboardEventHandler& keyboard)
: keyboardEventHandler(keyboard)
, parallelLight(true)
, pointLights({false, false})
, cameraFPS()
, mirrorCameraFPS()
, wallImage(GetCurPath() + "/resource/wall.jpeg")
, awesomefaceImage(GetCurPath() + "/resource/awesomeface.png")
, containerImage(GetCurPath() + "/resource/container.jpeg")
, containerImage2(GetCurPath() + "/resource/container2.png")
, containerImage2_specular(GetCurPath() + "/resource/lighting_maps_specular_color.png")
, matrixImage(GetCurPath() + "/resource/matrix.jpeg")
, grassImage(GetCurPath() + "/resource/grass.png")
, windowImage(GetCurPath() + "/resource/blending_transparent_window.png")
, cubeImage(GetCurPath() + "/resource/skybox/right.jpg"
          , GetCurPath() + "/resource/skybox/left.jpg"
          , GetCurPath() + "/resource/skybox/bottom.jpg"
          , GetCurPath() + "/resource/skybox/top.jpg"
          , GetCurPath() + "/resource/skybox/front.jpg"
          , GetCurPath() + "/resource/skybox/back.jpg")
, skybox()
, rectangle({0.6f, 0.6f})
, rectangle1({1.0f, 1.0f})
, grass({1.0f, 1.0f})
, transparentWindow({2.0f, 2.0f})
, winMask(2.0f, 2.0f)
, mirrorCanva(800, 600, 4)
, mirror(0.8f, 0.6f)
, cuboids()
, cuboid({2.0f, 2.0f, 2.0f})
, cuboid1({1.0f, 1.0f, 1.0f})
, circle1({1.0f, 1.0f, 1.0f})
, nanosuit(GetCurPath() + "/resource/models/nanosuit/nanosuit.obj")
, airplan(GetCurPath() + "/resource/models/Airplane/11803_Airplane_v1_l1.obj")
, richPoints()
, deltaTime(0.0f)
, lastFrame(0.0f) {
    initLights();
    initCameras();
    initDrawObjects();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_ZERO, GL_KEEP, GL_REPLACE);
    glDepthFunc(GL_LEQUAL);
    // glEnable(GL_PROGRAM_POINT_SIZE);
}

void TestActivity::render() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    SetGlobalLights(parallelLight, pointLights);
    SetGlobalCamera(cameraFPS);

    float time = (float)glfwGetTime();
    float radian = (sin(time)) * M_PI * (cos(time) > 0 ? 1 : -1);
    float sinV = sin(radian);
    float cosV = cos(radian);
    float x = sinV * 3;
    float y = cosV * 3;
    float z = 3.0f;

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

    // cuboid1.getAttituedeCtrl().setFront({x, 0.0f, y});


    mirrorCanva.paint(std::bind(&TestActivity::mirrorRender, this));


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 


    // EnableViewMask(rectangle1, rectangle);

    // TODO: 优化, 封装到RenderData中, 渲染元素级别控制面剔除方式
    glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);

    rectangle1.show();
    rectangle.show();

    for (auto light : pointLights) {
        light.show();
    }
    cuboid.show();
    cuboid1.show();
    circle1.show();

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
    grass.show();

    // TODO: 优化, 受混合+深度测试影响 透明物体需要按顺序绘制, 需要提供一个排序工具
    SortWitDistance(windowPositions, ((const ShaderCamera&)cameraFPS).getPosition());
    for (int index = 0; index < windowPositions.size(); ++index) {
        transparentWindow.setPosition(windowPositions[index]);
        transparentWindow.show();
    }
    glDisable(GL_BLEND);
    mirror.show();

    // render skybox
    skybox.show(cameraFPS.getPosition());

    // glPointSize(100.0f);
    // richPoints.show();

    // TODO: 优化, 1. 抽取抠图流程pip  2. 打开窗口透明后草地透明也会透明, 需要设置细粒度开关
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    winMask.show();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

}

void TestActivity::mirrorRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    SetGlobalCamera(mirrorCameraFPS);

    rectangle1.show();
    rectangle.show();

    for (auto light : pointLights) {
        light.show();
    }
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
    grass.show();

    // TODO: 优化, 受混合+深度测试影响 透明物体需要按顺序绘制, 需要提供一个排序工具
    SortWitDistance(windowPositions, ((const ShaderCamera&)mirrorCameraFPS).getPosition());
    for (int index = 0; index < windowPositions.size(); ++index) {
        transparentWindow.setPosition(windowPositions[index]);
        transparentWindow.show();
    }
    glDisable(GL_BLEND);

    // render skybox
    skybox.show(mirrorCameraFPS.getPosition());

    SetGlobalCamera(cameraFPS);
}

void TestActivity::initLights() {
    parallelLight.setDirection({-1.0f, 1.0f, -1.0f});
    // parallelLight.setColor({1.0f, 0.0f, 0.0f});

    LightSource& light = pointLights[0];
    light.setVertexData(tetrahedronShape);
    light.setPosition({-1.0f, 2.0f, 2.0f});
    light.setSize({0.5f, 0.5f, 0.5f});
    light.setDirection(Position(0.0f, 0.0f, 0.0f) - light.getPosition());
    // light.setColor(Color(0.33f, 0.42f, 0.18f));
    light.setColor(Color(1.0f, 0.0f, 0.0f));
    light.setSpotFacor(45.0f);
    light.setReach(50.0f);

    LightSource& light1 = pointLights[1];
    light1.setVertexData(cubeShape);
    light1.setPosition({1.0f, -2.0f, 2.0f});
    light1.setSize({0.5f, 0.5f, 0.5f});
    light1.setDirection(Position(0.0f, 0.0f, 2.0f) - light1.getPosition());
    light1.setColor(Color(0.0f, 1.0f, 0.0f));
    light1.setSpotFacor(12.0f);
    light1.setReach(100.0f);
}

void TestActivity::initCameras() {
    cameraFPS.setPosition({5.0f, 2.0f, 2.0f});
    cameraFPS.setAttitude(0.0f, 180.0f);
    keyboardEventHandler.registerObserver(GLFW_KEY_W, GLFW_PRESS, [this]() { cameraFPS.goForward(deltaTime * MOVE_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_S, GLFW_PRESS, [this]() { cameraFPS.goBack(deltaTime * MOVE_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_A, GLFW_PRESS, [this]() { cameraFPS.goLeft(deltaTime * MOVE_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_D, GLFW_PRESS, [this]() { cameraFPS.goRight(deltaTime * MOVE_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_RIGHT, GLFW_PRESS, [this]() { cameraFPS.turnRight(deltaTime * TURN_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_LEFT, GLFW_PRESS, [this]() { cameraFPS.turnLeft(deltaTime * TURN_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_UP, GLFW_PRESS, [this]() { cameraFPS.turnUp(deltaTime * TURN_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_DOWN, GLFW_PRESS, [this]() { cameraFPS.turnDown(deltaTime * TURN_SPEED); });

    mirrorCameraFPS = cameraFPS;
    mirrorCameraFPS.setFov(75.0f);
    mirrorCameraFPS.setAttitude(-80.0f, 180.0f);
    mirrorCameraFPS.setPosition({0.0f, 0.0f, 15.0f});
    keyboardEventHandler.registerObserver(GLFW_KEY_W, GLFW_PRESS, [this]() { mirrorCameraFPS.move(deltaTime * MOVE_SPEED * Vector3D(-1.0f, 0.0f, 0.0f)); });
    keyboardEventHandler.registerObserver(GLFW_KEY_S, GLFW_PRESS, [this]() { mirrorCameraFPS.move(deltaTime * MOVE_SPEED * Vector3D(1.0f, 0.0f, 0.0f)); });
    keyboardEventHandler.registerObserver(GLFW_KEY_A, GLFW_PRESS, [this]() { mirrorCameraFPS.move(deltaTime * MOVE_SPEED * Vector3D(0.0f, -1.0f, 0.0f)); });
    keyboardEventHandler.registerObserver(GLFW_KEY_D, GLFW_PRESS, [this]() { mirrorCameraFPS.move(deltaTime * MOVE_SPEED * Vector3D(0.0f, 1.0f, 0.0f)); });
}

void TestActivity::initDrawObjects() {
    buildSkybox();
    buildRectangle3D();
    buildGrass();
    buildTransparentWindow();
    buildWinMask();
    buildCuboids();
    buildMirror();
    buildCircle();
    buildNanosuit();
    buildAirplan();
    buildRichPoints();
}

void TestActivity::buildSkybox() {
    skybox.setImage(cubeImage);
}

void TestActivity::buildGrass() {
    grass.setVertexData(rectShape);
    grass.setPosition({-1.0f, 3.0f, 2.0f});
    grass.setImage(grassImage);
    grass.getAttituedeCtrl()
         .setFront({1.0f, 0.0f, 0.0f});
    std::vector<IncorporateColorTex3DShader> grasses;
    grasses.reserve(100 * 100);
    for (int x = -100; x <= 100; ++x) {
        for (int y = -100; y <= 100; ++y) {
            grasses.emplace_back(grass);
            grasses.back().setPosition({x * 0.5f, y * 0.5f, 0.0f});
        }
    }
    grass.mergeCopies(grasses);
}

void TestActivity::buildTransparentWindow() {
    transparentWindow.setVertexData(rectShape);
    transparentWindow.setPosition({0.0f, 0.0f});
    transparentWindow.setImage(windowImage);
    transparentWindow.getAttituedeCtrl()
                     .setFront({1.0f, 0.0f, 0.0f});
    windowPositions = {
        {1.1f, 1.0f, 0.0f},
        {3.0f, 2.0f, 0.0f},
        {4.0f, 3.0f, 0.0f},
        {5.0f, 4.0f, 0.0f},
    };
}

void TestActivity::buildWinMask() {
    winMask.setVertexData(rectShape);
    winMask.setImage(awesomefaceImage);
}

void TestActivity::buildCuboids() {
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
    cuboids.reserve(10);
    for (int index = 0; index < 10; ++index) {
        cuboids.emplace_back(Vector3D(1.0f, 1.0f, 1.0f));
        ColorTexMulilight3DShader& cuboid = cuboids.back();
        cuboid.setPosition(cuboidPositions[index]);
        // cuboid.setPrimaryImage(mirrorCanva);
        cuboid.setPrimaryImage(awesomefaceImage);
        cuboid.setSecondaryImage(containerImage);
        // cuboid.setSecondaryImage(matrixImage);
        cuboid.setVertexData(cubeShape);
    }


    ShaderMaterial material(containerImage2.getTexture(ImageWrapMode::Repeat), containerImage2_specular.getTexture(ImageWrapMode::Repeat));
    cuboid.setPosition({0.0f, 2.0f, 0.0f});
    // cuboid.setColor(Color(1.0f, 0.5f, 0.31f));
    cuboid.setColor(Color(1.0f, 1.0f, 1.0f));
    // cuboid.setPrimaryImage(containerImage);
    // cuboid.setSecondaryImage(awesomefaceImage);
    cuboid.setMaterial(material);
    cuboid.setVertexData(cubeShape);


    cuboid1.setPosition({1.0f, -3.5f, 0.0f});
    cuboid1.setSize({1.25f, 5.0f, 0.75f});
    cuboid1.setPrimaryImage(wallImage);
    // cuboid1.setMaterial(material);
    // cuboid1.getAttituedeCtrl().setFront({0.0f, 1.0f, 0.0f});
    cuboid1.setVertexData(tetrahedronShape);
}

void TestActivity::buildRectangle3D() {
    rectangle.setVertexData(rectShape);
    rectangle.setPosition({0.0f, 0.0f});
    // rectangle.setColor(Color(0.8f, 0.3f, 0.2f));
    rectangle.setColor(Color(0.0f, 0.0f, 0.0f));
    rectangle.setImage(awesomefaceImage);
    rectangle.getAttituedeCtrl()
             .setFront({1.0f, 0.0f, 0.0f});


    rectangle1.setVertexData(rectShape);
    rectangle1.setPosition({-1.0, 0.0f});
    rectangle1.setColor(Color(0.8f, 0.3f, 0.2f));
    // rectangle1.setImage(containerImage);
    // rectangle1.setPosition({0.0f, 0.0f});
}

void TestActivity::buildMirror() {
    mirrorCanva.setBackgroundColor({0.3f, 0.2f, 0.3f});
    mirror.setVertexData(rectShape);
    mirror.setPosition({-0.6f, 0.7f});
    mirror.setColor(Color(0.8f, 0.3f, 0.2f));
    mirror.setImage(mirrorCanva);
}

void TestActivity::buildCircle() {
    circle1.setDrawMode(RenderDataMode::LINE_STRIP);
    circle1.setPosition({-1.0f, 3.0f, 2.0f});
    circle1.setVertexData(circleShape);
    circle1.setColor(Color(0.0f, 1.0f, 0.0f));
    circle1.getAttituedeCtrl().setFront({1.0f, 0.0f, 0.0f}).setUp({0.0f, 0.0f, 1.0f});
}

void TestActivity::buildNanosuit() {
    nanosuit.setSize({0.1, 0.1, 0.1});
    nanosuit.setPosition({0.0f, 1.5f, 1.5f});
    nanosuit.setAttituedeCtrl({0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f});
}

void TestActivity::buildAirplan() {
    airplan.setSize({0.001, 0.001, 0.001});
    airplan.setPosition({0.0f, 5.0f, 1.5f});
    airplan.setAttituedeCtrl({0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
}

void TestActivity::buildRichPoints() {
    richPoints.setVertexData(rectShape);
}
