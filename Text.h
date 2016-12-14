#ifndef TEXT_H
#define TEXT_H
#define WIN32_LEAN_AND_MEAN
#include <string>
#include "Sprite.h"
#include "constants.h"
namespace textNS{
	enum Alignment { LEFT, CENTER, RIGHT, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM };
	struct FontData
	{
		UINT left;
		UINT right;
	};
	const int FONT_BORDER =1; //各文字の周囲の枠幅のサイズ,透明な枠2ピクセル分グリッドの枠線分で3ピクセル
	const int FONT_WIDTH = 31; //文字幅
	const int FONT_HEIGHT = 50;// 文字の高さ
	const int GRID_WIDTH = FONT_WIDTH + FONT_BORDER; //文字一つ分のグリッドの横幅
	const int GRID_HEIGHT = FONT_HEIGHT + FONT_BORDER; //文字一つ分のグリッドの高さ幅
	const int COLUMNS = 4;	//フォント画像内の列数
	const int ROWS = 5; //フォント画像内の行数
	const int FRAMES = 1; //各文字のアニメーションのフレーム数(1 = アニメーションなし)
	const double ANIM_DELAY = 0.0; //アニメーションフレーム間の時間間隔
	const int MAX_FONT_HEIGHT = 1000; //フォントの最大の高さ
	const int MIN_CHAR = 0x0029; //左上隅に位置する文字の文字コード。(' ')ここからMAX_CHARまで
	const int MAX_CHAR = 0x0039; //左右隅に位置する文字の文字コード
	const int PROPORTIONAL_SPACING = 5; //プロポーショナルスペースで表示する文字間のスペースの広さ
	const int TAB_SIZE = 8; //文字位置によってあらわされるタブ位置のサイズ
	const char UNDERLINE = '_';	//下線を表示する際に使われる文字ASCII値は0x5F(これはテクスチャのグリッドと対応)
	const char SOLID = 0x7F; //単色で塗りつぶしたブロックを表示するために使用される文字0x7Fは左上隅(MIN_CHAR)からの文字の個数に足したもの
	const int BOLD_SIZE = 4; //文字を横にずらして表示
}
class Text : public Sprite{
private:
	TextureManager fontTexture;
	LP_TEXTURE textureData;
	char *file;
	Graphics *graphics;
	textNS::FontData fontData[textNS::ROWS][textNS::COLUMNS];
	textNS::Alignment align;
	UINT width, height;
	UINT fontHeight;
	UINT tabSize;
	COLOR_ARGB color;
	COLOR_ARGB backColor;
	UINT proportionalSpacing;
	bool proportional;

public:
	Text();
	virtual ~Text();
	virtual void setXY(int x2, int y2);
	virtual void print(const std::string &str);
	virtual void print(const std::string &str, int x, int y);
	virtual void print(const std::string & str, int x, int y, textNS::Alignment align);
	virtual bool initialize(Graphics *g);
	virtual void doAlign(const std::string &str);
	virtual void drawChar(UCHAR ch);
	virtual void onResetDevice();
	virtual void onLostDevice();
	//Set Function
	virtual void setFontHeight(UINT height)
	{
		if (height == 0 || height > textNS::MAX_FONT_HEIGHT)
			return;
		fontHeight = height;
		spriteData.scale = (float)height / (float)textNS::FONT_HEIGHT;
	}
	virtual void setFontColor(COLOR_ARGB c){ color = c; }
	virtual void setBackColor(COLOR_ARGB bc){ backColor = bc; }
	virtual void setProportional(bool p){ proportional = p; }
	virtual void setProportionalSpacing(UINT s){ proportionalSpacing = s; }
	virtual void setTabSize(UINT size)
	{
		if (size == 0)
			return;
		tabSize = size;
	}

	//getFunction
	virtual UINT getFontHeight(){ return height; }
	virtual COLOR_ARGB getFontColor(){ return color; }
	virtual COLOR_ARGB getBackColor(){ return backColor; }
	virtual void getWidthHeight(const std::string &str, UINT &w, UINT &h);
	virtual bool getProportional(){ return proportional; }
	virtual UINT getProportionalSpacing(){ return proportionalSpacing; }
	virtual UINT getTabSize(){ return tabSize; }

};

#endif