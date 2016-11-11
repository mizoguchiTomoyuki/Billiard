#ifndef _DLIGHT_H
#define _DLIGHT_H
#include "gameObject.h"
//�����̃e�X�g�ō����
//DirectionalLight�̋@�\���������I�u�W�F�N�g
//CreateConsole��Task��gameObject�ւ�Attach�������܂Ŏg��
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