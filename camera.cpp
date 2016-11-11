#include "camera.h"
Camera::Camera() : gameObject(){

	transform.position = { 0.0f, 30.0f, 0.0f };
	SetObjClassName("Camera");

	MyPrefab = PREFAB_MENU::CAMERA;
	_camera = new CameraData();

	_camera->aspect = 640.0f / 480.0f;
	_camera->farClip = 10000.0f;
	_camera->nearClip = 1.0f;
	_camera->fovY = 1.0f;
	_camera->orthographics = false;

	_camera->Up = { 0.0f, 0.0f, 1.0f };
	_camera->initialized = true;
	_camera->position = transform.position;
	_camera->LookAt = { 0, 0, 0 };
	CameraFactory::Instance().CreateElem(*_camera);
}

Camera::~Camera(){
}

void Camera::update(){

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	velocity += deltaV;
	deltaV.x = 0;
	deltaV.y = 0;
	deltaV.z = 0;
	transform.position.x += velocity.x*frameTime;
	transform.position.y += velocity.y*frameTime;
	transform.position.z += velocity.z*frameTime;

	_camera->position = transform.position;

	recordupdate();
}

void Camera::draw(){
}

void Camera::ai(){

}
void Camera::collision(){
}

