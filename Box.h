#ifndef _BOX_H
#define _BOX_H
#include "gameObject.h"
#include "MeshRenderer.h"
#include "AABBCollider.h"
class Box : public gameObject{
private:
	MeshRenderer _mesh;
	AABBCollider col;
	float t;
public:
	Box();
	virtual ~Box();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};

#endif