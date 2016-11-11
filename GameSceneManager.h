#ifndef _GAMESCENEMANAGER_H
#define _GAMESCENEMANAGER_H
#define  WIN32_LEAN_AND_MEAN
#include <list>
#include <map>
#include "game.h"
typedef size_t OBJID;	// �I�u�W�F�N�gID
typedef const std::string TEXNAME;	// �v�fID

//===========================================================
//�Q�[���V�[���̃|�C���^��ێ����Ă���
//===========================================================
class GameSceneManager
{

	Game* gamePointer;

protected:
	GameSceneManager(){}
	virtual ~GameSceneManager(){};
	static GameSceneManager *instance;
public:
	static void Create();
	static void Destroy();
	static GameSceneManager &Instance(){
		return *instance;
	}

	Game* GetGameptr(){
		return gamePointer;
	}
	void SetGamePtr(Game* gptr){
		gamePointer = gptr;
	}

};


#endif