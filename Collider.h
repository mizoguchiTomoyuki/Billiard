#ifndef _COLLIDER_H
#define _COLLIDER_H
#include "Task.h"
namespace ColliderNS{
	enum COL_TYPE
	{
		SPHERE = 0,
		BOX = 1,
		AABB =2, //三次元空間の軸に平行なボックス
		END,
	};

}
//衝突判定を行うクラスの親クラス
//Taskを継承するが、ほかのTaskと一緒くたにせずcollision判定の中に入れる
class Collider : public Task{
protected:
	bool enable; //衝突判定を行うかどうかの判定
	D3DXVECTOR3 pos;
	ColliderNS::COL_TYPE _type; //相手のCOlliderの判定に用いる
	bool isVisible;
	Transform Coltransform; //colliderのトランスフォーム基本的にはgameObjectに準拠する
	bool bounceFlag;
	D3DXVECTOR3 cVector; //Bounce時の保持よう
	gameObject* bounceObj;
	bool Freeze;
public:
	Collider();
	~Collider();
	virtual void start(gameObject* t);
	virtual void update();
	virtual bool collide(gameObject &obj, D3DXVECTOR3 &collisionVector) = 0;//純粋仮想関数これをもとに作るBoxCollider,SphereColliderで実装
	virtual void draw();
	void SetEnable(bool b){ enable = b; }
	bool getEnable(){ return enable; }
	ColliderNS::COL_TYPE getType(){ return _type; }
	void bounce(gameObject &obj, D3DXVECTOR3 &collisionVector, D3DXVECTOR3 Sinking, D3DXVECTOR3 ref);
	void bounceOn(gameObject &obj, D3DXVECTOR3 &collisionVector);
	void SetFreeze(bool b){ Freeze = b; }

};


#endif