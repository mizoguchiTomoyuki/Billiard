#ifndef _CAMERAFACTORY_H
#define _CAMERAFACTORY_H
#define  WIN32_LEAN_AND_MEAN

#include <list>
#include <map>

#include "constants.h"

struct CameraData;
//===========================================================
//Camera�̓o�^���s��
//Main�J�����̏�񂪕K�v�Ȃ̂�Main�̃J�������������Ǘ�����
//===========================================================
class CameraFactory
{
public:
	typedef struct CameraLink{
		CameraLink* previous;
		CameraData* pointer;
		CameraLink* next;
		ELEMID id;
	};

	static void Create();
	static bool initialized();
	static CameraFactory &Instance(){
		return *instance;
	}


	static void Destroy();


	virtual ELEMID CreateElem(CameraData &obj);

	CameraData* GetElem(ELEMID id){
		if (m_IDMap.find(id) != m_IDMap.end())
			return m_IDMap[id]->pointer;
		else
			return nullptr;
	}

	CameraLink* GetFirstLink(){

		return FirstCameraLink;
	}
	CameraLink* GetLastLink(){

		return LastCameraLink;
	}


	ELEMID Getsize(){
		return m_IDMap.size();

	}
	void ReleaseEntity(ELEMID id);
	// �S�I�u�W�F�N�g�N���A
	virtual void ClearAll();
	virtual void ReleaseAll();
	virtual void SetMainCamera(ELEMID id){
		_Main_id = id;
	}//�J������ID���猻�݂̃��C���J������ID��ύX����
	CameraData* GetMainCamera(){
		return GetElem(_Main_id);
	}

private:
	typedef std::pair<OBJID, CameraLink* > OBJIDPAIR;	// �I�u�W�F�N�g�y�A
	typedef std::map< OBJID, CameraLink* > OBJIDMAP;	// �I�u�W�F�N�g�}�b�v

	OBJIDMAP m_IDMap;	// �I�u�W�F�N�g�i�[�}�b�v
	ELEMID _Main_id;
protected:
	CameraFactory(){}
	virtual ~CameraFactory(){};
	static CameraFactory *instance;
	static CameraLink *LastCameraLink;
	static CameraLink *FirstCameraLink;

};
#include "graphics.h"

#endif