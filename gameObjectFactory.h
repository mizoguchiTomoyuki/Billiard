#ifndef _GAMEOBJFACTORY_H
#define _GAMEOBJFACTORY_H
#define  WIN32_LEAN_AND_MEAN

#include <list>
#include <map>
#include "gameObject.h"
#include "constants.h"

//===========================================================
//gameObjectの物体の登録を行う
//動的物体gameObjectは管理しないので注意
//===========================================================
class gameObjectFactory
{
public:
	typedef struct gameObjectLink{
		gameObjectLink* previous;
		gameObject* pointer;
		gameObjectLink* next;
		ELEMID id;
	};
	static void Create();
	static bool initialized();
	static gameObjectFactory &Instance(){
		return *instance;
	}


	static void Destroy();


	virtual ELEMID CreateElem(gameObject &obj);

	gameObject* GetElem(ELEMID id){
		if (m_IDMap.find(id) != m_IDMap.end())
			return m_IDMap[id]->pointer;
		else
			return nullptr;
	}

	gameObjectLink* GetFirstLink(){

		return FirstgameObjectLink;
	}
	gameObjectLink* GetLastLink(){

		return LastgameObjectLink;
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
	gameObjectFactory(){}
	virtual ~gameObjectFactory(){};
	static gameObjectFactory *instance;
	static gameObjectLink *LastgameObjectLink;
	static gameObjectLink *FirstgameObjectLink;
private:
	
	typedef std::pair<OBJID, gameObjectLink* > OBJIDPAIR;	// オブジェクトペア
	typedef std::map< OBJID, gameObjectLink* > OBJIDMAP;	// オブジェクトマップ

	OBJIDMAP m_IDMap;	// オブジェクト格納マップ

};


#endif