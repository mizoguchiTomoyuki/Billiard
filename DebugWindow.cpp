#include "DebugWindow.h"
//======================================================================================
//コンストラクタ
//======================================================================================
DebugWindow::DebugWindow()
{
	initialized = false;
	graphics = NULL;
	visible = false;
	fontColor = DebugWindowNS::FONT_COLOR;
	backColor = DebugWindowNS::BACK_COLOR;
	backColor_NonActive = DebugWindowNS::BACK_COLOR_NA;
	x = DebugWindowNS::X;
	y = DebugWindowNS::Y;
	textRect.bottom = DebugWindowNS::Y + DebugWindowNS::HEIGHT - DebugWindowNS::MARGIN;
	textRect.left = DebugWindowNS::X + DebugWindowNS::MARGIN;
	textRect.right = DebugWindowNS::X + DebugWindowNS::WIDTH - DebugWindowNS::MARGIN;
	textRect.top = DebugWindowNS::Y + DebugWindowNS::MARGIN;
	vertexBuffer = NULL;
	rows = 0;
	scrollAmount = 0;
	MaxLine = DebugWindowNS::MAX_LINE; 
	dxFont.print("|", textRect, DT_CALCRECT);//「|」を全高を扱う文字として使用
	rowHeight = textRect.bottom + 2; //一行の高さ(+2は行間スペース)
	Activate = false;
}

//=============================================================================
// Destructor
//=============================================================================
DebugWindow::~DebugWindow()
{
	onLostDevice();            // call onLostDevice() for every graphics item
}

//======================================================================================
//コンソールの初期化
//======================================================================================
bool DebugWindow::initialize(Graphics *g, Input *in)
{
	try{
		graphics = g;  //グラフィックデバイス
		input = in;
		//左上
		vtx[0].x = x;
		vtx[0].y = y;
		vtx[0].z = 0.0f;
		vtx[0].rhw = 1.0f;
		vtx[0].dwColor = backColor;
		//右上
		vtx[1].x = x + DebugWindowNS::WIDTH;
		vtx[1].y = y;
		vtx[1].z = 0.0f;
		vtx[1].rhw = 1.0f;
		vtx[1].dwColor = backColor;
		//右下
		vtx[2].x = x + DebugWindowNS::WIDTH;
		vtx[2].y = y + DebugWindowNS::HEIGHT;
		vtx[2].z = 0.0f;
		vtx[2].rhw = 1.0f;
		vtx[2].dwColor = backColor;
		//左下
		vtx[3].x = x;
		vtx[3].y = y + DebugWindowNS::HEIGHT;
		vtx[3].z = 0.0f;
		vtx[3].rhw = 1.0f;
		vtx[3].dwColor = backColor;
		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
		//DirectXフォントを初期化
		if (dxFont.initialize(graphics, DebugWindowNS::FONT_HEIGHT, false, false, DebugWindowNS::FONT) == false)
			return false;
		dxFont.setFontColor(fontColor);

	}
	catch (...){
		return false;
	}
	initialized = true;
	return true;
}



//======================================================================================
//コンソールを描画
//実行前：BeginSceneとEndSceneの内側にする
//======================================================================================
const void DebugWindow::draw()
{
	//基本的な描画
	if (!visible || graphics == NULL || !initialized)
		return;

	if (Activate = true){
		SetActiveColor();
		Activate = false;
	}

	graphics->spriteBegin();
	graphics->drawQuad(vertexBuffer); //背景を描画
	if (text.size() == 0)
		return;

	//コンソールにテキストを表示
	textRect.left = 0;
	textRect.top = 0;
	//textRectの下端を一行の高さに
	dxFont.print("|", textRect, DT_CALCRECT);//「|」を全高を扱う文字として使用
	rowHeight = textRect.bottom + 2; //一行の高さ(+2は行間スペース)
	if (rowHeight <= 0)	//エラーよけ
		rowHeight = 20;

	rows = (DebugWindowNS::HEIGHT - 2 * DebugWindowNS::MARGIN) / rowHeight;
	rows -= 2; //下部に、入力プロンプト用のスペースを空ける
	if (rows <= 0) //これはtrueにならないと想定される
		rows = 5;	//機能する値を強制的に設定

	//一行のテキスト表示rectを設定
	textRect.left = (long)(x + DebugWindowNS::MARGIN);
	textRect.right = (long)(textRect.right + DebugWindowNS::WIDTH - DebugWindowNS::MARGIN);
	textRect.bottom = (long)(y + DebugWindowNS::HEIGHT - 2 * DebugWindowNS::MARGIN - 2 * rowHeight);
	for (int r = scrollAmount; r < rows + scrollAmount && r < (int)(text.size()); r++)
	{
		//この行のテキスト表示rectの上端を設定
		textRect.top = textRect.bottom - rowHeight;
		//テキストの一行を表示
		dxFont.print(text[r], textRect, DT_LEFT);
		//テキスト表示rectの下端を次の行用に調整
		textRect.bottom -= rowHeight;

	}
	//派生クラスで描きたい描画
	Appenddraw();
	graphics->spriteEnd();	//スプライトの描画を終了
}

//======================================================================================
//コンソールの表示と非表示
//======================================================================================
void DebugWindow::showHide()
{
	if (!initialized)
		return;
	visible = !visible;
	input->clear(inputNS::KEYS_PRESSED | inputNS::TEXT_IN); //古い入力を消去
}

//======================================================================================
//コンソールにテキストを追加
//strの先頭行のテキストのみ表示される
//======================================================================================
void DebugWindow::print(const std::string &str) //コンソールにテキストを追加
{
	if (!initialized)
		return;
	text.push_front(str); //strを、デックtextに追加
	if (text.size() > DebugWindowNS::MAX_LINE)
		text.pop_back(); //最も古い行を削除
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void DebugWindow::onLostDevice()
{
	if (!initialized)
		return;
	dxFont.onLostDevice();
	SAFE_RELEASE(vertexBuffer);
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void DebugWindow::onResetDevice()
{
	if (!initialized)
		return;
	graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
	dxFont.onResetDevice();
}

//=============================================================================
//背景の四角形の色を変更してActive非Activeを示す
//=============================================================================
void DebugWindow::SetActiveColor(){
	COLOR_ARGB c = backColor;
	if (!Active)
		c = backColor_NonActive;

	vtx[0].dwColor = c;
	vtx[1].dwColor = c;
	vtx[2].dwColor = c;
	vtx[3].dwColor = c;
	graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);

}

//ある規約のstringを分解してコンソールに表示する
//とりあえず";"で改行する.
//yは描画開始位置
//最後に行数を返す
int DebugWindow::Decode(std::string s, int y){
	int rowNum = 0;
	std::string substr = "";
	int index = 0;
	int next_index = s.find(";", index);

	while (next_index != std::string::npos){
		rowNum++;

		textRect.bottom = (long)((y + rowNum)*rowHeight + y + rowHeight);
		textRect.top = textRect.bottom - rowHeight;
		substr = s.substr(index, next_index-index); //CreateMenuに向かうMenu
		dxFont.print(substr, textRect, DT_LEFT);
		index = next_index+1;
		next_index = s.find(";", index);

	}
	return rowNum;
}