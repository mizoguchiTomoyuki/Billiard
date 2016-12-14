#include "TextureManager.h"
//コンストラクタいろいろなものを初期化
TextureManager::TextureManager(){
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;


}
TextureManager::~TextureManager()
{
	SAFE_RELEASE(texture);
}

//==========================================================================
//textureにfのファイルを読み込む
//==========================================================================
bool TextureManager::initialize(Graphics* g, const char *f)
{
	try{
		graphics = g;
		file = f;
		hr = graphics->loadTexture(
			file, TRANSCOLOR, width, height, texture);
		if (FAILED(hr))
		{
			SAFE_RELEASE(texture);
			return false;
		}


	}
	catch (...){ return false; }
	initialized = true;
	return true;

}

//==========================================================================
//グラフィックデバイスが消失したときに呼び出される
//==========================================================================
void TextureManager::onLostDevice()
{
	if (initialized)
		return;
	SAFE_RELEASE(texture);
}

//==========================================================================
//グラフィックスデバイスがリセットされたときに呼び出される
//==========================================================================
void TextureManager::onResetDevice()
{
	if (initialized)
		return;
	graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}