#include "DirectionalLight.h"
DirectionalLight::DirectionalLight() : gameObject(){
	transform.position = { 0.0f, 0.0f,0.0f };
	SetObjClassName("DirectionalLight");
	MyPrefab = PREFAB_MENU::DIRECTIONALLIGHT;
	_light = new Light();
	_light_id = LightFactory::Instance().CreateElem(*_light);
	_light->initialize();
	VECTOR3 di = { 0, 20.0f, 7 };
	_light->SetDirection(di);
}

DirectionalLight::~DirectionalLight(){
}

void DirectionalLight::update(){

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	velocity += deltaV;
	deltaV.x = 0;
	deltaV.y = 0;
	deltaV.z = 0;
	
	transform.position.x += velocity.x*frameTime;
	transform.position.y += velocity.y*frameTime;
	transform.position.z += velocity.z*frameTime;
	_light->SetPosition(transform.position);
	recordupdate();
}

void DirectionalLight::draw(){
}

void DirectionalLight::ai(){

}
void DirectionalLight::collision(){
}

