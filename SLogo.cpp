#include "SLogo.h"
#include "TextureStorage.h"
SLogo::SLogo() : SpriteObject(){
	sprite.setVisible(true);

}

SLogo::~SLogo(){


}
bool SLogo::initialize(){
	if (!SpriteObject::initialize(SLogoNS::WIDTH, SLogoNS::HEIGHT, 1, TextureStorage::Instance().GetTexture(LOGO_IMAGE))){

		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing PlayerAnim"));
		return false;
	}
	return true;

}
void SLogo::update(){
	sprite.setX(transform.position.x);
	sprite.setY(transform.position.y);
	sprite.setScale(transform.scale);
	sprite.setRadians(transform.angle);
	sprite.setVisible(visible);
	sprite.update();
}

void SLogo::draw(){
	sprite.draw();
}
void SLogo::ai(){

}
void SLogo::collision(){

}