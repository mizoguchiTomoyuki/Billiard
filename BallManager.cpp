#include "BallManager.h"
#include "GameSceneManager.h"
#include "gameObjectFactory.h"
#include "ScorePoint.h"
//コンストラクタ
BallManager::BallManager(){
	state = BMNS::NOSTART;
	resetflag = false;
	targetBall = 0;
	AllScore = 0;
	LClicked = false;
	shootpower = 0;

}

BallManager::~BallManager(){



}
bool BallManager::initialize(){

	arrow = new SArrow();
	arrow->initialize();
	arrow->setPosition({ 0, 0 });
	arrow->setScale(1.0f);
	arrow->isVisible(true);

	sring = new SRing();
	sring->initialize();
	sring->setPosition({ 0, 0 });
	sring->setScale(0.2f);
	sring->isVisible(true);


	slogo = new SLogo();
	slogo->initialize();
	slogo->setPosition({ 0, 0 });
	slogo->setScale(1.0f);
	slogo->isVisible(true);

	INFO[BMNS::TUTORIAL::TARGET] = new SInfo();
	INFO[BMNS::TUTORIAL::TARGET]->initialize(ITARGET_IMAGE, 512, 39); //testureのサイズを第二第三引数へ
	INFO[BMNS::TUTORIAL::TARGET]->setPosition({ 0, 0 });
	INFO[BMNS::TUTORIAL::TARGET]->setScale(1.0f);
	INFO[BMNS::TUTORIAL::TARGET]->isVisible(false);


	INFO[BMNS::TUTORIAL::SHOOT] = new SInfo();
	INFO[BMNS::TUTORIAL::SHOOT]->initialize(ISHOOT_IMAGE,256, 39); //testureのサイズを第二第三引数へ
	INFO[BMNS::TUTORIAL::SHOOT]->setPosition({ 0, 0 });
	INFO[BMNS::TUTORIAL::SHOOT]->setScale(1.0f);
	INFO[BMNS::TUTORIAL::SHOOT]->isVisible(false);

	INFO[BMNS::TUTORIAL::SETUP] = new SInfo();
	INFO[BMNS::TUTORIAL::SETUP]->initialize(ISETUP_IMAGE, 512, 39); //testureのサイズを第二第三引数へ
	INFO[BMNS::TUTORIAL::SETUP]->setPosition({ 0, 0 });
	INFO[BMNS::TUTORIAL::SETUP]->setScale(1.0f);
	INFO[BMNS::TUTORIAL::SETUP]->isVisible(false);


	INFO[BMNS::TUTORIAL::SCORE] = new SInfo();
	INFO[BMNS::TUTORIAL::SCORE]->initialize(ISCORE_IMAGE, 256, 64); //testureのサイズを第二第三引数へ
	INFO[BMNS::TUTORIAL::SCORE]->setPosition({ 0, 0 });
	INFO[BMNS::TUTORIAL::SCORE]->setScale(1.0f);
	INFO[BMNS::TUTORIAL::SCORE]->isVisible(false);


	INFO[BMNS::TUTORIAL::GAMEOVER_LOGO] = new SInfo();
	INFO[BMNS::TUTORIAL::GAMEOVER_LOGO]->initialize(GAMEOVER_IMAGE, 556, 256); //testureのサイズを第二第三引数へ
	INFO[BMNS::TUTORIAL::GAMEOVER_LOGO]->setPosition({ (GAME_WIDTH - INFO[BMNS::TUTORIAL::GAMEOVER_LOGO]->getWidth())/2,
		(GAME_HEIGHT - INFO[BMNS::TUTORIAL::GAMEOVER_LOGO]->getHeight()) / 2 });
	INFO[BMNS::TUTORIAL::GAMEOVER_LOGO]->setScale(1.0f);
	INFO[BMNS::TUTORIAL::GAMEOVER_LOGO]->isVisible(false);


	score = new SScore();
	score->initialize();
	score->setPosition({ GAME_WIDTH/4, 5});
	score->setScale(1.0f);
	score->isVisible(true);
	score->setString(std::to_string(AllScore));
	return true;

}
void BallManager::update(){

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
	switch (state){
		case BMNS::NOSTART:
			break;
		case BMNS::START:
			if (StartUp()){
				INFO[BMNS::TUTORIAL::SCORE]->isVisible(true);
				state = BMNS::TARGETBALL;
			}

			break;
		case BMNS::TARGETBALL:
			if (TargetBall()){
				state = BMNS::TARGETBALL_END;

			}
			break;
		case BMNS::TARGETBALL_END:
			if (!LClicked){
				SetTutorial(BMNS::TUTORIAL::SHOOT);
				state = BMNS::SHOOTBALL;
				shootpower = 0;
			}
			break;
		case BMNS::SHOOTBALL:
			if (ShootBall()){
				state = BMNS::WAITSTOPBALL;

			}
			break;
		case BMNS::WAITSTOPBALL:
			if (waitBall()){
				if (!resetflag){
					SetTutorial(BMNS::TUTORIAL::TARGET);
					state = BMNS::TARGETBALL;
				}
				else{
					SetTutorial(BMNS::TUTORIAL::SETUP);
					state = BMNS::SETUPBALL;
				}

			}
			break;

		case BMNS::SETUPBALL:
			if (SetBall()){
				SetTutorial(BMNS::TUTORIAL::TARGET);
				state = BMNS::TARGETBALL;

			}
			else{
				arrow->isVisible(false);

			}
			break;

		case BMNS::GAMEOVER:
			if (GameOver()){

				INFO[BMNS::TUTORIAL::GAMEOVER_LOGO]->isVisible(false);
				state = BMNS::START;

				slogo->isVisible(true);
			}
			break;
		case BMNS::END:
				break;
		default:
				break;


	}
	if (state != BMNS::GAME_STATE::SETUPBALL)
		CheckBalls();

	if (state != BMNS::GAME_STATE::GAMEOVER)
		spriteRing();


	Input* i = GameSceneManager::Instance().GetGameptr()->getInput();
	if (i->getMouseLButton()){
		if (!LClicked)
			LClicked = true;

	}
	else{
		LClicked = false;
	}
}

