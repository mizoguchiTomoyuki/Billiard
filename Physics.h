#ifndef _PHYSICS_H
#define _PHYSICS_H
#include "Task.h"
//gameObject�̕����I�ȋ�������������
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