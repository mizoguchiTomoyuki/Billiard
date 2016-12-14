#ifndef _BALLMANAGER_H
#define _BALLMANAGER_H
#include "PrefabManager.h"
#include "SArrow.h"
#include "SScore.h"
//�{�[���̊Ǘ����s���N���X�{�[���̐����A��ʂ̊Ǘ��Ȃǂ��s��
//�Q�[���̃��[��������S��
namespace BMNS{
	enum GAME_STATE{ //��Ԃ�\��
		NOSTART = 0, //�Q�[���J�n�O
		START = 1,	//�J�n���(n�b��Ɏ��̃X�e�[�g��)
		TARGETBALL = 2, //�_�����߂���
		SHOOTBALL = 3, //�{�[����ł��
		WAITSTOPBALL = 4, //�{�[���̃V���[�g���m�肵�ă{�[�����~�܂�̂�҂��
		SETUPBALL = 5, //�����̍ăZ�b�g�A�b�v
		GAMEOVER = 6, //�Q�[���N���A
		END

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
public:
	BallManager();
	~BallManager();
	void update();
	void GameStart(); //��ʂ̔z�u
	bool ShootBall();//�����̎�ʂ�ł���
	bool waitBall();
	bool TargetBall();
	bool SetBall();
	void spriteArrow(D3DXVECTOR2 spos);
	bool initialize();
	float ACOS(float x, float y);

};

#endif