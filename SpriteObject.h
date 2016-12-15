#ifndef _SPRITE_OBJECT_H
#define _SPRITE_OBJECT_H
#include "Sprite.h"
struct STransform{
	D3DXVECTOR2 position;
	float scale;
	float angle;
	STransform* parent;
	D3DXVECTOR2 getPosition(){ return position; }
	float getScale(){ return scale; }
	float getRotation(){ return angle; }

};
//スプライト用のオブジェクトクラス
class SpriteObject{
protected:
	STransform transform;
	Sprite sprite;
	int _id;
	bool visible;
	bool destroy;
public:
	SpriteObject();
	virtual ~SpriteObject();
	virtual bool initialize(int width, int height, int ncols, TextureManager *textureM);
	virtual void update(){};
	virtual void draw(){};
	virtual void ai(){};
	virtual void collision(){};
	D3DXVECTOR2 getPosition(){ return transform.position; }
	float getScale(){ return transform.scale; }
	float getRotation(){ return transform.angle; }
	float getWidth(){ return sprite.getWidth(); }
	float getHeight(){ return sprite.getHeight(); }
	virtual void setELEMID(ELEMID id){ _id = id; }
	virtual ELEMID getELEMID(){ return _id; }
	void setPosition(D3DXVECTOR2 pos){ transform.position = pos; }
	void setScale(float sca){ transform.scale = sca; }
	void setAngle(float a){ transform.angle = a; }
	bool isDestroy(){ return destroy; }
	void Destroy(){ destroy = true; }
	void isVisible(bool b){ visible = b; }

};
#endif