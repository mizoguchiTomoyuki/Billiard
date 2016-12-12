#ifndef _BILLIARDS_H
#define _BILLIARDS_H
#define WIN32_LEAN_AND_MEAN
#include "game.h"
#include "GameSceneManager.h"
#include "gameObjectFactory.h"
#include "Ball.h"
#include "light.h"
#include "BallManager.h"
class Billiard : public Game
{
private :
	int Ang;
	BallManager _bmanager;
public:
	Billiard();
	virtual ~Billiard();
	void initialize(HWND hwnd);
	virtual void update();
	virtual void ai();
	virtual void collisions();
	virtual void render();
	virtual void releaseAll();
	virtual void resetAll();
	virtual void deleteAll();
	virtual void Lightupdate();
	virtual void gameObjectupdate();
	virtual void gameObjectdraw();
	virtual void ResetGame();
};

#endif