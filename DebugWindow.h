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
	const COLOR_ARGB BACK_COLOR_NA = SETCOLOR_ARGB(192,50, 50, 50);//非アクティブ時のカラー
	const int MAX_LINE = 256;

}
//四角い窓を描画する
//printで窓の必要な位置に必要な文字列を入力できる
class DebugWindow{
protected:
	Graphics *graphics;
	Input *input;
	TextDX dxFont;
	float x, y; //コンソールの描画位置
	int rows; //コンソールに適合するようなテキストの列の数
	bool visible;
	std::string inputStr; //consoleにインプットされたテキスト
	std::string commandStr; //console command;
	std::deque<std::string> text;//コンソールテキストの格納デキュー
	RECT textRect; //textRectangle
	COLOR_ARGB fontColor;
	COLOR_ARGB backColor;
	COLOR_ARGB backColor_NonActive;
	int rowHeight;
	CUSTOMVERTEX vtx[4]; //背景の頂点データ
	LP_VERTEXBUFFER vertexBuffer;//頂点データを保持しているバッファ
	int scrollAmount; //Display上でどのくらいスクリールしたか
	bool initialized;
	int MaxLine;
	bool Active; //どのウインドウがアクティブか
	bool Activate;
public:
	DebugWindow();
	virtual ~DebugWindow();
	//コンソールの描画
	virtual const void draw();
	virtual const void Appenddraw(){};
	//初期化
	virtual bool initialize(Graphics *g, Input *in);
	//コンソールの表示非表示
	void showHide();
	//表示非表示の設定を返す
	bool getVisible(){ return visible; }
	//コンソールの表示を行います
	void show(){ visible = true; }
	//コンソールを非表示に
	void hide(){ visible = false; }

	virtual void print(const std::string & str);

	virtual void onLostDevice();
	virtual void onResetDevice();
	virtual void setActive(bool b){
		Active = b; 
		Activate = true;//設定変更を知らせる
	}
	virtual bool GetActive(){ return Active; }
	virtual void SetActiveColor();
	virtual int Decode(std::string s, int y);

};

#endif