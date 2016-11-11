#ifndef _LIGHT_H
#define _LIGHT_H
#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
class Light{
protected:
	_D3DLIGHT9 _light;
	D3DLIGHTTYPE _type;
	D3DCOLORVALUE _diffuse;
	D3DCOLORVALUE _specular;
	D3DCOLORVALUE _ambient;
	D3DVECTOR _pos;
	D3DVECTOR _direction;
	float _range;
	float _falloff;
	float _attnu0;
	float _attnu1;
	float _attnu2;
	float _theta;
	float _phi;
	ELEMID _id;
public:
	Light();
	virtual ~Light();
	virtual void initialize();
	virtual void update();
	virtual void SetRange(float r){ _range = r; }
	virtual void SetPosition(D3DVECTOR pos){ _pos = pos; }
	virtual void SetDirection(D3DVECTOR dir){ _direction = dir; }
	virtual void SetType(D3DLIGHTTYPE t){ _type = t; }
	virtual void setELEMID(ELEMID id){ _id = id; }
	virtual ELEMID getELEMID(){ return _id; }
	virtual _D3DLIGHT9 getlight(){ return _light; }

};


#endif