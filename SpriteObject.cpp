#include "SpriteObject.h"
#include "SpriteObjectFactory.h"
#include "GameSceneManager.h"
SpriteObject::SpriteObject(){
	transform.position = { 0, 0 };
	transform.scale = 1.0f;
	transform.angle = 0;
	transform.parent = NULL;
	_id = SpriteObjectFactory::Instance().CreateElem(*this);
	destroy = false;
}

SpriteObject::~SpriteObject(){


}

bool SpriteObject::initialize( int width, int height, int ncols, TextureManager *textureM){
	Graphics* gra = GameSceneManager::Instance().GetGameptr()->getGraphics();
	return sprite.initialize(gra, width, height, ncols, textureM);
}