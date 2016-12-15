

#include "ScorePoint.h"
#include "TextureStorage.h"
#include "GameSceneManager.h"
#define LIMITTIME 0.5f
ScorePoint::ScorePoint() : TextObject(){
	_text.setVisible(true);
	visible = true;
	Timer = 0;
	up = false;
}

ScorePoint::~ScorePoint(){


}
bool ScorePoint::initialize(){
	TextObject::initialize();
	return true;

}
void ScorePoint::update(){
	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	Timer += frameTime;
	if (up)
		transform.position.y -= frameTime*70;
	else
		transform.position.y += frameTime * 70;
	_text.setX(transform.position.x);
	_text.setY(transform.position.y);
	_text.setScale(transform.scale);
	_text.setRadians(transform.angle);
	_text.setVisible(visible);
	_text.update();
	if (Timer > LIMITTIME){
		Destroy();
		_text.setVisible(false);
	}
}

void ScorePoint::draw(){
	_text.setProportional(true);
	_text.setProportionalSpacing(1);
	_text.setFontColor(D3DCOLOR_ARGB( (int)((1 - Timer / 1.2f)*255),255,255,255));
	_text.setBackColor(TRANSCOLOR);
	_text.print(str);
}
void ScorePoint::ai(){

}
void ScorePoint::collision(){

}