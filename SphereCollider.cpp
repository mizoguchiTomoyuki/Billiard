#include "SphereCollider.h"
#include "AABBCollider.h"
SphereCollider::SphereCollider() : Collider(){
	_type = ColliderNS::SPHERE;
	sphe.r = 1.0f;
}


SphereCollider::~SphereCollider(){

}

void SphereCollider::start(gameObject* t){
	Collider::start(t);
	sphe.p = _this->getPosition();
	mesh.start(_this);
	mesh.initialize(PRIMITIVE_SPHERE, "", graphicsNS::BLUE, { 0.2f, 0.2f, 0.2f, 1.0f });
	mesh.onWireFrame();
	mesh.SetTransform(&Coltransform);
}

void SphereCollider::update(){
	Collider::update();
	sphe.p = Coltransform.position;
	Coltransform.scale = { sphe.r, sphe.r, sphe.r };
}

void SphereCollider::draw(){
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

bool SphereCollider::collide(gameObject &obj, D3DXVECTOR3 &collisionVector){
	if (obj.getCollider() == nullptr) //GameObject��Collider���Ȃ��ꍇ
		return false;
	bool returnValue = false;
	if (obj.getCollider()->getType() == ColliderNS::SPHERE){
		returnValue = ToSphere(obj, collisionVector);
	}

	if (obj.getCollider()->getType() == ColliderNS::AABB){
		returnValue = ToAABB(obj, collisionVector);
	}
	return returnValue;

}

bool SphereCollider::ToSphere(gameObject &obj, D3DXVECTOR3 &collisionVector){

	
	D3DXVECTOR3 pos_B = obj.getPosition(); //�Ƃ肠�����������AABB�̎����ɍ��킹�ĕύX
	SphereCollider* spb = (SphereCollider*)obj.getCollider();
	float radB = spb->getRadius();
	float xdist = pow(sphe.p.x - pos_B.x, 2);
	float ydist = pow(sphe.p.y - pos_B.y, 2);
	float zdist = pow(sphe.p.z - pos_B.z, 2);

	float Distance = xdist + ydist + zdist;
	float sumRadius = pow((sphe.r + radB),2);
	if (Distance <= sumRadius){
		//collisionVector = obj.getPosition() - _this->getPosition();
		//bounceOn(obj, collisionVector);
		D3DXVECTOR3 cV = obj.getPosition() - Coltransform.getPosition();
		D3DXVECTOR3 Sinking = { cV.x - getRadius(), 0.0f, cV.z - getRadius() };
		collisionVector = cV;
		bounce(obj, Sinking);
		return true;
	}
	return false;
}
//���x�Ɋ֌W�Ȃ��Փ�(�߂荞��)�𔻒�ł���
bool SphereCollider::ToAABB(gameObject &obj, D3DXVECTOR3 &collisionVector){


	float Sqlen = 0;
	AABBCollider* aabbC = (AABBCollider*)obj.getCollider();
	AABB aabb = aabbC->getAABB();
	D3DXVECTOR3 pos = Coltransform.getPosition();
	Float3 p;
	p = pos;
	for (int i = 0; i < 3; i++){//�W�I�ƂȂ�_�����g�̍ő�ŏ����傫����(�͂ݏo���Ă��邩)���e���Œ��ׂ�
		if (p.Get(i) < aabb.GetMin(i))
			Sqlen += (p.Get(i) - aabb.GetMin(i))*(p.Get(i) - aabb.GetMin(i));

		if (p.Get(i) > aabb.GetMax(i))
			Sqlen += (p.Get(i) - aabb.GetMax(i))*(p.Get(i) - aabb.GetMax(i));
	}
	float MinLength = sqrt(Sqlen);//���镨�̂̒��S�_��AABB�̊e�ӂɂ�����ŏ��l��������
	if (MinLength <= getRadius()){

	//	bounceOn(obj, collisionVector);
		D3DXVECTOR3 cV = (obj.getPosition() - Coltransform.getPosition());
		Point cvP;
		cvP = cV;
		float Vecx = cvP.dot(Float3(1.0f, 0, 0));
		float Vecy = cvP.dot(Float3(0, 1.0f, 0));
		float Vecz = cvP.dot(Float3(0, 0, 1.0f));
		float x = ((Vecx > aabb.hl.x) + (Vecx < -aabb.hl.x))*Vecx;
		float y = ((Vecy > aabb.hl.y) + (Vecy < -aabb.hl.y))*Vecy;
		float z = ((Vecz > aabb.hl.z) + (Vecz < -aabb.hl.z))*Vecz;
		D3DXVECTOR3 V = { x, y, z };
		//D3DXVECTOR3 V_e = { (abs(x)>0)*SIGN(x)*1.0f, (abs(y)>0)*SIGN(y)*1.0f, (abs(z)>0)*SIGN(z)*1.0f }; //�l�����镔����1��
		D3DXVECTOR3 V_e = { (abs(x)>0)*(-1.0f), (abs(y)>0)*(-1.0f), (abs(z)>0)*(-1.0f) }; //���˕����̒l�𔽓]������
		float Sink = MinLength - getRadius();
		D3DXVECTOR3 Sinking = { SIGN(x)*(V_e.x)*(Sink),
			SIGN(y)*(V_e.y)*(Sink),
			SIGN(z)*(V_e.z)*(Sink), };
		collisionVector = { collisionVector.x*V_e.x, collisionVector.y*V_e.y, collisionVector.z*V_e.z };
		bounce(obj, Sinking);
		return true;
	}
	return false;
}
