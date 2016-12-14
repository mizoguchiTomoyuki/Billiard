#include "TextObject.h"
#include "SpriteObjectFactory.h"
#include "GameSceneManager.h"
//コンストラクタはSpriteObjectにおまかせ
TextObject::TextObject(){
}

TextObject::~TextObject(){


}

bool TextObject::initialize(){
	Graphics* gra = GameSceneManager::Instance().GetGameptr()->getGraphics();
	return _text.initialize(gra);
}