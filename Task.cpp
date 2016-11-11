#include "Task.h"

Task::Task(){
	enable = true;
}

Task::~Task(){

}

void Task::start(gameObject* t){
	_this = t;

}
