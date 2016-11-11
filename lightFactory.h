#ifndef _MFACTORY_H
#define _MFACTORY_H
#define  WIN32_LEAN_AND_MEAN

#include <list>
#include <map>
#include "light.h"
#include "constants.h"
//===========================================================
//Lightの登録を行う
//レンダリング処理の関係でLightだけを抽出したいときに使う
//===========================================================
class LightFactory
{
public:

	typedef struct LightLink{
		LightLink* previous;
		Light* pointer;
		LightLink* next;
		ELEMID id;
	};
	static void Create();
	static LightFactory &Instance(){
		return *instance;
	}


	static void Destroy();


	virtual ELEMID CreateElem(Light &obj);

	Light* GetElem(ELEMID id){
		if (m_IDMap.find(id) != m_IDMap.end())
			return m_IDMap[id]->pointer;
		else
			return nullptr;
	}

	LightLink* GetFirstLink(){

		return FirstLightLink;
	}
	LightLink* GetLastLink(){

		return LastLightLink;
	}


	ELEMID Getsize(){
		return m_IDMap.size();

	}
	void ReleaseEntity(ELEMID id);

	// 全オブジェクトクリア
	virtual void ClearAll();

	void ReleaseAll();
private:
	typedef std::pair<OBJID, LightLink* > OBJIDPAIR;	// オブジェクトペア
	typedef std::map< OBJID, LightLink* > OBJIDMAP;	// オブジェクトマップ

	OBJIDMAP m_IDMap;	// オブジェクト格納マップ

protected:
	LightFactory(){}
	virtual ~LightFactory(){};
	static LightFactory *instance;
	static LightLink *LastLightLink;
	static LightLink *FirstLightLink;


};


#endif