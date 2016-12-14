#ifndef _DARKHOLE_H
#define _DARKHOLE_H
#include "gameObject.h"
#include "MeshRenderer.h"
#include "SphereCollider.h"
class DarkHole : public gameObject{
private:
	MeshRenderer _mesh;
	SphereCollider col;
	float t;
	bool isMove;
	float MoveLength;
public:
	DarkHole();
	virtual ~DarkHole();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};

#endif