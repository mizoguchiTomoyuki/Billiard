#ifndef _MOVERFACTORY_H
#define _MOVERFACTORY_H
#define  WIN32_LEAN_AND_MEAN
#include <list>
#include <map>
#include "TextureManager.h"
#include "graphics.h"

typedef size_t OBJID;	// オブジェクトID
typedef const std::string TEXNAME;	// 要素ID

//===========================================================
//Testure群の管理を行う
//===========================================================
class TextureStorage
{

	typedef std::pair<TEXNAME, TextureManager* > TEXPAIR;	// オブジェクトペア
	typedef std::map< TEXNAME, TextureManager* > TEXMAP;	// オブジェクトマップ

	TEXMAP m_TextureMap;	// オブジェクト格納マップ

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

	// 全オブジェクトクリア
	virtual void ClearAll();
	void ReleaseTexture(TEXNAME name_id);

	void releaseAll();
	void resetAll();
};


#endif