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
//球形の衝突判定

bool SphereCollider::collide(gameObject &obj, D3DXVECTOR3 &collisionVector){
	if (obj.getCollider() == nullptr) //GameObjectのColliderがない場合
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


	D3DXVECTOR3 pos_B = obj.getPosition(); //とりあえず無理やりAABBの実装に合わせて変更



	SphereCollider* spb = (SphereCollider*)obj.getCollider();
	float radB = spb->getRadius();
	float xdist = pow(sphe.p.x - pos_B.x, 2);
	float ydist = pow(sphe.p.y - pos_B.y, 2);
	float zdist = pow(sphe.p.z - pos_B.z, 2);

	float Distance = xdist + ydist + zdist;
	float sumRadius = pow((sphe.r + radB),2);
	//衝突の判定
	if (Distance <= sumRadius){
		//collisionVector = obj.getPosition() - _this->getPosition();
		//bounceOn(obj, collisionVector);

		//まず球同士の接触面方向のベクトルを求める(方向は自分から相手へ)
		D3DXVECTOR3 cV = Coltransform.getPosition() - obj.getPosition();
		D3DXVECTOR3 cUV; //衝突平面方向の単位ベクトル(collisionVectorは各collide関数から常に帰ってきている)
		D3DXVec3Normalize(&cUV, &cV); //ぶつかった方向の単位ベクトル
		float Length = D3DXVec3Length(&cV);
		SphereCollider* obj_sphe = (SphereCollider*)obj.getCollider();
		float Sink = (Length - getRadius())-obj_sphe->getRadius(); //ぶつかってる者同士の半径と中心間距離から沈んだ距離を出す
		D3DXVECTOR3 Sinking = cUV*Sink;//cUVは相手の方に向かう方向
	//	D3DXVECTOR3 Sinking = radV - cV;
		if (Length == getRadius()){


		}
		//radV = cVのとき衝突中にも関わらずSinkingは0になる
		//collisionVector = cV;
		bounce(obj, collisionVector,Sinking,cUV);//そのまま速度を与えることにする
		return true;
	}
	return false;
}
//速度に関係なく衝突(めり込み)を判定できる
bool SphereCollider::ToAABB(gameObject &obj, D3DXVECTOR3 &collisionVector){


	float Sqlen = 0;
	AABBCollider* aabbC = (AABBCollider*)obj.getCollider();
	AABB aabb = aabbC->getAABB();
	D3DXVECTOR3 pos = Coltransform.getPosition();
	Float3 p;
	p = pos;
	for (int i = 0; i < 3; i++){//標的となる点が自身の最大最小より大きいか(はみ出しているか)を各軸で調べる
		if (p.Get(i) < aabb.GetMin(i))
			Sqlen += (p.Get(i) - aabb.GetMin(i))*(p.Get(i) - aabb.GetMin(i));

		if (p.Get(i) > aabb.GetMax(i))
			Sqlen += (p.Get(i) - aabb.GetMax(i))*(p.Get(i) - aabb.GetMax(i));
	}
	float MinLength = sqrt(Sqlen);//ある物体の中心点とAABBの各辺における最小値が得られる
	if (MinLength <= getRadius()){

	//	bounceOn(obj, collisionVector);
		D3DXVECTOR3 cV = (Coltransform.getPosition() - obj.getPosition());
		Point cvP;
		cvP = cV;
		//AABBの中心と自身の距離をとっている
		float Vecx = cvP.dot(Float3(1.0f, 0, 0));//物体間の距離のxの+方向の射影(成分の大きさ)をとっている
		float Vecy = cvP.dot(Float3(0, 1.0f, 0));
		float Vecz = cvP.dot(Float3(0, 0, 1.0f));
		float x = ((Vecx > aabb.hl.x) + (Vecx < -aabb.hl.x))*Vecx; //AABBよりどちらかの外側にいる場合のみ0にならない
		float y = ((Vecy > aabb.hl.y) + (Vecy < -aabb.hl.y))*Vecy; //また外側にいる場合はAABBの面への方向を返す
		float z = ((Vecz > aabb.hl.z) + (Vecz < -aabb.hl.z))*Vecz;

		//Sink処理 めり込んだ力の中でもっとも外に出るまでに労力のかからない(距離が短い方向を採用する)
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
		//最小のめり込み距離をベクトルで返す

	
		D3DXVECTOR3 V = { x, y, z };//AABBの面にむかうベクトル
		if (D3DXVec3Length(&V) == 0){
			V = s_vec;
		}
		//D3DXVECTOR3 V_e = { (abs(x)>0)*SIGN(x)*1.0f, (abs(y)>0)*SIGN(y)*1.0f, (abs(z)>0)*SIGN(z)*1.0f }; //値がある部分を1で
		D3DXVECTOR3 V_e = { (abs(x)>0)*(-1.0f), (abs(y)>0)*(-1.0f), (abs(z)>0)*(-1.0f) }; //反射部分の値を反転させる
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
