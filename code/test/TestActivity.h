#ifndef _HEAD_FLAG_TEST_ACTIVITY_H
#define _HEAD_FLAG_TEST_ACTIVITY_H

#include "object/Model.h"
#include "object/LightSource.h"
#include "object/Skybox.h"
#include "shader/ColorTex3DShader.h"
#include "shader/ColorTex2DShader.h"
#include "shader/ColorTexMulilight3DShader.h"
#include "shader/ColorGeometryPointShader.h"
#include "shader/IncorporateColorTex3DShader.h"
#include "KeyboardEventHandler.h"
#include "Camera.h"


struct TestActivity {
    TestActivity(KeyboardEventHandler& keyboard);
    void render();

private:
    void initLights();
    void initCameras();
    void initDrawObjects();
    void buildSkybox();
    void buildGrass();
    void buildTransparentWindow();
    void buildWinMask();
    void buildCuboids();
    void buildRectangle3D();
    void buildMirror();
    void buildCircle();
    void buildNanosuit();
    void buildAirplan();
    void buildRichPoints();

    void mirrorRender();

private:
    KeyboardEventHandler& keyboardEventHandler;
    LightSource parallelLight;
    std::vector<LightSource> pointLights;
    CameraFPS cameraFPS;
    CameraFPS mirrorCameraFPS;
    LocalImage wallImage;
    LocalImage awesomefaceImage;
    LocalImage containerImage;
    LocalImage containerImage2;
    // LocalImage containerImage2_specular(GetCurPath() + "/resource/container2_specular.png");
    LocalImage containerImage2_specular;
    LocalImage matrixImage;
    LocalImage grassImage;
    LocalImage windowImage;
    CubeImage cubeImage;

    Skybox skybox;
    ColorTex3DShader rectangle;
    ColorTex3DShader rectangle1;
    IncorporateColorTex3DShader grass;
    ColorTex3DShader transparentWindow;
    ColorTex2DShader winMask;
    PaintImage mirrorCanva;
    ColorTex2DShader mirror;
    std::vector<ColorTexMulilight3DShader> cuboids;
    ColorTexMulilight3DShader cuboid;
    ColorTexMulilight3DShader cuboid1;
    ColorTex3DShader circle1;
    Model3DDrawObject nanosuit;
    Model3DDrawObject airplan;
    ColorGeometryPointShader richPoints;

    float deltaTime; // 当前帧与上一帧的时间差
    float lastFrame; // 上一帧的时间

    std::vector<Position> windowPositions;
};

#endif