#include "Collider.h"
Collider::Collider() : Task(){
	_type = ColliderNS::END;
	bounceFlag = false;
	cVector = { 0.0f, 0.0f, 0.0f };
	bounceObj = nullptr;
	Freeze = false;
}

Collider::~Collider(){

}

void Collider::start(gameObject* t){
	Task::start(t);

}

void Collider::update(){
	Coltransform = *_this->getTransform();
	pos = Coltransform.getPosition(); //�|�W�V�����̍X�V

}
void Collider::draw(){

}
//Collider�̋��ʋ@�\
void Collider::bounce(gameObject &obj,D3DXVECTOR3 &collisionVector)
{
	float e = 1.0f;
	D3DXVECTOR3 Vdiff = obj.getVelocity() - _this->getVelocity(); //���葤�̑��x�x�N�g�����玩�����̑��x�x�N�g�����������ƂŔ��˂����ׂ��������o���B
	D3DXVECTOR3 cUV; //�Փ˒P�ʃx�N�g��(collisionVector�͊ecollide�֐������ɋA���Ă��Ă���)
	D3DXVec3Normalize(&cUV, &collisionVector); //�Ԃ����������̒P�ʃx�N�g��
	float cUVdotVdiff = D3DXVec3Dot(&Vdiff,&cUV);
	float massRatio = 1.0f;
	if (_this->getMass() != 0)
		massRatio *= (obj.getMass() / (_this->getMass() + obj.getMass()));

	//�G���e�B�e�B�����łɗ��������ɓ����Ă���ꍇ�́Abounce��
	//�O�ɌĂяo���ꂽ���̂́A�Փ˂��܂������Ă����Ԃł��B
	//�G���e�B�e�B���AcollisionVector�ɉ����ė��������Ɉړ�
	if (cUVdotVdiff > 0) //�G���e�B�e�B���m�����������ɓ����Ă���
	{
		float X = (Coltransform.getPosition().x - collisionVector.x); //cUV�͏Փ˃G�l���M�[�̌����Ȃ̂ł����ŕ��̓��m�����������Ɉړ���������
		float Y = (Coltransform.getPosition().y - collisionVector.y);
		float Z = (Coltransform.getPosition().z - collisionVector.z);
		if (!Freeze)
			_this->setPosition({ X, Y, Z });
	}
	else{
		D3DXVECTOR3 dv = _this->getDeltaV();
		dv += ((massRatio*(1.0f + e) * cUVdotVdiff) * cUV); //���x�̕ω���
		_this->setDeltaV(dv);
	}


}

void Collider::bounceOn(gameObject &obj, D3DXVECTOR3 &collisionVector){


	bounceFlag = true;
	bounceObj = &obj;
	cVector = obj.getPosition() - Coltransform.getPosition();
	collisionVector = cVector;
}