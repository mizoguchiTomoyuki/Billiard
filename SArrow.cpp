#include "SArrow.h"
#include "TextureStorage.h"
SArrow::SArrow() : SpriteObject(){
	sprite.setVisible(true);

}

SArrow::~SArrow(){


}
bool SArrow::initialize(){
	if (!SpriteObject::initialize(SArrowNS::WIDTH, SArrowNS::HEIGHT, 1, TextureStorage::Instance().GetTexture(ARROW_IMAGE))){

		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing PlayerAnim"));
		return false;
	}
	return true;

}
void SArrow::update(){
	sprite.setX(transform.position.x);
	sprite.setY(transform.position.y);
	sprite.setScale(transform.scale);
	sprite.setRadians(transform.angle);
	sprite.setVisible(visible);
	sprite.update();
}

void SArrow::draw(){
	sprite.draw();
}
void SArrow::ai(){

}
void SArrow::collision(){

}