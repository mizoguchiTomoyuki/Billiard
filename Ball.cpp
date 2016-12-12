#include "Ball.h"
Ball::Ball() : gameObject(){
	_mesh.start(this);
	_mesh.initialize(BALL_MESH,NINEBALL_TEXTURE, graphicsNS::BLUE, { 0.2f, 0.2f, 0.2f, 1.0f });
	transform.position = { 10.0f, 10.0f, 10.0f };
	SetObjClassName("Ball");
	t = 0.0f;
	col.start(this);
	col.setRadius(1.0f);
	collider = &col;
	col.SetFreeze(false);
	isCollide = true;
	velocity.x = -3.00f;
	MyPrefab = PREFAB_MENU::BALL;
}

Ball::~Ball(){
}

void Ball::update(){
	
	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	velocity += deltaV;
	deltaV.x = 0;
	deltaV.y = 0;
	deltaV.z = 0;
	transform.position.x += velocity.x*frameTime;
	transform.position.y += velocity.y*frameTime;
	transform.position.z += velocity.z*frameTime;
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

