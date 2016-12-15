#include "TextureInitializer.h"
//===========================================================
//コンストラクタ
//===========================================================
TextureInitializer::TextureInitializer(){
	TextureStorage::Create();
}
//===========================================================
//デストラクタ
//===========================================================
TextureInitializer::~TextureInitializer(){
	DeleteAllTexture();
	TextureStorage::Destroy();
}

//===========================================================
//initializerテクスチャをすべて読み込む
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
//Deleterテクスチャをすべて破棄する
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