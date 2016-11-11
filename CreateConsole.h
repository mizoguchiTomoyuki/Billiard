#ifndef _CREATE_CONSOLE_H
#define _CREATE_CONSOLE_H
#include "DebugWindow.h"
#include "gameObjectFactory.h"
class MouseExtend;
namespace CConsoleNS{
	const UINT WIDTH = 300;
	const UINT HEIGHT = 400;
	const UINT X = 320, Y = 5;
	const UINT MARGIN = 4;
	const char FONT[] = "Courier New";
	const int FONT_HEIGHT = 14;
	const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;
	const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192, 128, 128, 128);
	const int MAX_LINE = 256;
	const enum CREATEMODE{
		SELECTMENU = 0,
		CREATEMENU = 1,
		EDIT = 2,
	};

};
class CreateConsole : public DebugWindow{
private:
	int selectRowY;
	int selectRow;
	int MaxSelectRow;
	int MinSelectRow;
	CConsoleNS::CREATEMODE cmode;
	int exitRow;
	float Arrow_Key;
	ELEMID selectObj;
	std::map < int, ELEMID > Objmap;
	std::vector<IndexString> st;
	float StartValue;//Drag開始前の値をとっておく

public:
	CreateConsole();
	virtual ~CreateConsole();

	virtual const void draw();
	virtual const void Appenddraw();
	virtual void SelectMenu();
	virtual void CreateMenu();
	virtual void EditMenu();
	virtual bool initialize(Graphics *g, Input *in);
	std::string getCommand();
	std::string getInput(){ return  inputStr; }
	void clearInput(){ inputStr = ""; }

	// Call when graphics device is lost.
	virtual void onLostDevice();

	// Call when graphics device is reset.
	virtual void onResetDevice();

	void CommandList();
	std::string DesideMenu(std::string s);
	std::string PrefabName(PREFAB_MENU p);
	void CreatePrefab(PREFAB_MENU p, std::string n);
	void EditNumber_fromKey();
	void EditNumber_fromMouse();
};
#endif