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
	TextureStorage::Instance().CreateTexture(graphics, RING_IMAGE);
	TextureStorage::Instance().CreateTexture(graphics, LOGO_IMAGE);
	TextureStorage::Instance().CreateTexture(graphics, ISHOOT_IMAGE);
	TextureStorage::Instance().CreateTexture(graphics, ITARGET_IMAGE);
	TextureStorage::Instance().CreateTexture(graphics, ISETUP_IMAGE);
	TextureStorage::Instance().CreateTexture(graphics, GAMEOVER_IMAGE);
	TextureStorage::Instance().CreateTexture(graphics, ISCORE_IMAGE);
}

//===========================================================
//Deleter�e�N�X�`�������ׂĔj������
//===========================================================
void TextureInitializer::DeleteAllTexture(){
	TextureStorage::Instance().ReleaseTexture(ARROW_IMAGE);
	TextureStorage::Instance().ReleaseTexture(RING_IMAGE);
	TextureStorage::Instance().ReleaseTexture(LOGO_IMAGE);
	TextureStorage::Instance().ReleaseTexture(ISHOOT_IMAGE);
	TextureStorage::Instance().ReleaseTexture(ITARGET_IMAGE);
	TextureStorage::Instance().ReleaseTexture(ISETUP_IMAGE);
	TextureStorage::Instance().ReleaseTexture(GAMEOVER_IMAGE);
	TextureStorage::Instance().ReleaseTexture(ISCORE_IMAGE);
}