#include "TextureManager.h"
//�R���X�g���N�^���낢��Ȃ��̂�������
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
//texture��f�̃t�@�C����ǂݍ���
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
//�O���t�B�b�N�f�o�C�X�����������Ƃ��ɌĂяo�����
//==========================================================================
void TextureManager::onLostDevice()
{
	if (initialized)
		return;
	SAFE_RELEASE(texture);
}

//==========================================================================
//�O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
//==========================================================================
void TextureManager::onResetDevice()
{
	if (initialized)
		return;
	graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}