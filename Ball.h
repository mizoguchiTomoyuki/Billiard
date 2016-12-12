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
	bool isMove;
	float MoveLength;
public:
	Ball();
	Ball(const char* meshName);
	virtual ~Ball();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();
	void attenuation(); //運動の減衰を示すもの。現状汎用化手法が思いつかないのでBallクラスのみ実装する
	void Rolling();
	bool isMoving(){ return isMove; }
};

#endif