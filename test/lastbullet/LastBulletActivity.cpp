#include "LastBulletActivity.h"
#include "assets/UtilsFileSys.h"
#include <math.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "collision/Collision.h"


extern RenderShape cubeShape;
extern RenderShape tetrahedronShape;
extern RenderShape rectShape;
extern RenderShape circleShape;
extern void RegisterKeyboardHandler(KeyboardEventHandler* keyboard);

static LocalImage wallImage(GetCurPath() + "/resource/wall.jpeg");
static LocalImage awesomefaceImage(GetCurPath() + "/resource/awesomeface.png");
static LocalImage containerImage(GetCurPath() + "/resource/container.jpeg");
static LocalImage containerImage2(GetCurPath() + "/resource/container2.png");
static LocalImage containerImage2_specular(GetCurPath() + "/resource/lighting_maps_specular_color.png");
// static LocalImage containerImage2_specular(GetCurPath() + "/resource/container2_specular.png");;
static LocalImage matrixImage(GetCurPath() + "/resource/matrix.jpeg");
static LocalImage windowImage(GetCurPath() + "/resource/blending_transparent_window.png");
static CubeImage cubeImage(GetCurPath() + "/resource/skybox/right.jpg"
                        , GetCurPath() + "/resource/skybox/left.jpg"
                        , GetCurPath() + "/resource/skybox/bottom.jpg"
                        , GetCurPath() + "/resource/skybox/top.jpg"
                        , GetCurPath() + "/resource/skybox/front.jpg"
                        , GetCurPath() + "/resource/skybox/back.jpg");


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


LastBulletActivity::LastBulletActivity(KeyboardEventHandler& keyboard)
: frameTimer()
, parallelLight(true)
, pointLights({false, false})
, cameraFPS()
, mirrorCameraFPS()
, skybox()
, fence()
, bodyKeyboardEventHandler(keyboard) {
    initLights();
    initCameras();
    initDrawObjects();
    registerKeyboardEvent(keyboard);
    RegisterKeyboardHandler(&bodyKeyboardEventHandler);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_ZERO, GL_KEEP, GL_REPLACE);
    glDepthFunc(GL_LEQUAL);
    // glEnable(GL_PROGRAM_POINT_SIZE);
}

void LastBulletActivity::render() {
    frameTimer.updateTime();

    // TODO: 优化, 画布渲染完毕以后需要恢复原gl上下文状态, 如blend, cull_face等
    SetGlobalLights(parallelLight, pointLights);
    SetGlobalCamera(cameraFPS);

    runAnimation();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
    renderSolidObjs();
    renderTransparentObjs();
}

void LastBulletActivity::renderSolidObjs() {
    glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    for (auto light : pointLights) {
        light.show();
    }
    for (Bullet& bullet : bullets) {
        bullet.show();
    }

    // render skybox
    skybox.show(cameraFPS.getPosition());

    // glPointSize(100.0f);

    glDisable(GL_CULL_FACE);
    fence.show();
}

void LastBulletActivity::renderTransparentObjs() {
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // // TODO: 优化, 1. 抽取抠图流程pip  2. 打开窗口透明后草地透明也会透明, 需要设置细粒度开关
    // glDisable(GL_DEPTH_TEST);
    // glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
}

