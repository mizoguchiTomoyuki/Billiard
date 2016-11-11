#ifndef _GAMEOBJFACTORY_H
#define _GAMEOBJFACTORY_H
#define  WIN32_LEAN_AND_MEAN

#include <list>
#include <map>
#include "gameObject.h"
#include "constants.h"

//===========================================================
//gameObject�̕��̂̓o�^���s��
//���I����gameObject�͊Ǘ����Ȃ��̂Œ���
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
	// �S�I�u�W�F�N�g�N���A
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
	
	typedef std::pair<OBJID, gameObjectLink* > OBJIDPAIR;	// �I�u�W�F�N�g�y�A
	typedef std::map< OBJID, gameObjectLink* > OBJIDMAP;	// �I�u�W�F�N�g�}�b�v

	OBJIDMAP m_IDMap;	// �I�u�W�F�N�g�i�[�}�b�v

};


#endif