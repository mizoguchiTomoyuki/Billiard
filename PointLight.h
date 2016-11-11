#ifndef _POINTLIGHT_H
#define _POINTLIGHT_H
#include "DirectionalLight.h"
//DirectionalLight‚æ‚è‰º‘w
class PointLight : public DirectionalLight{
public:
	PointLight();
	virtual ~PointLight();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};
#endif