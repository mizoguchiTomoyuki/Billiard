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
}

//===========================================================
//Deleterテクスチャをすべて破棄する
//===========================================================
void TextureInitializer::DeleteAllTexture(){
	TextureStorage::Instance().ReleaseTexture(ARROW_IMAGE);
}