#ifndef _TEXTURE_INITIALIZER_H
#define _TEXTURE_INITIALIZER_H
#define  WIN32_LEAN_AND_MEAN
#include "TextureManager.h"
#include "TextureStorage.h"
//===========================================================
//Textureの初期化のみを行う
//またTextureのリリースも行う
//===========================================================
class TextureInitializer{
private:
public:
	TextureInitializer();
	virtual ~TextureInitializer();
	void initializeTexture(Graphics *graphics);
	void DeleteAllTexture();
};
#endif