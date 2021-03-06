#ifndef _BOXCOLLDIER_H
#define _BOXCOLLIDER_H
#include "Collider.h"
#include "Primitive.h"
#include "MeshRenderer.h"
class AABBCollider : public Collider{
protected:
	AABB aabb;
	MeshRenderer mesh;
public:
	AABBCollider();
	~AABBCollider();
	virtual void start(gameObject* t);
	virtual void update();
	virtual void draw();
	virtual bool collide(gameObject &obj, D3DXVECTOR3 &collisionVector); //衝突しているかどうかを返す
	bool ToSphere(gameObject &obj, D3DXVECTOR3 &collisionVector);
	bool ToAABB(gameObject &obj, D3DXVECTOR3 &collisionVector);
	void setHL(D3DXVECTOR3 v){ aabb.hl = v; }//aabb.hlの代入
	D3DXVECTOR3 getHL(){ return aabb.hl; } //一応まだD3DXVECTOR3で返す
	AABB getAABB(){ return aabb; }

};

#endif