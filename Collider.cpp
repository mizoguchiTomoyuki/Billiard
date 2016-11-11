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
	pos = Coltransform.getPosition(); //ポジションの更新

}
void Collider::draw(){

}
//Colliderの共通機能
void Collider::bounce(gameObject &obj,D3DXVECTOR3 &collisionVector)
{
	float e = 1.0f;
	D3DXVECTOR3 Vdiff = obj.getVelocity() - _this->getVelocity(); //相手側の速度ベクトルから自分側の速度ベクトルを引くことで反射されるべく方向を出す。
	D3DXVECTOR3 cUV; //衝突単位ベクトル(collisionVectorは各collide関数から常に帰ってきている)
	D3DXVec3Normalize(&cUV, &collisionVector); //ぶつかった方向の単位ベクトル
	float cUVdotVdiff = D3DXVec3Dot(&Vdiff,&cUV);
	float massRatio = 1.0f;
	if (_this->getMass() != 0)
		massRatio *= (obj.getMass() / (_this->getMass() + obj.getMass()));

	//エンティティがすでに離れる方向に動いている場合は、bounceが
	//前に呼び出されたものの、衝突がまだ続いている状態です。
	//エンティティを、collisionVectorに沿って離れる方向に移動
	if (cUVdotVdiff > 0) //エンティティ同士が離れる方向に動いている
	{
		float X = (Coltransform.getPosition().x - collisionVector.x); //cUVは衝突エネルギーの向きなのでここで物体同士が離れる方向に移動させられる
		float Y = (Coltransform.getPosition().y - collisionVector.y);
		float Z = (Coltransform.getPosition().z - collisionVector.z);
		if (!Freeze)
			_this->setPosition({ X, Y, Z });
	}
	else{
		D3DXVECTOR3 dv = _this->getDeltaV();
		dv += ((massRatio*(1.0f + e) * cUVdotVdiff) * cUV); //速度の変化量
		_this->setDeltaV(dv);
	}


}

void Collider::bounceOn(gameObject &obj, D3DXVECTOR3 &collisionVector){


	bounceFlag = true;
	bounceObj = &obj;
	cVector = obj.getPosition() - Coltransform.getPosition();
	collisionVector = cVector;
}