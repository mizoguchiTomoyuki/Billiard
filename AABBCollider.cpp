#include "AABBCollider.h"
#include "SphereCollider.h"
AABBCollider::AABBCollider() : Collider(){
	_type = ColliderNS::AABB;
	aabb.hl = Float3(0.5f, 0.5f, 0.5f);
}


AABBCollider::~AABBCollider(){

}

void AABBCollider::start(gameObject* t){
	Collider::start(t);
	mesh.start(_this);
	mesh.initialize(PRIMITIVE_CUBE, NINEBALL_TEXTURE, graphicsNS::BLUE, { 0.2f, 0.2f, 0.2f, 1.0f });
	mesh.onWireFrame();
	mesh.SetTransform(&Coltransform);
}

void AABBCollider::update(){
	Collider::update();
	aabb.p = Coltransform.position;
	D3DXVECTOR3 scale = _this->getScale() / 2.0f;
	aabb.hl = scale;
	Coltransform.scale = { aabb.hl.x*2.05f, aabb.hl.y*2.05f, aabb.hl.z*2.05f };
}

void AABBCollider::draw(){
	if (GameSceneManager::Instance().GetGameptr()->getpause()){
		isVisible = true;
		mesh.update();
	}
	else{
		isVisible = false;
	}
	if (isVisible){
		mesh.draw();
	}

}

//���`�̏Փ˔���

bool AABBCollider::collide(gameObject &obj, D3DXVECTOR3 &collisionVector){
	if (obj.getCollider() == nullptr) //GameObject��Collider���Ȃ��ꍇ
		return false;
	bool returnValue = false;
	if (obj.getCollider()->getType() == ColliderNS::SPHERE){
		returnValue = ToSphere(obj, collisionVector);
	}

	if (obj.getCollider()->getType() == ColliderNS::AABB){
		returnValue = ToAABB(obj, collisionVector);
	}

	isCollide = returnValue;
	return returnValue;

}

bool AABBCollider::ToSphere(gameObject &obj, D3DXVECTOR3 &collisionVector){
	float Sqlen = 0;
	SphereCollider* spb = (SphereCollider*)obj.getCollider();
	D3DXVECTOR3 pos = obj.getPosition();
	Float3 p;
	p = pos;
	for (int i = 0; i < 3; i++){//�W�I�ƂȂ�_�����g�̍ő�ŏ����傫����(�͂ݏo���Ă��邩)���e���Œ��ׂ�
		if (p.Get(i) < aabb.GetMin(i)) 
			Sqlen += (p.Get(i) - aabb.GetMin(i))*(p.Get(i) - aabb.GetMin(i));

		if (p.Get(i) > aabb.GetMax(i))
			Sqlen += (p.Get(i) - aabb.GetMax(i))*(p.Get(i) - aabb.GetMax(i));
	}
	float MinLength = sqrt(Sqlen);//���镨�̂̒��S�_��AABB�̊e�ӂɂ�����ŏ��l��������
	if (MinLength <= spb->getRadius()){
	//	bounceOn(obj, collisionVector);
		//collisionVector�͏Փˎ��̐ڕ��ʂ�\�����߂ɗp����

		D3DXVECTOR3 cV = obj.getPosition() - Coltransform.getPosition();
		Point cvP;
		cvP = cV;
		float Vecx = cvP.dot(Float3(1.0f,0, 0));
		float Vecy = cvP.dot(Float3(0, 1.0f, 0));
		float Vecz = cvP.dot(Float3(0, 0, 1.0f));
		float x = ((Vecx > aabb.hl.x) + (Vecx < -aabb.hl.x))*Vecx;
		float y = ((Vecy > aabb.hl.y) + (Vecy < -aabb.hl.y))*Vecy;
		float z = ((Vecz > aabb.hl.z) + (Vecz < -aabb.hl.z))*Vecz;
		D3DXVECTOR3 V = { x, y, z };
		//D3DXVECTOR3 V_e = { (abs(x)>0)*SIGN(x)*1.0f, (abs(y)>0)*SIGN(y)*1.0f, (abs(z)>0)*SIGN(z)*1.0f }; //�l�����镔����1��
		D3DXVECTOR3 V_e = { (abs(x)>0)*(-1.0f), (abs(y)>0)*(-1.0f), (abs(z)>0)*(-1.0f) }; //���˕����̒l�𔽓]������
		D3DXVECTOR3 Sinking = { (abs(x)>0)*(MinLength - spb->getRadius()),
			(abs(y)>0)*(MinLength - spb->getRadius()),
			(abs(z)>0)*(MinLength - spb->getRadius()), };
		//collisionVector = { collisionVector.x*V_e.x, collisionVector.y*V_e.y, collisionVector.z*V_e.z };
		D3DXVECTOR3 ref;
		D3DXVec3Normalize(&ref, &Sinking);
		bounce(obj,collisionVector, Sinking,ref);
		return true;
	}
	return false;
}

bool AABBCollider::ToAABB(gameObject &obj, D3DXVECTOR3 &collisionVector){
	return false;
}