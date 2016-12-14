#ifndef _SPRITEOBJFACTORY_H
#define _SPRITEOBJFACTORY_H
#define  WIN32_LEAN_AND_MEAN

#include <list>
#include <map>
#include "SpriteObject.h"
#include "constants.h"

//===========================================================
//SpriteObject�̕��̂̓o�^���s��
//���I����SpriteObject�͊Ǘ����Ȃ��̂Œ���
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
	// �S�I�u�W�F�N�g�N���A
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

	typedef std::pair<OBJID, SpriteObjectLink* > OBJIDPAIR;	// �I�u�W�F�N�g�y�A
	typedef std::map< OBJID, SpriteObjectLink* > OBJIDMAP;	// �I�u�W�F�N�g�}�b�v

	OBJIDMAP m_IDMap;	// �I�u�W�F�N�g�i�[�}�b�v

};


#endif