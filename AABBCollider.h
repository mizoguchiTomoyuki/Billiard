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
	virtual bool collide(gameObject &obj, D3DXVECTOR3 &collisionVector); //Õ“Ë‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ğ•Ô‚·
	bool ToSphere(gameObject &obj, D3DXVECTOR3 &collisionVector);
	bool ToAABB(gameObject &obj, D3DXVECTOR3 &collisionVector);
	void setHL(D3DXVECTOR3 v){ aabb.hl = v; }//aabb.hl‚Ì‘ã“ü
	D3DXVECTOR3 getHL(){ return aabb.hl; } //ˆê‰‚Ü‚¾D3DXVECTOR3‚Å•Ô‚·
	AABB getAABB(){ return aabb; }

};

#endif