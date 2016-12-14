#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H
//�e�N�X�`���̏��������s���N���X
#include "graphics.h"
#include "constants.h"
class TextureManager{
private:
	Graphics *graphics;
	bool initialized;
	const char* file;
	HRESULT hr;
	UINT width;
	UINT height;
	LP_TEXTURE texture;
public:
	TextureManager();
	~TextureManager();
	bool initialize(Graphics *g, const char *f);
	void onLostDevice();
	void onResetDevice();

	LP_TEXTURE getTexture() const { return texture; }
	UINT getWidth() const { return width; }
	UINT getHeight() const { return height; }


};

#endif