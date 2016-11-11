#include "PointLight.h"
PointLight::PointLight() : DirectionalLight(){

	SetObjClassName("PointLight");
	MyPrefab = PREFAB_MENU::POINTLIGHT;
	_light->SetType(D3DLIGHTTYPE::D3DLIGHT_POINT);
	_light->SetRange(10.0f);
}

PointLight::~PointLight(){
}

void PointLight::update(){

	DirectionalLight::update();
}

void PointLight::draw(){
	DirectionalLight::draw();
}

void PointLight::ai(){
	DirectionalLight::ai();

}
void PointLight::collision(){
	DirectionalLight::collision();
}

