#ifndef _PHYSICS_H
#define _PHYSICS_H
#include "Task.h"
//gameObjectの物理的な挙動を処理する
class Physics : public Task{
protected:

public:
	Physics();
	~Physics();
	virtual void start(gameObject* t);
	virtual void update();
	virtual void draw();

}

#endif