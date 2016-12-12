#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <algorithm>
#include "constants.h"
#include "Table.h"
namespace gameObjectNS{

	enum TYPEINDEX{
		INDEX = 0,
		STRING = 1,
		FLOAT = 2,
		INT = 3
	};
}
struct Transform{
	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotate;
	D3DXVECTOR3 Axis;
	float Axis_angle;
	Transform* parent;
	D3DXVECTOR3 getPosition(){ return position; }
	D3DXVECTOR3 getScale(){ return scale; }
	D3DXVECTOR3 getRotation(){ return rotate; }
	
};
//文字情報に付加情報を付けて送る
struct IndexString{
	gameObjectNS::TYPEINDEX i; //この文章の情報
	std::string s;
	int indexNumber;
};
class ComponentManager;
class Task;
class Collider;
class gameObject{
protected:
	//Transform的な
	Transform transform;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 deltaV; //accell
	ComponentManager* Component; //相互参照先
	bool destroy;
	int _id;
	std::string name; //Object自身の名前
	std::string ClassName; //クラス名
	Collider* collider;
	bool isCollide;
	float Mass; //重量
	std::string record; //情報の保管先用の文字データ
	PREFAB_MENU MyPrefab;
	int table_id;
public:
	gameObject();
	virtual ~gameObject();
	virtual void update(){};
	virtual void draw(){};
	virtual void ai(){};
	virtual void collision(){};
	virtual void recordupdate();
	//get set1関数群
	D3DXVECTOR3 getPosition(){ return transform.position; }
	D3DXVECTOR3 getScale(){ return transform.scale; }
	D3DXVECTOR3 getRotation(){ return transform.rotate; }
	Transform* getParent(){ return transform.parent; }
	Transform* getTransform(){ return &transform; }
	void setTransform(Transform t){ transform = t; }
	void setPosition(D3DXVECTOR3 position){ transform.position = position; }
	void setScale(D3DXVECTOR3 scale){ transform.scale = scale; }
	void setRotation(D3DXVECTOR3 rotation){ transform.rotate = rotation; }
	Task* GetComponent(char* name);
	virtual void setELEMID(ELEMID id){ _id = id; }
	virtual ELEMID getELEMID(){ return _id; }
	void SetName(std::string n){ name = n; }
	std::string GetName(){ return name; }
	virtual void SetObjClassName(std::string n){ClassName = n;}
	std::string GetObjClassName(){ return ClassName; }
	void Destroy(){ destroy = true; } //死亡予約
	bool isDestroy(){ return destroy; }
	virtual std::vector<IndexString> ConsoleInformation();
	virtual void setfloat_IndexString(int index, float num);
	virtual float getfloat_IndexString(int index);
	Collider* getCollider(){ return collider; };
	void setCollider(Collider* col){ collider = col; }
	D3DXVECTOR3 getVelocity(){ return velocity; }
	float getMass(){ return Mass; }
	D3DXVECTOR3 getDeltaV(){ return deltaV; }
	void setDeltaV(D3DXVECTOR3 dv){ deltaV = dv; }
	std::string getRecord(){ return record; }
	void setMyPrefab(PREFAB_MENU p){ MyPrefab = p; }
	PREFAB_MENU getMyPrefab(){ return MyPrefab; }
	void setTable_id(int i){ table_id = i; }
	int getTable_id(){ return table_id; }
	bool getisCollide(){ return isCollide; }
};
#include "task.h"
#include "Collider.h"
#endif