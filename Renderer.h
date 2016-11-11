#ifndef _RENDERER_H
#define _RENDERER_H
#include "graphics.h"
#include "Task.h"
#include "GameSceneManager.h"
//Objectのレンダリング用のクラス
class Renderer:public Task{
protected:
	COLOR_ARGB colorFilter;
	bool initialized;
	Graphics* graphics;
	Transform* transform; //参照するTransform。GameObject本体を基本として参照先を変えたい場合など
public:
	Renderer();
	~Renderer();
	virtual void start(gameObject* t);
	virtual void update();
	virtual void draw();
	void SetTransform(Transform* t){ transform = t; }

};
#endif