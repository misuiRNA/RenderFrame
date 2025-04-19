#ifndef _HEAD_FLAG_LASTBULLET_ACTIVITY_H
#define _HEAD_FLAG_LASTBULLET_ACTIVITY_H

#include "object/Model.h"
#include "object/LightSource.h"
#include "object/Skybox.h"
#include "object/SkeletonModel3D.h"
#include "shader/ColorTex3DShader.h"
#include "shader/ColorTexMulilight3DShader.h"
#include "KeyboardEventHandler.h"
#include "Camera.h"
#include "FrameRateTimer.h"
#include "Bullet.h"
#include <memory>


struct LastBulletActivity {
    LastBulletActivity(KeyboardEventHandler& keyboard);
    void render();

private:
    void initLights();
    void initCameras();
    void initDrawObjects();
    void buildSkybox();
    void buildRectangle3D();
    void buildFence();
    void registerKeyboardEvent(KeyboardEventHandler& keyboardEventHandler);

    void runAnimation();

    void renderSolidObjs();
    void renderTransparentObjs();
    void createBullet();

private:
    FrameRateTimer frameTimer;
    LightSource parallelLight;
    std::vector<LightSource> pointLights;
    CameraFPS cameraFPS;
    CameraFPS mirrorCameraFPS;

    Skybox skybox;
    SkeletonModel3D fence;
    std::vector<Bullet> bullets;
    
    KeyboardEventHandler bodyKeyboardEventHandler;
};


#endif