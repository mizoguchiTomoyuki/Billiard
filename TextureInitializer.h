#ifndef _TEXTURE_INITIALIZER_H
#define _TEXTURE_INITIALIZER_H
#define  WIN32_LEAN_AND_MEAN
#include "TextureManager.h"
#include "TextureStorage.h"
//===========================================================
//Texture�̏������݂̂��s��
//�܂�Texture�̃����[�X���s��
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