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

	isCollide = returnValue;
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
	//�Փ˂̔���
	if (Distance <= sumRadius){
		//collisionVector = obj.getPosition() - _this->getPosition();
		//bounceOn(obj, collisionVector);

		//�܂������m�̐ڐG�ʕ����̃x�N�g�������߂�(�����͎������瑊���)
		D3DXVECTOR3 cV = Coltransform.getPosition() - obj.getPosition();
		D3DXVECTOR3 cUV; //�Փ˕��ʕ����̒P�ʃx�N�g��(collisionVector�͊ecollide�֐������ɋA���Ă��Ă���)
		D3DXVec3Normalize(&cUV, &cV); //�Ԃ����������̒P�ʃx�N�g��
		float Length = D3DXVec3Length(&cV);
		SphereCollider* obj_sphe = (SphereCollider*)obj.getCollider();
		float Sink = (Length - getRadius())-obj_sphe->getRadius(); //�Ԃ����Ă�ғ��m�̔��a�ƒ��S�ԋ������璾�񂾋������o��
		D3DXVECTOR3 Sinking = cUV*Sink;//cUV�͑���̕��Ɍ���������
	//	D3DXVECTOR3 Sinking = radV - cV;
		if (Length == getRadius()){


		}
		//radV = cV�̂Ƃ��Փ˒��ɂ��ւ�炸Sinking��0�ɂȂ�
		//collisionVector = cV;
		bounce(obj, collisionVector,Sinking,cUV);//���̂܂ܑ��x��^���邱�Ƃɂ���
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
		D3DXVECTOR3 cV = (Coltransform.getPosition() - obj.getPosition());
		Point cvP;
		cvP = cV;
		//AABB�̒��S�Ǝ��g�̋������Ƃ��Ă���
		float Vecx = cvP.dot(Float3(1.0f, 0, 0));//���̊Ԃ̋�����x��+�����̎ˉe(�����̑傫��)���Ƃ��Ă���
		float Vecy = cvP.dot(Float3(0, 1.0f, 0));
		float Vecz = cvP.dot(Float3(0, 0, 1.0f));
		float x = ((Vecx > aabb.hl.x) + (Vecx < -aabb.hl.x))*Vecx; //AABB���ǂ��炩�̊O���ɂ���ꍇ�̂�0�ɂȂ�Ȃ�
		float y = ((Vecy > aabb.hl.y) + (Vecy < -aabb.hl.y))*Vecy; //�܂��O���ɂ���ꍇ��AABB�̖ʂւ̕�����Ԃ�
		float z = ((Vecz > aabb.hl.z) + (Vecz < -aabb.hl.z))*Vecz;

		//Sink���� �߂荞�񂾗͂̒��ł����Ƃ��O�ɏo��܂łɘJ�͂̂�����Ȃ�(�������Z���������̗p����)
		float Sink_[3] = { abs(abs(Vecx) - (getRadius() + aabb.hl.x)),
			abs(abs(Vecy) - (getRadius() + aabb.hl.y)),
			abs(abs(Vecz) - (getRadius() + aabb.hl.z)) };
		//0 :X 1:Y 2:Z
		D3DXVECTOR3 s_vec = { 0, 0, 0 };
		int min = 0;
		if (Sink_[0] > Sink_[1])
			min = 1;
		else
			min = 0;

		if (Sink_[2] < Sink_[min])
			min = 2;

		switch (min){
		case 0:
			s_vec = { SIGN(abs(Vecx) - (getRadius() + aabb.hl.x))*Sink_[0], 0, 0 };
			break;
		case 1:
			s_vec = { 0, SIGN(abs(Vecy) - (getRadius() + aabb.hl.y))*Sink_[1], 0 };
			break;
		case 2:
			s_vec = { 0, 0, SIGN(abs(Vecz) - (getRadius() + aabb.hl.z))*Sink_[2] };
			break;
		default:
			s_vec = { Sink_[0], Sink_[1], Sink_[2] };
			break;
		}
		//�ŏ��̂߂荞�݋������x�N�g���ŕԂ�

	
		D3DXVECTOR3 V = { x, y, z };//AABB�̖ʂɂނ����x�N�g��
		if (D3DXVec3Length(&V) == 0){
			V = s_vec;
		}
		//D3DXVECTOR3 V_e = { (abs(x)>0)*SIGN(x)*1.0f, (abs(y)>0)*SIGN(y)*1.0f, (abs(z)>0)*SIGN(z)*1.0f }; //�l�����镔����1��
		D3DXVECTOR3 V_e = { (abs(x)>0)*(-1.0f), (abs(y)>0)*(-1.0f), (abs(z)>0)*(-1.0f) }; //���˕����̒l�𔽓]������
		float Sink = MinLength - getRadius();
		D3DXVECTOR3 ref;
		D3DXVec3Normalize(&ref, &V);
		D3DXVECTOR3 Sinking = { (ref.x)*abs(Sink),
			(ref.y)*abs(Sink),
			(ref.z)*abs(Sink) };
		//collisionVector = { collisionVector.x*V_e.x, collisionVector.y*V_e.y, collisionVector.z*V_e.z };
		bounce(obj, collisionVector,-Sinking,ref);
		return true;
	}
	return false;
}
