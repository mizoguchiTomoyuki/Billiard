#include "Renderer.h"
Renderer::Renderer(){
	enable = true;
	initialized = false;

}

Renderer::~Renderer(){

}
//start�֐�
//�Ăяo�����ň�񂾂��Ăяo��
void Renderer::start(gameObject* t){
	Task::start(t);
	if (&GameSceneManager::Instance() == nullptr)
		return;
	graphics = GameSceneManager::Instance().GetGameptr()->getGraphics();
	transform = t->getTransform();
}
void Renderer::update(){


}

void Renderer::draw(){

}