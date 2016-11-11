#ifndef _TASK_H
#define _TASK_H
class gameObject;
class Task{
protected:
	bool enable;
	gameObject* _this;
public:
	Task();
	~Task();
	virtual void start(gameObject* t);
	virtual void update(){};
	virtual void draw(){}; //���m�ɂ���Ă͎g��
	void setActive(bool active){ enable = active; }
	bool getActive(){ return enable; }

};

#include "gameObject.h"
#endif