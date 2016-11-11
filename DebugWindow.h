#ifndef _DEBUG_WINDOW
#define _DEBUG_WINDOW
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <deque>
#include "constants.h"
#include "graphics.h"
#include "input.h"
#include "textdx.h"
namespace DebugWindowNS{
	const UINT WIDTH = 500;
	const UINT HEIGHT = 400;
	const UINT X = 5, Y = 5;
	const UINT MARGIN = 4;
	const char FONT[] = "Courier New";
	const int FONT_HEIGHT = 14;
	const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;
	const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192, 188, 128, 128);
	const COLOR_ARGB BACK_COLOR_NA = SETCOLOR_ARGB(192,50, 50, 50);//��A�N�e�B�u���̃J���[
	const int MAX_LINE = 256;

}
//�l�p������`�悷��
//print�ő��̕K�v�Ȉʒu�ɕK�v�ȕ��������͂ł���
class DebugWindow{
protected:
	Graphics *graphics;
	Input *input;
	TextDX dxFont;
	float x, y; //�R���\�[���̕`��ʒu
	int rows; //�R���\�[���ɓK������悤�ȃe�L�X�g�̗�̐�
	bool visible;
	std::string inputStr; //console�ɃC���v�b�g���ꂽ�e�L�X�g
	std::string commandStr; //console command;
	std::deque<std::string> text;//�R���\�[���e�L�X�g�̊i�[�f�L���[
	RECT textRect; //textRectangle
	COLOR_ARGB fontColor;
	COLOR_ARGB backColor;
	COLOR_ARGB backColor_NonActive;
	int rowHeight;
	CUSTOMVERTEX vtx[4]; //�w�i�̒��_�f�[�^
	LP_VERTEXBUFFER vertexBuffer;//���_�f�[�^��ێ����Ă���o�b�t�@
	int scrollAmount; //Display��łǂ̂��炢�X�N���[��������
	bool initialized;
	int MaxLine;
	bool Active; //�ǂ̃E�C���h�E���A�N�e�B�u��
	bool Activate;
public:
	DebugWindow();
	virtual ~DebugWindow();
	//�R���\�[���̕`��
	virtual const void draw();
	virtual const void Appenddraw(){};
	//������
	virtual bool initialize(Graphics *g, Input *in);
	//�R���\�[���̕\����\��
	void showHide();
	//�\����\���̐ݒ��Ԃ�
	bool getVisible(){ return visible; }
	//�R���\�[���̕\�����s���܂�
	void show(){ visible = true; }
	//�R���\�[�����\����
	void hide(){ visible = false; }

	virtual void print(const std::string & str);

	virtual void onLostDevice();
	virtual void onResetDevice();
	virtual void setActive(bool b){
		Active = b; 
		Activate = true;//�ݒ�ύX��m�点��
	}
	virtual bool GetActive(){ return Active; }
	virtual void SetActiveColor();
	virtual int Decode(std::string s, int y);

};

#endif