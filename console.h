#ifndef CONSOLE_H
#define CONSOLE_H
#define WIN32_LEAN_AND_MEAN
#include "DebugWindow.h"

//Console��\�����R�}���h���͂�fps�\���Ȃǂ��s����悤�ɂ��邽�߂̃N���X
//�O���t�B�b�N�X��p���ĕ\�����s���̂�Graphics�N���X������
//�܂����͂ɑΉ����邽�߂�INPUT������
//������DirectXFONT�ŕ`����s��
namespace consoleNS{
	const UINT WIDTH = 300;
	const UINT HEIGHT = 400;
	const UINT X = 5, Y = 5;
	const UINT MARGIN = 4;
	const char FONT[] = "Courier New";
	const int FONT_HEIGHT = 14;
	const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;
	const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192, 128, 128, 128);
	const int MAX_LINE = 256;

}
class Console : public DebugWindow
{
private:
public:
	Console();
	virtual ~Console();

	virtual const void draw();
	virtual const void Appenddraw();
	virtual bool initialize(Graphics *g, Input *in);
	std::string getCommand();
	std::string getInput(){ return  inputStr; }
	void clearInput(){ inputStr = ""; }

	// Call when graphics device is lost.
	virtual void onLostDevice();

	// Call when graphics device is reset.
	virtual void onResetDevice();

	void CommandList();

};

#endif