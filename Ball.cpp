#include "Ball.h"
Ball::Ball() : gameObject(){
	_mesh.start(this);
	_mesh.initialize(BALL_MESH,NINEBALL_TEXTURE, graphicsNS::BLUE, { 0.2f, 0.2f, 0.2f, 1.0f });
	transform.position = { 10.0f, 10.0f, 10.0f };
	float radius = 0.65f;
	transform.scale = { radius, radius, radius };
	SetObjClassName("Ball");
	t = 0.0f;
	col.start(this);
	col.setRadius(radius);
	collider = &col;
	col.SetFreeze(false);
	isCollide = true;
	MyPrefab = PREFAB_MENU::BALL;
	isMove = false;
	MoveLength =0.0f;
}
Ball::Ball(const char* texName) : gameObject(){
	_mesh.start(this);
	_mesh.initialize(texName, NINEBALL_TEXTURE, graphicsNS::BLUE, { 0.2f, 0.2f, 0.2f, 1.0f });
	transform.position = { 10.0f, 10.0f, 10.0f };
	float radius = 0.65f;
	transform.scale = { radius, radius, radius };
	SetObjClassName("Ball");
	t = 0.0f;
	col.start(this);
	col.setRadius(radius);
	collider = &col;
	col.SetFreeze(false);
	isCollide = true;
	MyPrefab = PREFAB_MENU::BALL;
	isMove = false;
	MoveLength = 0.0f;
}

Ball::~Ball(){
}

void Ball::update(){
	
	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	velocity += deltaV;
	attenuation();
	Rolling();
	deltaV.x = 0;
	deltaV.y = 0;
	deltaV.z = 0;
	transform.position += velocity*frameTime;
	_mesh.update();
	col.update();
	recordupdate();
}

void Ball::draw(){
	if (GameSceneManager::Instance().GetGameptr()->getpause()){
		_mesh.update();
	}
	_mesh.draw();
	col.draw();
}

void Ball::ai(){

}
void Ball::collision(){
	gameObjectFactory::gameObjectLink* obj = gameObjectFactory::Instance().GetFirstLink();
	int mSize = gameObjectFactory::Instance().Getsize();
	for (int i = 0; i < mSize; i++){
		if (obj->pointer->getisCollide()){
			if (obj->pointer->getELEMID() != getELEMID())
				getCollider()->collide(*obj->pointer, velocity);
		}
		if (obj->next != nullptr)
			obj = obj->next;
		else
			i = mSize + 1;

	}
}

void Ball::attenuation(){
	//�����C�W��
	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	float u = 0.3f;
	float F = -9.8f*Mass*u*0.1f;//�]���薀�C�Ȃ̂ł܂����1/10��
	D3DXVECTOR3 nV = {0,0,0};
	D3DXVec3Normalize(&nV, &velocity);//�����̃x�N�g�����Ƃ�
	//���C�����̑��x
	D3DXVECTOR3 dv = deltaV;
	D3DXVECTOR3 attenu = nV*F*frameTime; //bounce���݂�deltaV�Ɍ������𑫂�

	D3DXVECTOR3 sumV = velocity + attenu;
	if (D3DXVec3Dot(&velocity, &sumV)<=0){//���Ƃ��ƌ����������ƌv�Z���ʂ̌��������������Ε����Ȃ��
		deltaV = { 0, 0, 0 };
		velocity = { 0, 0, 0 };
		isMove = false;

	}
	else{
		isMove = true;

		velocity += attenu;

	}

}
//��������]����������
void Ball::Rolling(){
	if (!isMove) //�������Ă���Ԃ����X�V���s��
		return;
	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	D3DXVECTOR3 nV;
	D3DXVec3Normalize(&nV, &velocity);
	D3DXVECTOR3 up = { 0, -1, 0 }; //�������y���Ƃ��Ē�`���Ă���
	D3DXVECTOR3 Axis;
	D3DXVec3Cross(&Axis, &up, &nV);
	transform.Axis = Axis;
	MoveLength += D3DXVec3Length(&velocity)*frameTime;
	if (MoveLength > 10.0f){

		MoveLength = 0.0f;
	}
	float pow = MoveLength;
	transform.Axis_angle = MoveLength*2*PI/10.0f;


}

