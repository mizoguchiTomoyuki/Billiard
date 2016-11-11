#ifndef _CAMERA_H
#define _CAMERA_H
#include "gameObject.h"
#include "graphics.h"
class GameSceneManager;
class CameraFactory;
class Camera : public gameObject{
private:
	CameraData* _camera;
public:
	Camera();
	virtual ~Camera();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();
};


#include "GameSceneManager.h"
#include "cameraFactory.h"
#endif