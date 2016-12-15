#include "SINFO.h"
#include "TextureStorage.h"
#include "GameSceneManager.h"
#define LIMIT_SINFO_TIME 3.0f
SInfo::SInfo() : SpriteObject(){
	sprite.setVisible(false);
	Unset();

}

SInfo::~SInfo(){


}
bool SInfo::initialize(std::string str , int width, int height){
	if (!SpriteObject::initialize(width, height, 1, TextureStorage::Instance().GetTexture(str))){

		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing PlayerAnim"));
		return false;
	}
	return true;

}
void SInfo::update(){
	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	if (start){
		Counter += frameTime;
		if (Counter > LIMIT_SINFO_TIME){
			Unset();
		}
	}
	sprite.setX(transform.position.x);
	sprite.setY(transform.position.y);
	sprite.setScale(transform.scale);
	sprite.setRadians(transform.angle);
	sprite.setVisible(visible);
	sprite.update();
}

void SInfo::draw(){
	sprite.draw();
}
void SInfo::ai(){

}
void SInfo::collision(){

}
void SInfo::setStart(){
	isVisible(true);
	start = true;

}
void SInfo::Unset(){
	Counter = 0;
	isVisible(false);
	start = false;


}