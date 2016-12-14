#include "BilliardTable.h"
BilliardTable::BilliardTable() : gameObject(){
	_tablemat.start(this);
	_mesh.start(this);
	_tablemat.initialize(BILLTABLE_CUBE, NINEBALL_TEXTURE, graphicsNS::BLUE, { 0.8f, 0.8f, 0.8f, 1.0f });
	_mesh.initialize(BILLMAT_CUBE, NINEBALL_TEXTURE, graphicsNS::BLUE, { 0.8f, 0.8f, 0.8f, 1.0f });
	transform.position = { 10.0f, 10.0f, 10.0f };
	transform.scale = { 10.0f, 10.0f, 10.0f };
	SetObjClassName("BilliardTable");
	MyPrefab = PREFAB_MENU::BILLIARDTABLE;
	Mass = 100000;
	deltaV = { 0, 0, 0 };
	velocity = { 0, 0, 0 };
	_mesh.setActive(true);
	_tablemat.setActive(true);
}

BilliardTable::~BilliardTable(){
}

void BilliardTable::update(){

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	velocity += deltaV;
	transform.position += velocity*frameTime;
	deltaV.x = 0;
	deltaV.y = 0;
	deltaV.z = 0;
	_mesh.update();
	_tablemat.Setoffset({ 0, 0.7f, 0 });
	_tablemat.update();
	recordupdate();
}

void BilliardTable::draw(){

	if (GameSceneManager::Instance().GetGameptr()->getpause()){
		_mesh.update();
		_tablemat.update();
	}
	_mesh.draw();
	_tablemat.draw();
}

void BilliardTable::ai(){

}
void BilliardTable::collision(){
}

