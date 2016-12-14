#ifndef _BALLMANAGER_H
#define _BALLMANAGER_H
#include "PrefabManager.h"
#include "SArrow.h"
#include "SScore.h"
//ボールの管理を行うクラスボールの生成、手玉の管理などを行う
//ゲームのルール部分を担当
namespace BMNS{
	enum GAME_STATE{ //状態を表す
		NOSTART = 0, //ゲーム開始前
		START = 1,	//開始状態(n秒後に次のステートへ)
		TARGETBALL = 2, //狙いを定める状態
		SHOOTBALL = 3, //ボールを打つ状態
		WAITSTOPBALL = 4, //ボールのシュートが確定してボールが止まるのを待つ状態
		SETUPBALL = 5, //自球の再セットアップ
		GAMEOVER = 6, //ゲームクリア
		END

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
public:
	BallManager();
	~BallManager();
	void update();
	void GameStart(); //手玉の配置
	bool ShootBall();//自分の手玉を打つ処理
	bool waitBall();
	bool TargetBall();
	bool SetBall();
	void spriteArrow(D3DXVECTOR2 spos);
	bool initialize();
	float ACOS(float x, float y);

};

#endif