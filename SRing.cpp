#include "SRing.h"
#include "TextureStorage.h"
SRing::SRing() : SpriteObject(){
	sprite.setVisible(true);

}

SRing::~SRing(){


}
bool SRing::initialize(){
	if (!SpriteObject::initialize(SRingNS::WIDTH, SRingNS::HEIGHT, 1, TextureStorage::Instance().GetTexture(RING_IMAGE))){

		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing PlayerAnim"));
		return false;
	}
	return true;

}
void SRing::update(){
	sprite.setX(transform.position.x);
	sprite.setY(transform.position.y);
	sprite.setScale(transform.scale);
	sprite.setRadians(transform.angle);
	sprite.setVisible(visible);
	sprite.update();
}

void SRing::draw(){
	sprite.draw();
}
void SRing::ai(){

}
void SRing::collision(){

}