#ifndef SPRITE_H
#define SPRITE_H
//2D画像の表示に利用する
#include "graphics.h"
#include "TextureManager.h"
class Sprite{
protected:
	Graphics* graphics;
	TextureManager *textureManager;
	SpriteData spriteData;
	int cols; //列数
	int currentFrame;
	int startFrame;
	int endFrame;
	int loopFrame;
	int startHeight;
	int animationNumber;
	double animTimer;
	float frameDelay;
	bool animComplete;
	bool loop;

	bool initialized;
	bool visible;
	COLOR_ARGB colorFilter;
	int RefCounter;
public:
	Sprite();
	~Sprite();
	virtual bool initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM);
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);
	virtual void draw(SpriteData sd,COLOR_ARGB color = graphicsNS::WHITE);
	virtual void flipHorizontal(bool flip)
	{
		spriteData.flipHorizontal = flip;
	}

	virtual void flipVertical(bool flip)
	{
		spriteData.flipVertical = flip;
	}
	void update();

	//==================================================================
	//get関数
	//==================================================================
	//SpriteData構造体への参照を返す
	const virtual SpriteData& getSpriteInfo(){ return spriteData; }
	//visibleパラメータを返す
	virtual bool getVisible(){ return visible; }
	//X位置を返す
	virtual float getX(){ return spriteData.x; }
	//Y位置を返す
	virtual float getY(){ return spriteData.y; }
	//倍率を返す
	virtual float getScale(){ return spriteData.scale; }
	//幅を返す
	virtual int getWidth(){ return spriteData.width; }
	//高さを返す
	virtual int getHeight(){ return spriteData.height; }
	//中心点のXを返す
	virtual float getCenterX(){
		return spriteData.x + (spriteData.width / 2) * getScale();
	}
	virtual float getScreenCenterX(){
		return spriteData.x + (spriteData.width / 2) * getScale() + graphics->getCameraPos().x;
	}
	//中心点のYを返す
	virtual float getCenterY(){
		return spriteData.y + (spriteData.height / 2) * getScale();

	}
	//回転角度(度単位)を返す
	virtual float getDigrees(){ return spriteData.angle*(180.0f / (float)PI); }
	//回転角度(ラジアン単位)を返す
	virtual float getRadians(){ return spriteData.angle; }
	//アニメーションのフレーム間の間隔を返す
	virtual float getFrameDelay(){ return frameDelay; }
	//開始フレーム番号を返す
	virtual int getStartFrame(){ return startFrame; }
	//終了フレーム番号を返す
	virtual int getEndFrame(){ return endFrame; }
	//現在のフレーム番号を返す
	virtual int getCurrentFrame(){ return currentFrame; }
	//ImageのRECT構造体を返す
	virtual RECT getSpriteDataRect(){ return spriteData.rect; }
	//アニメーション完了の状態を返す
	virtual bool getAnimationComplete(){ return animComplete; }
	//colorFilterを返す
	virtual COLOR_ARGB getColorFilter(){ return colorFilter; }
	virtual int getAnimationNumber(){ return animationNumber; }

	virtual int getRefCounter(){ return RefCounter; }
	//==================================================================
	//set関数
	//==================================================================
	//位置を設定
	virtual void refStart(){ RefCounter++; } //参照カウンタを増やす
	virtual void Destroy(){ RefCounter--; }   //参照カウンタを減らす
	virtual void setX(float newX){ spriteData.x = newX; }
	virtual void setY(float newY){ spriteData.y = newY; }
	//拡大縮小を設定
	virtual void setScreenX(float newX){ spriteData.screenX = newX; }
	virtual void setScreenY(float newY){ spriteData.screenY = newY; }
	virtual void setScale(float s){ spriteData.scale = s; }
	//回転角度を設定(度単位)
	//0度は上向きです。角度を時計回りに進みます
	virtual void setDegrees(float deg){ spriteData.angle = deg*((float)PI / 180.0f); }
	//回転角度を設定（ラジアン単位）
	//0ラジアンは上向き
	virtual void setRadians(float rad){ spriteData.angle = rad; }
	//可視性を設定
	virtual void setVisible(bool v){ visible = v; }
	//アニメーションのフレーム間の間隔を設定
	virtual void setFrameDelay(float d){ frameDelay = d; }
	//アニメーションの開始フレームと終了フレームを設定
	virtual void setFrames(int s, int e){ startFrame = s; endFrame = e; }
	//アニメーションの現在のフレームを設定
	virtual void setCurrentFrame(int c);
	//currentFrameを描画するspriteData.rectを設定
	virtual void setRect();
	//currentFrameを描画するspriteData.rectを設定
	//spriteData.rectをrに設定
	virtual void setSpriteDataRect(RECT r){ spriteData.rect = r; }
	//アニメーションループを設定(lp = true の場合にループする)
	virtual void setLoop(bool lp){ loop = lp; }
	//アニメーション完了ブール値を設定
	virtual void setAnimationComplete(bool a){ animComplete = a; }
	//色フィルタを設定(WHITEを使用すると変化しない)
	virtual void setColorFilter(COLOR_ARGB color) { colorFilter = color; }
};

#endif