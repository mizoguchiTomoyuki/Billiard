#ifndef _MFACTORY_H
#define _MFACTORY_H
#define  WIN32_LEAN_AND_MEAN

#include <list>
#include <map>
#include "light.h"
#include "constants.h"
//===========================================================
//Light�̓o�^���s��
//�����_�����O�����̊֌W��Light�����𒊏o�������Ƃ��Ɏg��
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

	// �S�I�u�W�F�N�g�N���A
	virtual void ClearAll();

	void ReleaseAll();
private:
	typedef std::pair<OBJID, LightLink* > OBJIDPAIR;	// �I�u�W�F�N�g�y�A
	typedef std::map< OBJID, LightLink* > OBJIDMAP;	// �I�u�W�F�N�g�}�b�v

	OBJIDMAP m_IDMap;	// �I�u�W�F�N�g�i�[�}�b�v

protected:
	LightFactory(){}
	virtual ~LightFactory(){};
	static LightFactory *instance;
	static LightLink *LastLightLink;
	static LightLink *FirstLightLink;


};


#endif