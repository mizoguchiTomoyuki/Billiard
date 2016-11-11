#include "light.h"

Light::Light(){
	ZeroMemory(&_light, sizeof(D3DLIGHT9));
	_type = D3DLIGHT_DIRECTIONAL;
	_diffuse = D3DCOLORVALUE{ 1.0f, 1.0f, 1.0f };

	_specular = D3DCOLORVALUE{ 1.0f, 1.0f, 1.0f };

	_ambient = D3DCOLORVALUE{ 0.5f, 0.5f, 0.5f };

	_pos = D3DVECTOR{ 0.0f, 0.0f, 0.0f };

	_direction = D3DVECTOR{ -1.0f,-20.0f, 0.0f };
	_range = 1000.0f;
	_falloff = 1.0f;
	_attnu0 = 1.0f;
	_attnu1 = 0.0f;
	_attnu2 = 0.0f;
	_theta = 90.0f;
	_phi = 90.0f;
}

Light::~Light(){

}

void Light::initialize(){
	_light.Type = _type;
	_light.Diffuse = _diffuse;
	_light.Ambient = _ambient;
	//_light.Specular = _specular;
	//_light.Position = _pos;
	_light.Direction = _direction;
	_light.Range = _range;
	//_light.Falloff = _falloff;
	//_light.Attenuation0 = _attnu0;
	//_light.Attenuation1 = _attnu1;
	//_light.Attenuation2 = _attnu2;
	//_light.Theta = _theta;
	//_light.Phi = _phi;
}

void Light::update(){
	_light.Type = _type;
	_light.Diffuse = _diffuse;
	_light.Ambient = _ambient;
	//_light.Specular = _specular;
	//_light.Position = _pos;
	_light.Direction = _direction;
	_light.Range = _range;
	//_light.Falloff = _falloff;
	//_light.Attenuation0 = _attnu0;
	//_light.Attenuation1 = _attnu1;
	//_light.Attenuation2 = _attnu2;
	//_light.Theta = _theta;
	//_light.Phi = _phi;
	return;

}
