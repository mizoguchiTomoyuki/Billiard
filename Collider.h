#ifndef _COLLIDER_H
#define _COLLIDER_H
#include "Task.h"
namespace ColliderNS{
	enum COL_TYPE
	{
		SPHERE = 0,
		BOX = 1,
		AABB =2, //�O������Ԃ̎��ɕ��s�ȃ{�b�N�X
		END,
	};

}
//�Փ˔�����s���N���X�̐e�N���X
//Task���p�����邪�A�ق���Task�ƈꏏ�����ɂ���collision����̒��ɓ����
class Collider : public Task{
protected:
	bool enable; //�Փ˔�����s�����ǂ����̔���
	D3DXVECTOR3 pos;
	ColliderNS::COL_TYPE _type; //�����COllider�̔���ɗp����
	bool isVisible;
	Transform Coltransform; //collider�̃g�����X�t�H�[����{�I�ɂ�gameObject�ɏ�������
	bool bounceFlag;
	D3DXVECTOR3 cVector; //Bounce���̕ێ��悤
	gameObject* bounceObj;
	bool Freeze;
public:
	Collider();
	~Collider();
	virtual void start(gameObject* t);
	virtual void update();
	virtual bool collide(gameObject &obj, D3DXVECTOR3 &collisionVector) = 0;//�������z�֐���������Ƃɍ��BoxCollider,SphereCollider�Ŏ���
	virtual void draw();
	void SetEnable(bool b){ enable = b; }
	bool getEnable(){ return enable; }
	ColliderNS::COL_TYPE getType(){ return _type; }
	void bounce(gameObject &obj, D3DXVECTOR3 &collisionVector, D3DXVECTOR3 Sinking, D3DXVECTOR3 ref);
	void bounceOn(gameObject &obj, D3DXVECTOR3 &collisionVector);
	void SetFreeze(bool b){ Freeze = b; }

};


#endif