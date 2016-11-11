#include "console.h"
//======================================================================================
//コンストラクタ
//======================================================================================
Console::Console() : DebugWindow()
{
	fontColor = consoleNS::FONT_COLOR;
	backColor = consoleNS::BACK_COLOR;
	x = consoleNS::X;
	y = consoleNS::Y;
	textRect.bottom = consoleNS::Y + consoleNS::HEIGHT - consoleNS::MARGIN;
	textRect.left = consoleNS::X + consoleNS::MARGIN;
	textRect.right = consoleNS::X + consoleNS::WIDTH - consoleNS::MARGIN;
	textRect.top = consoleNS::Y + consoleNS::MARGIN;
}

//=============================================================================
// Destructor
//=============================================================================
Console::~Console()
{
	onLostDevice();            // call onLostDevice() for every graphics item
}

//======================================================================================
//コンソールの初期化
//======================================================================================
bool Console::initialize(Graphics *g, Input *in)
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
		vtx[1].x = x + consoleNS::WIDTH;
		vtx[1].y = y;
		vtx[1].z = 0.0f;
		vtx[1].rhw = 1.0f;
		vtx[1].dwColor = backColor;
		//右下
		vtx[2].x = x + consoleNS::WIDTH;
		vtx[2].y = y + consoleNS::HEIGHT;
		vtx[2].z = 0.0f;
		vtx[2].rhw = 1.0f;
		vtx[2].dwColor = backColor;
		//左下
		vtx[3].x = x;
		vtx[3].y = y + consoleNS::HEIGHT;
		vtx[3].z = 0.0f;
		vtx[3].rhw = 1.0f;
		vtx[3].dwColor = backColor;
		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
		//DirectXフォントを初期化
		if (dxFont.initialize(graphics, consoleNS::FONT_HEIGHT, false, false, consoleNS::FONT) == false)
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
const void Console::draw()
{
	if (!visible || graphics == NULL || !initialized)
		return;

	DebugWindow::draw();
}

const void Console::Appenddraw(){
	if (rowHeight <= 0)	//エラーよけ
		rowHeight = 20;

	textRect.bottom = (long)(y + consoleNS::HEIGHT - consoleNS::MARGIN);
	textRect.top = textRect.bottom - rowHeight;
	std::string prompt = ">";		//プロンプト文字れるを作成
	if (Active){
		prompt = prompt + input->getTextIn();
	}
	dxFont.print(prompt, textRect, DT_LEFT); //プロンプトとコマンドを表示
}
//======================================================================================
//コンソールコマンドを戻す
//シングルキーのコンソールコマンドを処理する
//その他のコマンドはすべてゲームに戻す
//======================================================================================
std::string Console::getCommand()
{
	if (!Active)
		return "";
	//コンソールが初期化されていない場合、または表示されていない場合
	if (!initialized || !visible)
		return "";
	//コンソールキーをチェック
	if (input->wasKeyPressed(CONSOLE_KEY))
		hide();
	//Excキーをチェック
	if (input->wasKeyPressed(ESC_KEY))
		return "";
	//スクロールをチェック
	if (input->wasKeyPressed(VK_UP))
		scrollAmount++;
	else if (input->wasKeyPressed(VK_DOWN))
		scrollAmount--;
	else if (input->wasKeyPressed(VK_PRIOR))
		scrollAmount += rows;
	else if (input->wasKeyPressed(VK_NEXT))
		scrollAmount -= rows;
	if (scrollAmount < 0)
		scrollAmount = 0;
	if (scrollAmount > consoleNS::MAX_LINE - 1)
		scrollAmount = consoleNS::MAX_LINE - 1;
	if (scrollAmount > (int)(text.size()) - 1)
		scrollAmount = (int)(text.size()) - 1;
	commandStr = input->getTextIn();	//ユーザーが入力したテキストを取得
	//キーをゲームに渡さない
	input->clear(inputNS::KEYS_DOWN | inputNS::KEYS_PRESSED | inputNS::MOUSE);
	if (commandStr.length() == 0)	//コマンドが入力されていない場合
		return "";
	//Enterキーが押されていない場合
	if (commandStr.at(commandStr.length() - 1) != '\r')
		return "";					//戻る(コマンドでない)
	//コマンドの末尾から「\r」を消去
	commandStr.erase(commandStr.length() - 1);
	input->clearTextIn();	//入力行をクリア
	inputStr = commandStr;                      // save input text
	return commandStr;
}
//=============================================================================
// called when graphics device is lost
//=============================================================================
void Console::onLostDevice()
{
	if (!initialized)
		return;
	dxFont.onLostDevice();
	SAFE_RELEASE(vertexBuffer);
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Console::onResetDevice()
{
	if (!initialized)
		return;
	graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
	dxFont.onResetDevice();
}
