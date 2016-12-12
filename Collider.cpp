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
//現在ここで衝突後のふるまいを決めている様子
//collisionVectorは自分の速度を表しており衝突判定前は計算用に
//最後は反発後の速度s'1を入れておく
//場合分けに対応するため速度の保管用などの用途とは別に跳ね返るべき方向成分Sinkingを用意(球とAABBでは跳ね返す方向のルールが違うため)
void Collider::bounce(gameObject &obj, D3DXVECTOR3 &collisionVector, D3DXVECTOR3 Sinking, D3DXVECTOR3 ref)
{
	float e = 1.0f;
	D3DXVECTOR3 Vdiff = collisionVector - obj.getVelocity(); //相手側の速度ベクトルから自分側の速度ベクトルを引くことで反射されるべく方向を出す。
	D3DXVECTOR3 cUV; //衝突単位ベクトル(collisionVectorは各collide関数から常に帰ってきている)
	D3DXVec3Normalize(&cUV, &ref); //ぶつかった方向の単位ベクトル
	float cUVdotVdiff = D3DXVec3Dot(&Vdiff,&cUV);
	float massRatio = 1.0f;
	if (_this->getMass() != 0)
		massRatio *= (obj.getMass() / (_this->getMass() + obj.getMass()));

	//エンティティがすでに離れる方向に動いている場合は、bounceが
	//前に呼び出されたものの、衝突がまだ続いている状態です。
	//エンティティを、collisionVectorに沿って離れる方向に移動
	if (cUVdotVdiff >  0) //エンティティ同士が離れる方向に動いている
	{
		//Sinkingを使って埋め込まれている反対方向分ずらす。Sinkingは離れる方向のベクトルにする
		if (!Freeze)
			_this->setPosition(Coltransform.getPosition()-Sinking);

	//	_this->Freedata_1 = Sinking.x;
	//	_this->Freedata_2 = Sinking.y;
	//	_this->Freedata_3 = Sinking.z;
	}
	else{
	//	D3DXVECTOR3 dv = _this->getDeltaV();
	//	dv += ((massRatio*(1.0f + e) * cUVdotVdiff) * cUV); //速度の変化量
	//	_this->setDeltaV(dv);
		D3DXVECTOR3 V_dash = -((massRatio*(1.0f + e) * cUVdotVdiff) * cUV);
		_this->setDeltaV(V_dash);//次回のupdate時に速度に足す。今更新するとcolide処理の他の物体の計算に影響を及ぼす
		
	}


}

void Collider::bounceOn(gameObject &obj, D3DXVECTOR3 &collisionVector){


	bounceFlag = true;
	bounceObj = &obj;
	cVector = obj.getPosition() - Coltransform.getPosition();
	collisionVector = cVector;
}