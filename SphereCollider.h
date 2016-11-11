#ifndef _SPHERE_COLLIDER_H
#define _SPHERE_COLLIDER_H
#include "Collider.h"
#include "Primitive.h"
#include "MeshRenderer.h"
class SphereCollider : public Collider{
protected:
	Sphere sphe;
	MeshRenderer mesh;
public:
	SphereCollider();
	~SphereCollider();
	virtual void start(gameObject* t);
	virtual void update();
	virtual void draw();
	virtual bool collide(gameObject &obj, D3DXVECTOR3 &collisionVector); //è’ìÀÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©Çï‘Ç∑
	bool ToSphere(gameObject &obj, D3DXVECTOR3 &collisionVector);
	bool ToAABB(gameObject &obj, D3DXVECTOR3 &collisionVector);
	float getRadius(){ return sphe.r; }
	void setRadius(float r){ sphe.r = r; }

};

#endif