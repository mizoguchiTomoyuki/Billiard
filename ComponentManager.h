#ifndef _COMPONENT_MANAGER_H
#define _COMPONENT_MANAGER_H
#include "task.h"
class gameObject;
//gameObject�ɂ�������N���X���i�[���Ă���
//������update�ł������Ă�Task�N���X���ׂĂ�update���s��
//�����ł����炢����
class ComponentManager{
private:
	gameObject* gameObj; //���ݎQ��
	Task* tasks;
public:
	ComponentManager(gameObject* Obj);
	~ComponentManager();
	virtual void start();
	virtual void update();

};


#endif 