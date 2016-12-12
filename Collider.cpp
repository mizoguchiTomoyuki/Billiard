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
//���݂����ŏՓˌ�̂ӂ�܂������߂Ă���l�q
//collisionVector�͎����̑��x��\���Ă���Փ˔���O�͌v�Z�p��
//�Ō�͔�����̑��xs'1�����Ă���
//�ꍇ�����ɑΉ����邽�ߑ��x�̕ۊǗp�Ȃǂ̗p�r�Ƃ͕ʂɒ��˕Ԃ�ׂ���������Sinking��p��(����AABB�ł͒��˕Ԃ������̃��[�����Ⴄ����)
void Collider::bounce(gameObject &obj, D3DXVECTOR3 &collisionVector, D3DXVECTOR3 Sinking, D3DXVECTOR3 ref)
{
	float e = 1.0f;
	D3DXVECTOR3 Vdiff = collisionVector - obj.getVelocity(); //���葤�̑��x�x�N�g�����玩�����̑��x�x�N�g�����������ƂŔ��˂����ׂ��������o���B
	D3DXVECTOR3 cUV; //�Փ˒P�ʃx�N�g��(collisionVector�͊ecollide�֐������ɋA���Ă��Ă���)
	D3DXVec3Normalize(&cUV, &ref); //�Ԃ����������̒P�ʃx�N�g��
	float cUVdotVdiff = D3DXVec3Dot(&Vdiff,&cUV);
	float massRatio = 1.0f;
	if (_this->getMass() != 0)
		massRatio *= (obj.getMass() / (_this->getMass() + obj.getMass()));

	//�G���e�B�e�B�����łɗ��������ɓ����Ă���ꍇ�́Abounce��
	//�O�ɌĂяo���ꂽ���̂́A�Փ˂��܂������Ă����Ԃł��B
	//�G���e�B�e�B���AcollisionVector�ɉ����ė��������Ɉړ�
	if (cUVdotVdiff >  0) //�G���e�B�e�B���m�����������ɓ����Ă���
	{
		//Sinking���g���Ė��ߍ��܂�Ă��锽�Ε��������炷�BSinking�͗��������̃x�N�g���ɂ���
		if (!Freeze)
			_this->setPosition(Coltransform.getPosition()-Sinking);

	//	_this->Freedata_1 = Sinking.x;
	//	_this->Freedata_2 = Sinking.y;
	//	_this->Freedata_3 = Sinking.z;
	}
	else{
	//	D3DXVECTOR3 dv = _this->getDeltaV();
	//	dv += ((massRatio*(1.0f + e) * cUVdotVdiff) * cUV); //���x�̕ω���
	//	_this->setDeltaV(dv);
		D3DXVECTOR3 V_dash = -((massRatio*(1.0f + e) * cUVdotVdiff) * cUV);
		_this->setDeltaV(V_dash);//�����update���ɑ��x�ɑ����B���X�V�����colide�����̑��̕��̂̌v�Z�ɉe�����y�ڂ�
		
	}


}

void Collider::bounceOn(gameObject &obj, D3DXVECTOR3 &collisionVector){


	bounceFlag = true;
	bounceObj = &obj;
	cVector = obj.getPosition() - Coltransform.getPosition();
	collisionVector = cVector;
}