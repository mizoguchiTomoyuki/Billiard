#ifndef _SPRITEOBJFACTORY_H
#define _SPRITEOBJFACTORY_H
#define  WIN32_LEAN_AND_MEAN

#include <list>
#include <map>
#include "SpriteObject.h"
#include "constants.h"

//===========================================================
//SpriteObjectの物体の登録を行う
//動的物体SpriteObjectは管理しないので注意
//===========================================================
class SpriteObjectFactory
{
public:
	typedef struct SpriteObjectLink{
		SpriteObjectLink* previous;
		SpriteObject* pointer;
		SpriteObjectLink* next;
		ELEMID id;
	};
	static void Create();
	static bool initialized();
	static SpriteObjectFactory &Instance(){
		return *instance;
	}


	static void Destroy();


	virtual ELEMID CreateElem(SpriteObject &obj);

	SpriteObject* GetElem(ELEMID id){
		if (m_IDMap.find(id) != m_IDMap.end())
			return m_IDMap[id]->pointer;
		else
			return nullptr;
	}

	SpriteObjectLink* GetFirstLink(){

		return FirstSpriteObjectLink;
	}
	SpriteObjectLink* GetLastLink(){

		return LastSpriteObjectLink;
	}


	ELEMID Getsize(){
		return m_IDMap.size();

	}
	void ReleaseEntity(ELEMID id);
	// 全オブジェクトクリア
	virtual void ClearAll();
	virtual void ReleaseAll();
	virtual void Optimize();

protected:
	SpriteObjectFactory(){}
	virtual ~SpriteObjectFactory(){};
	static SpriteObjectFactory *instance;
	static SpriteObjectLink *LastSpriteObjectLink;
	static SpriteObjectLink *FirstSpriteObjectLink;
private:

	typedef std::pair<OBJID, SpriteObjectLink* > OBJIDPAIR;	// オブジェクトペア
	typedef std::map< OBJID, SpriteObjectLink* > OBJIDMAP;	// オブジェクトマップ

	OBJIDMAP m_IDMap;	// オブジェクト格納マップ

};


#endif