void BallManager::GameStart(){
	state = BMNS::START;
	float r = 0.6f;//球の半径
	D3DXVECTOR3 pos[9];
	D3DXVECTOR3 origin = { -10.6f - 2.0f*sqrt(3.0f)*r, 0, 0 }; //初期のひし形中央に置く球の位置
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

	targetBall = 0;
	for (int i = 0; i < 9; i++){
		balls[i] = _prefab.PrefabCreate((PREFAB_MENU)(i + 5));
		gameObject* obj = gameObjectFactory::Instance().GetElem(balls[i]);
		obj->setPosition(pos[i] + origin); //上の値に中心値を足してセット
	}

	D3DXVECTOR3 mBallpos = { 10.6f, 0, 0 };//自球の位置今は適当

	_myball = _prefab.PrefabCreate((PREFAB_MENU)(0));
	gameObject* obj = gameObjectFactory::Instance().GetElem(_myball);
	obj->setPosition(mBallpos); //上の値に中心値を足してセット
	obj->SetName("MyBall");

	
}


void BallManager::GameReset(){

	AllScore = 0;
	state = BMNS::START;
	float r = 0.6f;//球の半径

	D3DXVECTOR3 pos[9];
	D3DXVECTOR3 origin = { -10.6f-2.0f*sqrt(3.0f)*r, 0, 0 }; //初期のひし形中央に置く球の位置
	//ひし形配置用のポジション値
	pos[0] = { 2.0f*sqrt(3.0f)*r, 0, 0 };	 //1
	pos[1] = { 0, 0, -2 * r };			//2
	pos[2] = { -2.0f*sqrt(3.0f)*r, 0, 0 };//3
	pos[3] = { 0, 0, 2 * r };//4
	pos[4] = { sqrt(3.0f)*r, 0, -r };
	pos[5] = { sqrt(3.0f)*r, 0, r };
	pos[6] = { -sqrt(3.0f)*r, 0, -r };
	pos[7] = { -sqrt(3.0f)*r, 0, r };
	pos[8] = { 0, 0, 0 };


	for (int i = 0; i < 9; i++){
		Ball* ball = (Ball*)gameObjectFactory::Instance().GetElem(balls[i]);
		ball->setPosition(pos[i] + origin);
		ball->setVisible(true);
		ball->resetBall();
		ball->setFall(false);
		ball->FallCheckReset();
		ball->SetTrigger(false);
	}
	D3DXVECTOR3 mBallpos = { 10.6f, 0, 0 };//自球の位置今は適当
	Ball* obj = (Ball*)gameObjectFactory::Instance().GetElem(_myball);
	obj->setPosition(mBallpos); //上の値に中心値を足してセット
	obj->setVisible(true);
	obj->resetBall();
	obj->setFall(false);
	obj->FallCheckReset();
	obj->SetTrigger(false);
	targetBall = 0;
	AllScore = 0;
	resetflag = false;
	score->setString(std::to_string(AllScore));
	score->setPosition({ GAME_WIDTH / 4, 5 });
	INFO[BMNS::TUTORIAL::SCORE]->setPosition({ 0, 0 });

}
//ボールを打ったかどうかの判定ボールを打ったならばtrueを返す
bool BallManager::ShootBall(){
	Input* in = GameSceneManager::Instance().GetGameptr()->getInput();

	if (in->getMouseLButton()){
		if (LClicked){
			float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();
			shootpower += frameTime;
			if (shootpower > SHOOTCYCLE){

				shootpower = 0.0f; //初期化
			}
			float pow = 0.5f;
			float Power = pow*cos(shootpower*2*PI)+pow;//最終的な力(sin変異)
			arrow->PowerColor(Power);
		}

	}
	else{
		if (LClicked){ //いままでクリックされていたなら
			arrow->isVisible(false);
			gameObject* ball = gameObjectFactory::Instance().GetElem(_myball);
			shootvec = { shootvec.x, 0, shootvec.z };
			float pow = 0.5f;
			float Power = pow*cos(shootpower * 2 * PI) + pow;//最終的な力(sin変異)
			ball->setDeltaV(shootvec*20.0f*(Power+0.2f));
			shootpower = 0;
			arrow->PowerColor(1.0f);
			return true;
		}


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
		arrow->isVisible(ret);
	return ret;

}
//ボールの狙いを定める
bool BallManager::TargetBall(){
	Input* in = GameSceneManager::Instance().GetGameptr()->getInput();
	gameObject* ball = gameObjectFactory::Instance().GetElem(_myball);
	D3DXVECTOR3 spos; //スクリーン上の位置
	GameSceneManager::Instance().GetGameptr()->getGraphics()->getScreenPosition(ball->getTransform()->getPosition(), spos);
	spos = { spos.x, 0, spos.y };
	
	D3DXVECTOR3 mPos = { (float)in->getMousePosX(), 0, (float)in->getMousePosY() };
	shootvec = mPos - spos;
	D3DXVec3Normalize(&shootvec, &shootvec);
	spriteArrow({spos.x,spos.z});
	if (in->getMouseLButton() && !LClicked){

		return true;

	}



	return false;
}
//ボールの状態チェック
void BallManager::CheckBalls(){
	//ボールが落ちているかどうかのチェック
	bool Complete = true;
	for (int i = 0; i < 9; i++){
		Ball* ball = (Ball*)gameObjectFactory::Instance().GetElem(balls[i]);
		if (ball->getisFall()&& !ball->getisFallcheck()){
			ball->FallCheck();
			ball->setVisible(false);
			ball->SetTrigger(true);
			if (i == targetBall){ //狙うべきボールだった場合
				D3DXVECTOR3 spos; //スクリーン上の位置
				D3DXVECTOR2 screenpos = { 0,0 };
				GameSceneManager::Instance().GetGameptr()->getGraphics()->getScreenPosition(ball->getTransform()->getPosition(), spos);
				screenpos = { spos.x, spos.y };
				int Point = (i+1) * 100;
				std::string s = "+" + std::to_string(Point);
				ScoreEffect(screenpos, s,true);
				AllScore += Point;
				score->setString(std::to_string(AllScore));
			}
			else{
				D3DXVECTOR3 spos; //スクリーン上の位置
				D3DXVECTOR2 screenpos = { 0, 0 };
				GameSceneManager::Instance().GetGameptr()->getGraphics()->getScreenPosition(ball->getTransform()->getPosition(), spos);
				screenpos = { spos.x, spos.y };
				ScoreEffect(screenpos, "-100",false);
				AllScore -= 100;
				score->setString(std::to_string(AllScore));

			}
		}
		Complete &= ball->getisFallcheck(); //全部落ちているか確認


	}
	//自分のボールのチェック
	Ball* ball = (Ball*)gameObjectFactory::Instance().GetElem(_myball);
	if (ball->getisFall() && !ball->getisFallcheck()){
		ball->FallCheck();
		ball->setVisible(false);
		D3DXVECTOR3 spos; //スクリーン上の位置
		D3DXVECTOR2 screenpos = { 0, 0 };
		GameSceneManager::Instance().GetGameptr()->getGraphics()->getScreenPosition(ball->getTransform()->getPosition(), spos);
		screenpos = { spos.x, spos.y };
		std::string s = "-100";
		ScoreEffect(screenpos, s,false);
		AllScore -= 100;
		score->setString(std::to_string(AllScore));
		resetflag = true;
		arrow->isVisible(false);
	}
	if (Complete){
		state = BMNS::GAME_STATE::GAMEOVER;
		INFO[BMNS::TUTORIAL::SCORE]->isVisible(true);
		INFO[BMNS::TUTORIAL::GAMEOVER_LOGO]->isVisible(true);
		score->setPosition({ GAME_WIDTH / 4+(GAME_WIDTH-score->getWidth())/2, 5 + 70 });
		INFO[BMNS::TUTORIAL::SCORE]->setPosition({ (GAME_WIDTH - score->getWidth()) / 2, +70 });
	}

	//TargetBallを設定

	for (int i = 0; i < 9; i++){
		Ball* ball = (Ball*)gameObjectFactory::Instance().GetElem(balls[i]);
		if (ball->getisFallcheck()){
		}
		else{
			targetBall = i; //落ちてないものの最初の番号をとる
			i = 100;
		}

	}


}
void BallManager::spriteArrow(D3DXVECTOR2 spos){
	float a = -shootvec.x;
	float b = -shootvec.z;
	float atNUM = b / sqrt((pow(a, 2) + pow(b, 2))) + a;
	float theta = 2*atanf(atNUM); //方向から角度を出す
	theta = ACOS(a, b);
	D3DXVECTOR2 offset = { arrow->getWidth() / 2.0f, arrow->getHeight() / 2.0f };
	arrow->setPosition(spos - offset);
	arrow->setAngle(theta);
}
//リングの表示用
void BallManager::spriteRing(){

	for (int i = 0; i < 9; i++){
		Ball* ball = (Ball*)gameObjectFactory::Instance().GetElem(balls[i]);
		if (ball->getisFall() && !ball->getisFallcheck()){

		}
		else{

			if (targetBall == i){

				D3DXVECTOR3 spos; //スクリーン上の位置
				D3DXVECTOR2 screenpos = { 0, 0 };
				GameSceneManager::Instance().GetGameptr()->getGraphics()->getScreenPosition(ball->getTransform()->getPosition(), spos);
				screenpos = { spos.x - (SRingNS::WIDTH / 2)*sring->getScale(), spos.y - (SRingNS::HEIGHT / 2)*sring->getScale() };
				sring->setPosition(screenpos);
			}
		}

	}

}
//Scoreのエフェクト文字を指定座標に出す
void BallManager::ScoreEffect(D3DXVECTOR2 spos,std::string str,bool up){
	ScorePoint* s = new ScorePoint();
	s->setUp(up);
	s->setPosition(spos);
	s->setString(str);
	s->initialize();
}

float BallManager::ACOS(float x, float y){
	float radius = 1.0f;
	float theta = 0;
	if (SIGN(y) < 0){
		theta = -acos(x / radius);
	}
	else{
		theta = acos(x / radius);
	}
	return theta;

}
//白球のセットしなおし配置したらtrue;
bool BallManager::SetBall(){
	Input* in = GameSceneManager::Instance().GetGameptr()->getInput();
	Graphics* g = GameSceneManager::Instance().GetGameptr()->getGraphics();
	
	D3DXVECTOR3 mPos = { (float)in->getMousePosX(), (float)in->getMousePosY(), 0 };
	D3DXVECTOR3 spos = { (-mPos.x*2/GAME_WIDTH) *18.5f +18.5f , 0, (-mPos.y*2/GAME_HEIGHT) * 10.0f +10.0f };
	
	Ball* ball = (Ball*)gameObjectFactory::Instance().GetElem(_myball);
	if (ball->getisFallcheck()){
		ball->setVisible(true);
		ball->FallCheckReset();
		ball->SetTrigger(true);
	}
	ball->setPosition(spos);
	if (in->getMouseLButton()){
		if (!LClicked){
			ball->setFall(false);
			ball->SetTrigger(false);
			ball->resetBall();
			arrow->isVisible(true);
			resetflag = false;
			return true;
		}

	}
	return false;

}

bool BallManager::StartUp(){

	Input* in = GameSceneManager::Instance().GetGameptr()->getInput();
	arrow->isVisible(false);
	sring->isVisible(false);
	score->isVisible(false);
	slogo->isVisible(true);
	slogo->setPosition({ GAME_WIDTH / 2 - slogo->getWidth() / 2, GAME_HEIGHT / 2 - slogo->getHeight() / 2 });
	if (in->getMouseLButton()){
		if (!LClicked){
			arrow->isVisible(true);
			sring->isVisible(true);
			score->isVisible(true);
			slogo->isVisible(false);
			return true;
		}

	}
	return false;
}

void BallManager::SetTutorial(BMNS::TUTORIAL t){
	INFO[t]->setPosition({ (GAME_WIDTH - INFO[t]->getWidth()) / 2, GAME_HEIGHT*0.85f });
	for (int i = 0; i < BMNS::TUTORIAL::SCORE; i++){
		if ((int)t == i)
			INFO[i]->setStart();
		else
			INFO[i]->Unset();

	}

}
bool BallManager::GameOver(){
	Input* in = GameSceneManager::Instance().GetGameptr()->getInput();
	if (in->getMouseLButton()){
		if (!LClicked){
			GameReset();
			return true;
		}

	}
	return false;
}