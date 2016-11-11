#ifndef _COMPONENT_MANAGER_H
#define _COMPONENT_MANAGER_H
#include "task.h"
class gameObject;
//gameObjectにくっつけるクラスを格納しておく
//こいつのupdateでくっついてるTaskクラスすべてのupdateを行う
//実装できたらいいな
class ComponentManager{
private:
	gameObject* gameObj; //相互参照
	Task* tasks;
public:
	ComponentManager(gameObject* Obj);
	~ComponentManager();
	virtual void start();
	virtual void update();

};


#endif 