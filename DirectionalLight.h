#ifndef _DLIGHT_H
#define _DLIGHT_H
#include "gameObject.h"
//生成のテストで作った
//DirectionalLightの機能を持ったオブジェクト
//CreateConsoleでTaskのgameObjectへのAttachがおわるまで使う
class Light;
class GameSceneManager;
class LightFactory;
class DirectionalLight : public gameObject{
protected:
	Light* _light;
	ELEMID _light_id;
public:
	DirectionalLight();
	virtual ~DirectionalLight();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};
#include "light.h"
#include "lightFactory.h"
#include "GameSceneManager.h"
#endif