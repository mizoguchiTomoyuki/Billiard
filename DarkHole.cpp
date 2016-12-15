#include "DarkHole.h"
DarkHole::DarkHole() : gameObject(){
	_mesh.start(this);
	_mesh.initialize(BALL_MESH, NINEBALL_TEXTURE, graphicsNS::BLUE, { 0.2f, 0.2f, 0.2f, 1.0f });

	transform.position = { 10.0f, 10.0f, 10.0f };
	float radius = 0.65f;
	transform.scale = { radius, radius, radius };
	SetObjClassName("DarkHole");
	t = 0.0f;
	col.start(this);
	col.setRadius(radius);
	collider = &col;
	col.SetFreeze(false);
	col.setTag(ColliderNS::COL_TAG::DARKHOLE);
	col.setTrigger(true);
	isCollide = true;
	MyPrefab = PREFAB_MENU::DARKHOLE;
	isMove = false;
	MoveLength = 0.0f;
}
DarkHole::~DarkHole(){
}

void DarkHole::update(){

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	velocity += deltaV;
	deltaV.x = 0;
	deltaV.y = 0;
	deltaV.z = 0;
	transform.position += velocity*frameTime;
	_mesh.update();
	col.update();
	recordupdate();
}

void DarkHole::draw(){
	if (GameSceneManager::Instance().GetGameptr()->getpause()){
		_mesh.update();
	}
	//_mesh.draw();
	col.draw();
}

void DarkHole::ai(){

}
void DarkHole::collision(){
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
