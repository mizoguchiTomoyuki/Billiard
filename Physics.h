#ifndef _PHYSICS_H
#define _PHYSICS_H
#include "Task.h"
//gameObject‚Ì•¨—“I‚È‹““®‚ğˆ—‚·‚é
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