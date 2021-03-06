#ifndef _PHYSICS_H
#define _PHYSICS_H
#include "Task.h"
//gameObjectの物理的な挙動を処理する
class Physics : public Task{
protected:
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 deltaV; //accell
public:
	Physics();
	~Physics();
	virtual void start(gameObject* t);
	virtual void update();
	virtual void draw();
	D3DXVECTOR3 getVelocity(){ return velocity; }
	D3DXVECTOR3 getdeltaV(){ return deltaV; }

};

#endif