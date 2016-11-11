#include "GameSceneManager.h"

GameSceneManager *GameSceneManager::instance = nullptr;
void GameSceneManager::Create(){
	if (!instance){
		instance = new GameSceneManager;
	}
}


void GameSceneManager::Destroy(){

	delete instance;
	instance = nullptr;
}
