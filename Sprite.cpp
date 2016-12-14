#include "Sprite.h"
#include "GameSceneManager.h"
Sprite::Sprite()
{
	initialized = false;
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.z = 0.0;
	spriteData.ratioZ = 1.0;
	spriteData.screenX = 0.0f;
	spriteData.screenY = 0.0f;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;       // used to select one frame from multi-frame image
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;      // the sprite texture (picture)
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	loopFrame = 0;
	endFrame = 0;
	startHeight = 0;
	currentFrame = 0;
	frameDelay = 1.0;               // default to 1 second per frame of animation
	animTimer = 0.0;
	visible = true;                 // the image is visible
	loop = true;                    // loop frames
	animComplete = false;
	graphics = NULL;                // link to graphics system
	colorFilter = graphicsNS::WHITE; // WHITE for no change
	RefCounter = 0;


}

Sprite::~Sprite(){}

//==========================================================================
//Sprite��������


//==========================================================================

bool Sprite::initialize(Graphics *g, int width, int height,int ncols, TextureManager *textureM){
	try{
		graphics = g;
		textureManager = textureM;
		spriteData.texture = textureManager->getTexture();

		if (width == 0)
			width = textureManager->getWidth();
		
		if (height == 0)
			height = textureManager->getHeight();
		spriteData.height = height;
		spriteData.width = width;
		cols = ncols;

		if (cols == 0)
			cols = 1;

		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols)*spriteData.height;
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch (...){ return false; }
	initialized = true;
	return true;

}
//==========================================================================
//color ���t�B���^�Ɏg���ĉ摜��`��
//color�p�����[�^�̓I�v�V����
//(image.h�Ńf�t�H���g�͔��Ƃ����蓖�Ă��Ă���)
//���s�O:spriteBegin()���Ăяo��
//���s��:spriteEnd()���Ăяo��
//==========================================================================
void Sprite::draw(COLOR_ARGB color){
	if (!visible || graphics == NULL)
		return;

	spriteData.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER)
		graphics->drawSprite(spriteData, colorFilter);
	else
		graphics->drawSprite(spriteData, color);

}

//==========================================================================
//�w�肳�ꂽspriteData���g���Ă��̉摜��`��
//���݂�SpriteData.rect���g���āA�e�N�X�`����I��
//���s�O:spriteBegin()���Ăяo��
//���s��:spriteEnd()���Ăяo��
//==========================================================================
void Sprite::draw(SpriteData sd, COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	//���̉摜�̋�`�������ăe�N�X�`����I��
	sd.rect = spriteData.rect;
	//onReset���Ăяo���ꂽ�Ƃ��ɐV�����e�N�X�`�����擾
	sd.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER) //�t�B���^���g���ĕ`�悷��ꍇ
		//colorFilteer���g�p
		graphics->drawSprite(sd, colorFilter);
	else
		//�t�B���^�Ƃ���color���g�p
		graphics->drawSprite(sd, color);

}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//Screen���W�̒������s��
//=============================================================================
void Sprite::update()
{

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	D3DXVECTOR2 Screenpos = graphics->GetSpriteScreenPosition(spriteData);
	spriteData.screenX = Screenpos.x;
	spriteData.screenY = Screenpos.y;

	if (endFrame - startFrame > 0)          // endFrame��startFrame�ɍ�������Ȃ�A�j���[�V�����Ƃ݂Ȃ�
	{
		animTimer += frameTime;             // animTimer��frameTime�����Z���J�E���g
		if (animTimer > frameDelay)			//frameDelay(�R�}���Ƃ̒x���A�t���[�����ɑΉ�)�̏���
		{
			animTimer -= frameDelay;
			currentFrame++;					//�A�j���[�V�����t���[��������炷
			if (currentFrame < startFrame || currentFrame > endFrame) //���݂̃t���[���ʒu�A�j���[�V�����͈͂𒴂�����
			{
				if (loop == true)            // loop���Ă���Ȃ�
					currentFrame = loopFrame;	//�ŏ��̃t���[����
				else                        // loop�Ȃ��Ȃ�
				{
					currentFrame = endFrame;	//�������Ȃ�ŏI�t���[����
					animComplete = true;    // animation complete
				}
			}
			setRect();                      // set spriteData.rect
		}
	}
}
//=============================================================================
// c�����݂̃t���[���Ƃ���
//=============================================================================
void Sprite::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		currentFrame = c;			//�A�j���[�V�����̌��݈ʒu��n��
		animComplete = false;		//�A�j���[�V�����̊����𖢒B����
		setRect();                          // set spriteData.rect
	}
}

//=============================================================================
//  ���݂̃t���[���ʒu�ɋ�`���ړ�
//=============================================================================
inline void Sprite::setRect()
{
	// ���݂̃t���[���ԍ��ɉ����ăX�v���C�g��̐؂���ʒu��ς���
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	// right edge + 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height + startHeight;
	// bottom edge + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}
