#ifndef _GAME_H
#define _GAME_H
#define Win32_LEAN_AND_MEAN
//�Q�[���V�[���̂��ƂƂȂ�N���X2
#include <windows.h>
#include <string>
#include <vector>
#include <Mmsystem.h>
#include "constants.h"
#include "input.h"
#include "gameError.h"
//#include "audio.h"
#include "textdx.h"
#include "console.h"
#include "CreateConsole.h"
#include "graphics.h"
namespace gameNS
{
	const char FONT[] = "Courier New";
	const int POINT_SIZE = 14;
	const COLOR_ARGB FONT_COLOR = SETCOLOR_ARGB(255, 255, 255, 255);
	const float SLOWMOTION = 0.5f;
	const float NORMALMOTION = 1.0f;
}
class ObjectDataManager;
class Game
{
protected:
	Graphics *graphics;
	Input *input;
	//Audio *audio;
	HWND hwnd; //�E�C���h�E�̃n���h��
	HRESULT hr;
	LARGE_INTEGER timeStart; //�p�t�H�[�}���X�J�E���^�[�̊J�n�l
	LARGE_INTEGER timeEnd; //�p�t�H�[�}���X�J�E���^�[�̏I���l
	LARGE_INTEGER timerFreq; //�p�t�H�[�}���X�J�E���^�[�̎��g��
	Console* console;               // pointer to Console
	CreateConsole* c_console;
	float frameTime;	//�Ō�̃t���[���ɗv��������
	float fps; //�t���[�����[�g
	TextDX dxFont;
	bool fpsOn;
	DWORD sleepTime;	//�t���[���ԂŃX���[�v���鎞��(�~���b)
	bool paused;	//�Q�[�����ꎞ��~����Ă���ꍇ�Atrue
	bool initialized;
	std::string  command;           // command from console
	ObjectDataManager* ODM;
	bool Slowmotion; //�R�}���胂�[�h���J�n����
	bool Trigger; //�R�}���菈����i�߂�
	bool preinput;
public:
	//�R���X�g���N�^
	Game();

	//�f�X�g���N�^	
	virtual ~Game();

	//�����o�[�֐�

	//Windows���b�Z�[�W�n���h��
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//�Q�[����������
	//���s�Ohwnd�̓E�C���h�E�ւ̃n���h��
	virtual void initialize(HWND hw);

	//WinMain���̃��C���̃��b�Z�[�W���[�v��run���J��Ԃ��Ăяo��
	virtual void run(HWND hwnd);

	//�O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
	//�O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
	//�\�񂳂�Ă����r�f�I�����������ׂĉ��
	virtual void releaseAll();

	//���ׂẴT�[�t�F�C�X���č쐬���A���ׂẴG���e�B�e�B�����Z�b�g
	virtual void resetAll();

	//�\�񂳂�Ă��������������ׂč폜
	virtual void deleteAll();

	//�Q�[���A�C�e���������_�[
	virtual void renderGame();

	//���������O���t�B�b�N�f�o�C�X������
	virtual void handleLostGraphicsDevice();

	virtual void consoleCommand();

	// Display���[�h�̕ύX
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	//Graphics�ւ̃|�C���^��߂�
	Graphics* getGraphics(){ return graphics; }

	//Input�ւ̃|�C���^��Ԃ�
	Input* getInput(){ return input; }

	//Audio�ւ̃|�C���^��Ԃ�
//	Audio* getAudio(){ return audio; }

	//�Q�[�����I��
	void exitGame(){ PostMessage(hwnd, WM_DESTROY, 0, 0); }

	//�������z�֐��̐錾
	//�ȉ��̊֐��ł��ꂼ��̃Q�[���V�[���ł̏������s��

	//�Q�[���A�C�e�����X�V
	virtual void update() = 0;
	//AI�v�Z�����s
	virtual void ai() = 0;
	//�Փ˂��`�F�b�N
	virtual void collisions() = 0;
	//�O���t�B�b�N�X�������_�[
	//graphics->spriteBegin();���Ăяo���B
	//�X�v���C�g��`��
	//graphics->spriteEnd();���Ăяo��
	//�X�v���C�g�ȊO��`��
	virtual void render() = 0;

	float getframeTime(){ return frameTime; }
	bool getpause(){ return paused; }
	ObjectDataManager* getODM(){ return ODM; }

	void SaveODM();
	virtual void ResetGame() = 0;
};
#include "ObjectDataManager.h"
#endif