#ifndef _MOVERFACTORY_H
#define _MOVERFACTORY_H
#define  WIN32_LEAN_AND_MEAN
#include <list>
#include <map>
#include "TextureManager.h"
#include "graphics.h"

typedef size_t OBJID;	// �I�u�W�F�N�gID
typedef const std::string TEXNAME;	// �v�fID

//===========================================================
//Testure�Q�̊Ǘ����s��
//===========================================================
class TextureStorage
{

	typedef std::pair<TEXNAME, TextureManager* > TEXPAIR;	// �I�u�W�F�N�g�y�A
	typedef std::map< TEXNAME, TextureManager* > TEXMAP;	// �I�u�W�F�N�g�}�b�v

	TEXMAP m_TextureMap;	// �I�u�W�F�N�g�i�[�}�b�v

protected:
	TextureStorage(){}
	virtual ~TextureStorage(){};
	static TextureStorage *instance;
public:
	static void Create();
	static TextureStorage &Instance(){
		return *instance;
	}


	static void Destroy();


	virtual bool CreateTexture(Graphics *graphics, TEXNAME name_id);
	TextureManager* GetTexture(TEXNAME id){
		return m_TextureMap[id];
	}
	int Getsize(){
		return m_TextureMap.size();

	}

	// �S�I�u�W�F�N�g�N���A
	virtual void ClearAll();
	void ReleaseTexture(TEXNAME name_id);

	void releaseAll();
	void resetAll();
};


#endif