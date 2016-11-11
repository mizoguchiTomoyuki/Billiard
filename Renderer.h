#ifndef _RENDERER_H
#define _RENDERER_H
#include "graphics.h"
#include "Task.h"
#include "GameSceneManager.h"
//Object�̃����_�����O�p�̃N���X
class Renderer:public Task{
protected:
	COLOR_ARGB colorFilter;
	bool initialized;
	Graphics* graphics;
	Transform* transform; //�Q�Ƃ���Transform�BGameObject�{�̂���{�Ƃ��ĎQ�Ɛ��ς������ꍇ�Ȃ�
public:
	Renderer();
	~Renderer();
	virtual void start(gameObject* t);
	virtual void update();
	virtual void draw();
	void SetTransform(Transform* t){ transform = t; }

};
#endif