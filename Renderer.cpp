#include "Renderer.h"
Renderer::Renderer(){
	enable = true;
	initialized = false;

}

Renderer::~Renderer(){

}
//startŠÖ”
//ŒÄ‚Ño‚µ‘¤‚Åˆê‰ñ‚¾‚¯ŒÄ‚Ño‚·
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