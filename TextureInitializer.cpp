#include "TextureInitializer.h"
//===========================================================
//�R���X�g���N�^
//===========================================================
TextureInitializer::TextureInitializer(){
	TextureStorage::Create();
}
//===========================================================
//�f�X�g���N�^
//===========================================================
TextureInitializer::~TextureInitializer(){
	DeleteAllTexture();
	TextureStorage::Destroy();
}

//===========================================================
//initializer�e�N�X�`�������ׂēǂݍ���
//===========================================================
void TextureInitializer::initializeTexture(Graphics *graphics){
	TextureStorage::Instance().CreateTexture(graphics, ARROW_IMAGE);
}

//===========================================================
//Deleter�e�N�X�`�������ׂĔj������
//===========================================================
void TextureInitializer::DeleteAllTexture(){
	TextureStorage::Instance().ReleaseTexture(ARROW_IMAGE);
}