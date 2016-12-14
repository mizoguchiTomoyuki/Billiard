#include "SShadow.h"
#include "TextureStorage.h"
SShadow::SShadow() : SpriteObject(){
	sprite.setVisible(true);

}

SShadow::~SShadow(){


}
bool SShadow::initialize(){
	if (!SpriteObject::initialize(SShadowNS::WIDTH, SShadowNS::HEIGHT, 1, TextureStorage::Instance().GetTexture(SHADOW_IMAGE))){

		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing PlayerAnim"));
		return false;
	}
	return true;

}
void SShadow::update(){
	sprite.setX(transform.position.x);
	sprite.setY(transform.position.y);
	sprite.setScale(transform.scale);
	sprite.setRadians(transform.angle);
	sprite.setVisible(visible);
	sprite.update();
}

void SShadow::draw(){
	sprite.draw();
}
void SShadow::ai(){

}
void SShadow::collision(){

}