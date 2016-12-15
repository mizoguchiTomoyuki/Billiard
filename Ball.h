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
	bool isFall;
	bool Fallcheck;
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
	void Holedown();
	bool getisFall(){ return isFall; };
	bool getisFallcheck(){ return Fallcheck; };
	void FallCheck(){ Fallcheck = true; isMove = false; }//チェック完了を通知
	void FallCheckReset(){ Fallcheck = false; isMove = true; }
	void setVisible(bool b){ _mesh.isVisible(b); }
	void SetTrigger(bool b){ col.setTrigger(b); }
	void setFall(bool b){ isFall = b; }
	void resetBall(){
		velocity = { 0, 0, 0 };
		deltaV = { 0, 0, 0 };

	}
};

#endif