void LastBulletActivity::registerKeyboardEvent(KeyboardEventHandler& keyboardEventHandler) {
    constexpr float MOVE_SPEED = 2.5f;
    constexpr float TURN_SPEED = 10.0f;

    keyboardEventHandler.registerObserver(GLFW_KEY_W, GLFW_PRESS, [this]() { cameraFPS.goForward(frameTimer.getFrameTime() * MOVE_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_S, GLFW_PRESS, [this]() { cameraFPS.goBack(frameTimer.getFrameTime() * MOVE_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_A, GLFW_PRESS, [this]() { cameraFPS.goLeft(frameTimer.getFrameTime() * MOVE_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_D, GLFW_PRESS, [this]() { cameraFPS.goRight(frameTimer.getFrameTime() * MOVE_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_RIGHT, GLFW_PRESS, [this]() { cameraFPS.turnRight(frameTimer.getFrameTime() * TURN_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_LEFT, GLFW_PRESS, [this]() { cameraFPS.turnLeft(frameTimer.getFrameTime() * TURN_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_UP, GLFW_PRESS, [this]() { cameraFPS.turnUp(frameTimer.getFrameTime() * TURN_SPEED); });
    keyboardEventHandler.registerObserver(GLFW_KEY_DOWN, GLFW_PRESS, [this]() { cameraFPS.turnDown(frameTimer.getFrameTime() * TURN_SPEED); });

    keyboardEventHandler.registerObserver(GLFW_KEY_W, GLFW_PRESS, [this]() { mirrorCameraFPS.move(frameTimer.getFrameTime() * MOVE_SPEED * Vector3D(-1.0f, 0.0f, 0.0f)); });
    keyboardEventHandler.registerObserver(GLFW_KEY_S, GLFW_PRESS, [this]() { mirrorCameraFPS.move(frameTimer.getFrameTime() * MOVE_SPEED * Vector3D(1.0f, 0.0f, 0.0f)); });
    keyboardEventHandler.registerObserver(GLFW_KEY_A, GLFW_PRESS, [this]() { mirrorCameraFPS.move(frameTimer.getFrameTime() * MOVE_SPEED * Vector3D(0.0f, -1.0f, 0.0f)); });
    keyboardEventHandler.registerObserver(GLFW_KEY_D, GLFW_PRESS, [this]() { mirrorCameraFPS.move(frameTimer.getFrameTime() * MOVE_SPEED * Vector3D(0.0f, 1.0f, 0.0f)); });

    keyboardEventHandler.registerObserver(GLFW_KEY_J, GLFW_PRESS, [this]() { createBullet();});
}

void LastBulletActivity::createBullet() {
    static float lastCreateTime = 0;
    float curTime = frameTimer.getCurTime();
    if (curTime - lastCreateTime < 1.0f) {
        return;
    }
    lastCreateTime = curTime;

    bullets.emplace_back(cameraFPS.getPosition(), cameraFPS.getDirection());
    Bullet& bullet = bullets.back();
    bullet.enableCollision(true);
    const Position& pos = bullet.getPosition();
    const Vector3D& front = bullet.getFront();
    printf("bullet count=%lu, pos=(%f, %f, %f), front=(%f, %f, %f)\n", bullets.size(), pos.x, pos.y, pos.z, front.x, front.y, front.z);
};

void LastBulletActivity::runAnimation() {
    float timeStep = frameTimer.getFrameTime();
    CollisionSystem::getInstance().update(timeStep);
}

void LastBulletActivity::initLights() {
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

void LastBulletActivity::initCameras() {
    cameraFPS.setPosition({20.0f, 0.0f, 3.0f});
    cameraFPS.setAttitude(0.0f, 180.0f);

    mirrorCameraFPS = cameraFPS;
    mirrorCameraFPS.setFov(75.0f);
    mirrorCameraFPS.setAttitude(-80.0f, 180.0f);
    mirrorCameraFPS.setPosition({0.0f, 0.0f, 15.0f});
}

void LastBulletActivity::initDrawObjects() {
    buildSkybox();
    buildRectangle3D();
    buildFence();
}

void LastBulletActivity::buildSkybox() {
    skybox.setImage(cubeImage);
}

void LastBulletActivity::buildRectangle3D() {
}

void LastBulletActivity::buildFence() {
    // fence.setPosition({-2.0f, 3.0f, 5.0f});
    std::vector<RenderShape::Vertex> crossSection = {
        // 后(环回)
        {Position{0.0f, 0.0f, 0.0f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 0.0f, 1.0f}},
        // 底
        {Position{0.0f, 0.0f, 0.0f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{1.0f, 1.0f, 1.0f}},
        {Position{0.0f, 1.5f, 0.0f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{1.0f, 1.0f, 1.0f}},
        // 前
        {Position{0.0f, 1.5f, 0.5f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{1.0f, 1.0f, 0.0f}},
        {Position{0.0f, 1.0f, 0.8f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{1.0f, 0.5f, 0.0f}},
        {Position{0.0f, 1.0f, 1.5f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{1.0f, 0.3f, 0.0f}},
        // 顶
        {Position{0.0f, 1.0f, 1.5f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 1.0f, 1.0f}},
        {Position{0.0f, 0.0f, 1.5f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 1.0f, 1.0f}},
        // 后
        {Position{0.0f, 0.0f, 1.5f}, Vector3D{0.0f, 0.0f, 1.0f}, Vector2D{0.0f, 0.0f}, Color{0.0f, 0.0f, 1.0f}},
    };
    fence.setCrossSection(crossSection);

    std::vector<Position> laneMinLine = {
        {10.0f, -10.0f, 0.0f},
        {10.0f, 10.0f, 0.0f},

        {-10.f, 10.0f, 0.0f},
        {-10.0f, 2.0f, 0.0f},
        {-10.0f, 1.0f, 2.0f},
        {-10.0f, -1.0f, 2.0f},
        {-10.0f, -2.0f, 0.0f},
        {-10.0f, -10.0f, 0.0f},
        {10.0f, -10.0f, 0.0f},
        {10.0f, 10.0f, 0.0f},
    };
    fence.append(laneMinLine);
}
