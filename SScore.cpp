#include "SScore.h"
#include "TextureStorage.h"
#include "GameSceneManager.h"
SScore::SScore() : TextObject(){
	_text.setVisible(true);
	visible = true;

}

SScore::~SScore(){


}
bool SScore::initialize(){
	TextObject::initialize();
	return true;

}
void SScore::update(){
	transform.position = { 0, 0 };
	_text.setX(transform.position.x);
	_text.setY(transform.position.y);
	_text.setScale(transform.scale);
	_text.setRadians(transform.angle);
	_text.setVisible(visible);
	_text.update();
}

void SScore::draw(){
	_text.setFontColor(graphicsNS::WHITE);
	_text.setBackColor(TRANSCOLOR);
	_text.print(str,0,0);
}
void SScore::ai(){

}
void SScore::collision(){

}