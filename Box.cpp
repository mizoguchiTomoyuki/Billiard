#include "Box.h"
Box::Box() : gameObject(){
	_mesh.start(this);
	_mesh.initialize(PRIMITIVE_CUBE,"", graphicsNS::BLUE, { 0.2f, 0.2f, 0.2f, 1.0f });
	transform.position = { 10.0f, 10.0f, 10.0f };
	transform.scale = {10.0f,10.0f,1.0f};
	SetObjClassName("Box");
	t = 0.0f;
	col.start(this);
	col.setHL({5.0f,5.0f,1.0f});
	col.SetFreeze(true);
	collider = &col;
	isCollide = true;
	MyPrefab = PREFAB_MENU::BOX; 
	Mass = 10;
}

Box::~Box(){
}

void Box::update(){

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	velocity += deltaV;
	deltaV.x = 0;
	deltaV.y = 0;
	deltaV.z = 0;
	//transform.position.x += velocity.x*frameTime;
	//transform.position.y += velocity.y*frameTime;
	//transform.position.z += velocity.z*frameTime;
	_mesh.update();
	col.update();
	recordupdate();
}

void Box::draw(){
	if (GameSceneManager::Instance().GetGameptr()->getpause()){
		_mesh.update();
	}
	_mesh.draw();
	col.draw();
}

void Box::ai(){

}
void Box::collision(){
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

