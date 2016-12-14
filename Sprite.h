#ifndef SPRITE_H
#define SPRITE_H
//2D�摜�̕\���ɗ��p����
#include "graphics.h"
#include "TextureManager.h"
class Sprite{
protected:
	Graphics* graphics;
	TextureManager *textureManager;
	SpriteData spriteData;
	int cols; //��
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
	//get�֐�
	//==================================================================
	//SpriteData�\���̂ւ̎Q�Ƃ�Ԃ�
	const virtual SpriteData& getSpriteInfo(){ return spriteData; }
	//visible�p�����[�^��Ԃ�
	virtual bool getVisible(){ return visible; }
	//X�ʒu��Ԃ�
	virtual float getX(){ return spriteData.x; }
	//Y�ʒu��Ԃ�
	virtual float getY(){ return spriteData.y; }
	//�{����Ԃ�
	virtual float getScale(){ return spriteData.scale; }
	//����Ԃ�
	virtual int getWidth(){ return spriteData.width; }
	//������Ԃ�
	virtual int getHeight(){ return spriteData.height; }
	//���S�_��X��Ԃ�
	virtual float getCenterX(){
		return spriteData.x + (spriteData.width / 2) * getScale();
	}
	virtual float getScreenCenterX(){
		return spriteData.x + (spriteData.width / 2) * getScale() + graphics->getCameraPos().x;
	}
	//���S�_��Y��Ԃ�
	virtual float getCenterY(){
		return spriteData.y + (spriteData.height / 2) * getScale();

	}
	//��]�p�x(�x�P��)��Ԃ�
	virtual float getDigrees(){ return spriteData.angle*(180.0f / (float)PI); }
	//��]�p�x(���W�A���P��)��Ԃ�
	virtual float getRadians(){ return spriteData.angle; }
	//�A�j���[�V�����̃t���[���Ԃ̊Ԋu��Ԃ�
	virtual float getFrameDelay(){ return frameDelay; }
	//�J�n�t���[���ԍ���Ԃ�
	virtual int getStartFrame(){ return startFrame; }
	//�I���t���[���ԍ���Ԃ�
	virtual int getEndFrame(){ return endFrame; }
	//���݂̃t���[���ԍ���Ԃ�
	virtual int getCurrentFrame(){ return currentFrame; }
	//Image��RECT�\���̂�Ԃ�
	virtual RECT getSpriteDataRect(){ return spriteData.rect; }
	//�A�j���[�V���������̏�Ԃ�Ԃ�
	virtual bool getAnimationComplete(){ return animComplete; }
	//colorFilter��Ԃ�
	virtual COLOR_ARGB getColorFilter(){ return colorFilter; }
	virtual int getAnimationNumber(){ return animationNumber; }

	virtual int getRefCounter(){ return RefCounter; }
	//==================================================================
	//set�֐�
	//==================================================================
	//�ʒu��ݒ�
	virtual void refStart(){ RefCounter++; } //�Q�ƃJ�E���^�𑝂₷
	virtual void Destroy(){ RefCounter--; }   //�Q�ƃJ�E���^�����炷
	virtual void setX(float newX){ spriteData.x = newX; }
	virtual void setY(float newY){ spriteData.y = newY; }
	//�g��k����ݒ�
	virtual void setScreenX(float newX){ spriteData.screenX = newX; }
	virtual void setScreenY(float newY){ spriteData.screenY = newY; }
	virtual void setScale(float s){ spriteData.scale = s; }
	//��]�p�x��ݒ�(�x�P��)
	//0�x�͏�����ł��B�p�x�����v���ɐi�݂܂�
	virtual void setDegrees(float deg){ spriteData.angle = deg*((float)PI / 180.0f); }
	//��]�p�x��ݒ�i���W�A���P�ʁj
	//0���W�A���͏����
	virtual void setRadians(float rad){ spriteData.angle = rad; }
	//������ݒ�
	virtual void setVisible(bool v){ visible = v; }
	//�A�j���[�V�����̃t���[���Ԃ̊Ԋu��ݒ�
	virtual void setFrameDelay(float d){ frameDelay = d; }
	//�A�j���[�V�����̊J�n�t���[���ƏI���t���[����ݒ�
	virtual void setFrames(int s, int e){ startFrame = s; endFrame = e; }
	//�A�j���[�V�����̌��݂̃t���[����ݒ�
	virtual void setCurrentFrame(int c);
	//currentFrame��`�悷��spriteData.rect��ݒ�
	virtual void setRect();
	//currentFrame��`�悷��spriteData.rect��ݒ�
	//spriteData.rect��r�ɐݒ�
	virtual void setSpriteDataRect(RECT r){ spriteData.rect = r; }
	//�A�j���[�V�������[�v��ݒ�(lp = true �̏ꍇ�Ƀ��[�v����)
	virtual void setLoop(bool lp){ loop = lp; }
	//�A�j���[�V���������u�[���l��ݒ�
	virtual void setAnimationComplete(bool a){ animComplete = a; }
	//�F�t�B���^��ݒ�(WHITE���g�p����ƕω����Ȃ�)
	virtual void setColorFilter(COLOR_ARGB color) { colorFilter = color; }
};

#endif