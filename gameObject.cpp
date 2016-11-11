#include "gameObject.h"
#include "ComponentManager.h"
#include "gameObjectFactory.h"
gameObject::gameObject(){
	transform.position = { 0.0f, 0.0f, 0.0f };
	transform.scale = { 1.0f, 1.0f, 1.0f };
	transform.rotate = { 0.0f, 0.0f, 0.0f };
	velocity = { 0.0f, 0.0f, 0.0f };
	destroy = false;
	_id = -1;
	Component = new ComponentManager(this);
	gameObjectFactory::Instance().CreateElem(*this);
	collider = nullptr;
	Mass = 10.0f;
	MyPrefab = PREFAB_MENU::EXIT;
	table_id = -1;
	isCollide = false;
}

void gameObject::recordupdate(){
	char c[2048];
	const char* n_c = name.c_str();
	sprintf_s(c,2048, "%d,%d,%s,%f,%f,%f,%f,%f,%f",
		table_id,
		(int)MyPrefab,
		n_c,
		transform.position.x,
		transform.position.y,
		transform.position.z,
		transform.scale.x,
		transform.scale.y,
		transform.scale.z);
		record = c;
}

gameObject::~gameObject(){
	delete Component;
}

std::vector<IndexString> gameObject::ConsoleInformation(){
	std::vector<IndexString> value;

	IndexString str;

	str.i = gameObjectNS::TYPEINDEX::INDEX;
	str.s = "Class :"+ClassName;
	str.indexNumber = 0;
	value.push_back(str);

	str.i = gameObjectNS::TYPEINDEX::INDEX;
	str.s = "name :" + GetName();
	str.indexNumber = 1;
	value.push_back(str);

	str.i = gameObjectNS::TYPEINDEX::INDEX;
	str.s = "Transform";
	str.indexNumber = 2;
	value.push_back(str);

	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "PosX :" + std::to_string(transform.position.x);
	str.indexNumber = 3;
	value.push_back(str);

	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "PosY :" + std::to_string(transform.position.y);
	str.indexNumber = 4;
	value.push_back(str);

	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "PosZ :" + std::to_string(transform.position.z);
	str.indexNumber = 5;
	value.push_back(str);


	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "VelX :" + std::to_string(velocity.x);
	str.indexNumber = 6;
	value.push_back(str);


	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "VelY :" + std::to_string(velocity.y);
	str.indexNumber = 7;
	value.push_back(str);


	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "VelZ :" + std::to_string(velocity.z);
	str.indexNumber = 8;
	value.push_back(str);
	
	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "ScaX :" + std::to_string(transform.scale.x);
	str.indexNumber = 9;
	value.push_back(str);

	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "ScaY :" + std::to_string(transform.scale.y);
	str.indexNumber = 10;
	value.push_back(str);

	str.i = gameObjectNS::TYPEINDEX::FLOAT;
	str.s = "ScaZ :" + std::to_string(transform.scale.z);
	str.indexNumber = 11;
	value.push_back(str);
	

	return value;

}

void gameObject::setfloat_IndexString(int index, float num){
	switch (index){
	case 3:
		transform.position.x = num;
		break;
	case 4:
		transform.position.y = num;
		break;
	case 5:
		transform.position.z = num;
		break;
	case 6:
		velocity.x = num;
		break;
	case 7:
		velocity.y = num;
		break;
	case 8:
		velocity.z = num;
		break;
	case 9:
		transform.scale.x = num;
		break;
	case 10:
		transform.scale.y = num;
		break;
	case 11:
		transform.scale.z = num;
	default:
		break;

	}
}


float gameObject::getfloat_IndexString(int index){
	float retNum = 0.0f;
	switch (index){
	case 3:
		retNum = transform.position.x;
		break;
	case 4:
		retNum = transform.position.y;
		break;
	case 5:
		retNum = transform.position.z;
		break;
	case 6:
		retNum = velocity.x;
		break;
	case 7:
		retNum = velocity.y;
		break;
	case 8:
		retNum = velocity.z;
		break;
	default:
		break;

	}
	return retNum;
}

