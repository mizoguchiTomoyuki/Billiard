#ifndef _BALLMANAGER_H
#define _BALLMANAGER_H
#include "PrefabManager.h"
#include "SArrow.h"
#include "SRing.h"
#include "SScore.h"
#include "SLogo.h"
#include "SINFO.h"
#define SHOOTCYCLE 1.0f //1秒間隔でパワーを変更
//ボールの管理を行うクラスボールの生成、手玉の管理などを行う
//ゲームのルール部分を担当
namespace BMNS{
	enum GAME_STATE{ //状態を表す
		NOSTART = 0, //ゲーム開始前
		START = 1,	//開始状態(n秒後に次のステートへ)
		TARGETBALL = 2, //狙いを定める状態
		TARGETBALL_END = 3, //狙いを定める状態
		SHOOTBALL = 4, //ボールを打つ状態
		WAITSTOPBALL = 5, //ボールのシュートが確定してボールが止まるのを待つ状態
		SETUPBALL = 6, //自球の再セットアップ
		GAMEOVER = 7, //ゲームクリア
		END

	};
	enum TUTORIAL{ //状態を表す
		TARGET = 0,
		SHOOT = 1,
		SETUP =2,
		SCORE =3,
		GAMEOVER_LOGO = 4,
		TEND

	};


}
class BallManager{
protected:
	BMNS::GAME_STATE state;
	ELEMID balls[9]; //九つの落とすべきボール
	ELEMID _myball; //自分の持ち球
	PrefabManager _prefab; //ボールの生成などを行わせる
	D3DXVECTOR3 shootvec;//打つ方向
	SArrow* arrow;
	SScore* score;
	SRing* sring;
	SLogo* slogo;
	SInfo* INFO[BMNS::TUTORIAL::TEND];
	bool resetflag;
	int targetBall;
	int AllScore;
	bool LClicked;
	float shootpower;
public:
	BallManager();
	~BallManager();
	void update();
	void GameStart(); //手玉の配置
	void GameReset(); //手玉の配置
	bool ShootBall();//自分の手玉を打つ処理
	bool waitBall();
	bool TargetBall();
	bool SetBall();
	void CheckBalls();
	void ScoreEffect(D3DXVECTOR2 spos, std::string str,bool up);
	void spriteArrow(D3DXVECTOR2 spos);
	void spriteRing();
	bool initialize();
	float ACOS(float x, float y);
	bool StartUp();
	void SetTutorial(BMNS::TUTORIAL t);
	bool GameOver();
};

#endif