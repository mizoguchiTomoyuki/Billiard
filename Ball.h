#ifndef _BALL_H
#define _BALL_H
#include "gameObject.h"
#include "MeshRenderer.h"
#include "SphereCollider.h"
class Ball : public gameObject{
private:
	MeshRenderer _mesh;
	SphereCollider col;
	float t;
public:
	Ball();
	virtual ~Ball();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};

#endif