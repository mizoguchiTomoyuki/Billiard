#include "BallManager.h"
#include "GameSceneManager.h"
#include "gameObjectFactory.h"
//コンストラクタ
BallManager::BallManager(){
	state = BMNS::NOSTART;

}

BallManager::~BallManager(){



}

void BallManager::update(){
	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	switch (state){
		case BMNS::NOSTART:
			break;
		case BMNS::START:
			state = BMNS::TARGETBALL;

			break;
		case BMNS::TARGETBALL:
			if (TargetBall()){
				state = BMNS::SHOOTBALL;

			}
			break;
		case BMNS::SHOOTBALL:
			if (ShootBall()){
				state = BMNS::WAITSTOPBALL;

			}
			break;
		case BMNS::WAITSTOPBALL:
			if (waitBall()){
				state = BMNS::TARGETBALL;

			}
			break;

		case BMNS::SETUPBALL:
			if (SetBall()){
				state = BMNS::TARGETBALL;

			}
			break;

		case BMNS::GAMEOVER:
			break;
		case BMNS::END:
				break;
		default:
				break;


	}


}

void BallManager::GameStart(){
	state = BMNS::START;
	float r = 0.7f;//球の半径
	D3DXVECTOR3 pos[9];
	D3DXVECTOR3 origin = {0,0,0}; //初期のひし形中央に置く球の位置
	//ひし形配置用のポジション値
	pos[0] = {2.0f*sqrt(3.0f)*r,0,0};	 //1
	pos[1] = { 0, 0, -2 * r };			//2
	pos[2] = { -2.0f*sqrt(3.0f)*r, 0, 0 };//3
	pos[3] = { 0, 0, 2 * r };//4
	pos[4] = { sqrt(3.0f)*r, 0, -r };
	pos[5] = { sqrt(3.0f)*r, 0, r };
	pos[6] = { -sqrt(3.0f)*r, 0, -r };
	pos[7] = { -sqrt(3.0f)*r, 0, r };
	pos[8] = { 0, 0, 0 };

	for (int i = 0; i < 9; i++){
		balls[i] = _prefab.PrefabCreate((PREFAB_MENU)(i + 5));
		gameObject* obj = gameObjectFactory::Instance().GetElem(balls[i]);
		obj->setPosition(pos[i] + origin); //上の値に中心値を足してセット
	}

	D3DXVECTOR3 mBallpos = { 7, 0, 0 };//自球の位置今は適当

	_myball = _prefab.PrefabCreate((PREFAB_MENU)(0));
	gameObject* obj = gameObjectFactory::Instance().GetElem(_myball);
	obj->setPosition(mBallpos); //上の値に中心値を足してセット
	obj->SetName("MyBall");

	
}

//ボールを打ったかどうかの判定ボールを打ったならばtrueを返す
bool BallManager::ShootBall(){
	Input* in = GameSceneManager::Instance().GetGameptr()->getInput();

	if (in->getMouseRButton()){
		gameObject* ball = gameObjectFactory::Instance().GetElem(_myball);
		shootvec = { shootvec.x, 0, shootvec.z };
		ball->setDeltaV(shootvec*20.0f);
		return true;

	}

	return false;
}
//ボールがすべて止まった時にtrue
bool BallManager::waitBall(){
	bool ret = true;
	for (int i = 0; i < 9; i++){
		Ball* ball = (Ball*)gameObjectFactory::Instance().GetElem(balls[i]);
		if (ball->isMoving()){

			ret = false;
		}
		

	}
		Ball* ball = (Ball*)gameObjectFactory::Instance().GetElem(_myball);
		if (ball->isMoving()){

			ret = false;
		}
	return ret;

}
//ボールの狙いを定める
bool BallManager::TargetBall(){
	Input* in = GameSceneManager::Instance().GetGameptr()->getInput();
	gameObject* ball = gameObjectFactory::Instance().GetElem(_myball);
	D3DXVECTOR3 spos; //スクリーン上の位置
	GameSceneManager::Instance().GetGameptr()->getGraphics()->getScreenPosition(ball->getTransform()->getPosition(), spos);
	spos = { spos.x, 0, spos.y };
	
	D3DXVECTOR3 mPos = { (float)in->getMouseX(), 0, (float)in->getMouseY() };
	if (in->getMouseLButton()){
		shootvec = mPos - spos;
		D3DXVec3Normalize(&shootvec, &shootvec);
		return true;

	}


	return false;
}

//白球のセットしなおし配置したらtrue;
bool BallManager::SetBall(){

	return false;

}