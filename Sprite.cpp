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
//Spriteを初期化


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
//color をフィルタに使って画像を描画
//colorパラメータはオプション
//(image.hでデフォルトは白とし割り当てられている)
//実行前:spriteBegin()を呼び出す
//実行後:spriteEnd()を呼び出す
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
//指定されたspriteDataを使ってこの画像を描画
//現在のSpriteData.rectを使って、テクスチャを選択
//実行前:spriteBegin()を呼び出す
//実行後:spriteEnd()を呼び出す
//==========================================================================
void Sprite::draw(SpriteData sd, COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	//この画像の矩形をつかってテクスチャを選択
	sd.rect = spriteData.rect;
	//onResetが呼び出されたときに新しいテクスチャを取得
	sd.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER) //フィルタを使って描画する場合
		//colorFilteerを使用
		graphics->drawSprite(sd, colorFilter);
	else
		//フィルタとしてcolorを使用
		graphics->drawSprite(sd, color);

}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//Screen座標の調整を行う
//=============================================================================
void Sprite::update()
{

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	D3DXVECTOR2 Screenpos = graphics->GetSpriteScreenPosition(spriteData);
	spriteData.screenX = Screenpos.x;
	spriteData.screenY = Screenpos.y;

	if (endFrame - startFrame > 0)          // endFrameとstartFrameに差があるならアニメーションとみなす
	{
		animTimer += frameTime;             // animTimerにframeTimeを加算しカウント
		if (animTimer > frameDelay)			//frameDelay(コマごとの遅延、フレーム数に対応)の処理
		{
			animTimer -= frameDelay;
			currentFrame++;					//アニメーションフレームを一つずらす
			if (currentFrame < startFrame || currentFrame > endFrame) //現在のフレーム位置アニメーション範囲を超えたら
			{
				if (loop == true)            // loopしているなら
					currentFrame = loopFrame;	//最初のフレームへ
				else                        // loopなしなら
				{
					currentFrame = endFrame;	//ちがうなら最終フレームへ
					animComplete = true;    // animation complete
				}
			}
			setRect();                      // set spriteData.rect
		}
	}
}
//=============================================================================
// cを現在のフレームとする
//=============================================================================
void Sprite::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		currentFrame = c;			//アニメーションの現在位置を渡す
		animComplete = false;		//アニメーションの完遂を未達成に
		setRect();                          // set spriteData.rect
	}
}

//=============================================================================
//  現在のフレーム位置に矩形を移動
//=============================================================================
inline void Sprite::setRect()
{
	// 現在のフレーム番号に応じてスプライト上の切り取り位置を変える
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	// right edge + 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height + startHeight;
	// bottom edge + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}
