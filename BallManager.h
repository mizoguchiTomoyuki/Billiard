#ifndef _BALLMANAGER_H
#define _BALLMANAGER_H
#include "PrefabManager.h"
#include "SArrow.h"
#include "SRing.h"
#include "SScore.h"
#include "SLogo.h"
#include "SINFO.h"
#define SHOOTCYCLE 1.0f //1�b�Ԋu�Ńp���[��ύX
//�{�[���̊Ǘ����s���N���X�{�[���̐����A��ʂ̊Ǘ��Ȃǂ��s��
//�Q�[���̃��[��������S��
namespace BMNS{
	enum GAME_STATE{ //��Ԃ�\��
		NOSTART = 0, //�Q�[���J�n�O
		START = 1,	//�J�n���(n�b��Ɏ��̃X�e�[�g��)
		TARGETBALL = 2, //�_�����߂���
		TARGETBALL_END = 3, //�_�����߂���
		SHOOTBALL = 4, //�{�[����ł��
		WAITSTOPBALL = 5, //�{�[���̃V���[�g���m�肵�ă{�[�����~�܂�̂�҂��
		SETUPBALL = 6, //�����̍ăZ�b�g�A�b�v
		GAMEOVER = 7, //�Q�[���N���A
		END

	};
	enum TUTORIAL{ //��Ԃ�\��
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
	ELEMID balls[9]; //��̗��Ƃ��ׂ��{�[��
	ELEMID _myball; //�����̎�����
	PrefabManager _prefab; //�{�[���̐����Ȃǂ��s�킹��
	D3DXVECTOR3 shootvec;//�ł���
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
	void GameStart(); //��ʂ̔z�u
	void GameReset(); //��ʂ̔z�u
	bool ShootBall();//�����̎�ʂ�ł���
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