#ifndef _HEAD_FLAG_TEST_ACTIVITY_H
#define _HEAD_FLAG_TEST_ACTIVITY_H

#include "object/Model.h"
#include "object/LightSource.h"
#include "object/Skybox.h"
#include "object/SkeletonModel3D.h"
#include "shader/ColorTex3DShader.h"
#include "shader/ColorTex2DShader.h"
#include "shader/ColorTexMulilight3DShader.h"
#include "shader/ColorGeometryPointShader.h"
#include "shader/IncorporateColorTex3DShader.h"
#include "KeyboardEventHandler.h"
#include "Camera.h"
#include "FrameRateTimer.h"
#include "collision/Collision.h"


struct TestActivity {
    TestActivity(KeyboardEventHandler& keyboard);
    void render();

private:
    void initLights();
    void initCameras();
    void initCollisionHandle();
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
    void buildCar();
    void buildRichPoints();
    void buildAirplanTrace();
    void buildFence();
    void registerKeyboardEvent(KeyboardEventHandler& keyboardEventHandler);

    void mirrorRender();
    void runAnimation();

    void renderSolidObjs();
    void renderTransparentObjs();

private:
    FrameRateTimer frameTimer;
    LightSource parallelLight;
    std::vector<LightSource> pointLights;
    CameraFPS cameraFPS;
    CameraFPS mirrorCameraFPS;

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
    Model3DDrawObject car;
    ColorGeometryPointShader richPoints;
    SkeletonModel3D airplanTrace;
    SkeletonModel3D fence;


    std::vector<Position> windowPositions;

    KeyboardEventHandler bodyKeyboardEventHandler;

    std::shared_ptr<CollisionBody> _light1Coll;
};

#endif