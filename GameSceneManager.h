#ifndef _GAMESCENEMANAGER_H
#define _GAMESCENEMANAGER_H
#define  WIN32_LEAN_AND_MEAN
#include <list>
#include <map>
#include "game.h"
typedef size_t OBJID;	// オブジェクトID
typedef const std::string TEXNAME;	// 要素ID

//===========================================================
//ゲームシーンのポインタを保持している
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