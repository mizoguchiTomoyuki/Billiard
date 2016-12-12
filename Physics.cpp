#include"Physics.h"
#include "GameSceneManager.h"
Physics::Physics() : Task(){
	velocity = { 0 ,0,0};
	deltaV = { 0 ,0,0};

}

Physics::~Physics(){



}

void Physics::start(gameObject* t){
	Task::start(t);


}

void Physics::update(){
	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	D3DXVECTOR3 pos = _this->getPosition();
	velocity += deltaV;
	deltaV = { 0, 0, 0 };
	pos += velocity*frameTime;

}

void Physics::draw(){



}