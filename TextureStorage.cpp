#include "TextureStorage.h"
TextureStorage *TextureStorage::instance = nullptr;
void TextureStorage::Create(){
	if (!instance){
		instance = new TextureStorage;
	}
}


void TextureStorage::Destroy(){

	delete instance;
	instance = nullptr;
}


bool TextureStorage::CreateTexture(Graphics *graphics, TEXNAME name_id){
	TextureManager* newTex = new TextureManager;
	const char* str = name_id.c_str();
	if (!newTex->initialize(graphics, str))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing spaceshipTexture"));

	TEXPAIR NewpointPair(name_id, newTex);
	m_TextureMap.insert(NewpointPair);
	return true;
}



// �S�I�u�W�F�N�g�N���A
void TextureStorage::ClearAll()
{
	m_TextureMap.clear();
}

void TextureStorage::ReleaseTexture(TEXNAME name_id){
	m_TextureMap[name_id]->onLostDevice();
	m_TextureMap[name_id]->onResetDevice();
	delete m_TextureMap[name_id];
	m_TextureMap[name_id] = nullptr;
	m_TextureMap.erase(name_id);

}


//=====================================================================
//�O���t�B�b�N�X�f�o�C�X�����������ꍇ
//�O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
//�\�񂳂�Ă����r�f�I�����������ׂĉ��
//=====================================================================
void TextureStorage::releaseAll()
{
	TEXMAP::iterator it;
	for (it = m_TextureMap.begin(); it != m_TextureMap.end();){

		it->second->onLostDevice();
		it++;
	}

	return;
}


//=====================================================================
//�O���t�B�b�N�X�f�o�C�X�����������ꍇ
//�O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
//�\�񂳂�Ă����r�f�I�����������ׂĉ��
//=====================================================================
void TextureStorage::resetAll()
{
	TEXMAP::iterator it;
	for (it = m_TextureMap.begin(); it != m_TextureMap.end();){

		it->second->onResetDevice();
		it++;
	}

	return;
}