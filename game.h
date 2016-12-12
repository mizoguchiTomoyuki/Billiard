#ifndef _GAME_H
#define _GAME_H
#define Win32_LEAN_AND_MEAN
//ゲームシーンのもととなるクラス2
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
	HWND hwnd; //ウインドウのハンドル
	HRESULT hr;
	LARGE_INTEGER timeStart; //パフォーマンスカウンターの開始値
	LARGE_INTEGER timeEnd; //パフォーマンスカウンターの終了値
	LARGE_INTEGER timerFreq; //パフォーマンスカウンターの周波数
	Console* console;               // pointer to Console
	CreateConsole* c_console;
	float frameTime;	//最後のフレームに要した時間
	float fps; //フレームレート
	TextDX dxFont;
	bool fpsOn;
	DWORD sleepTime;	//フレーム間でスリープする時間(ミリ秒)
	bool paused;	//ゲームが一時停止されている場合、true
	bool initialized;
	std::string  command;           // command from console
	ObjectDataManager* ODM;
	bool Slowmotion; //コマ送りモードを開始する
	bool Trigger; //コマ送り処理を進める
	bool preinput;
public:
	//コンストラクタ
	Game();

	//デストラクタ	
	virtual ~Game();

	//メンバー関数

	//Windowsメッセージハンドラ
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//ゲームを初期化
	//実行前hwndはウインドウへのハンドル
	virtual void initialize(HWND hw);

	//WinMain内のメインのメッセージループでrunを繰り返し呼び出す
	virtual void run(HWND hwnd);

	//グラフィックスデバイスが消失したときに呼び出す
	//グラフィックスデバイスをリセット可能にするため、
	//予約されていたビデオメモリをすべて解放
	virtual void releaseAll();

	//すべてのサーフェイスを再作成し、すべてのエンティティをリセット
	virtual void resetAll();

	//予約されていたメモリをすべて削除
	virtual void deleteAll();

	//ゲームアイテムをレンダー
	virtual void renderGame();

	//消失したグラフィックデバイスを処理
	virtual void handleLostGraphicsDevice();

	virtual void consoleCommand();

	// Displayモードの変更
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	//Graphicsへのポインタを戻す
	Graphics* getGraphics(){ return graphics; }

	//Inputへのポインタを返す
	Input* getInput(){ return input; }

	//Audioへのポインタを返す
//	Audio* getAudio(){ return audio; }

	//ゲームを終了
	void exitGame(){ PostMessage(hwnd, WM_DESTROY, 0, 0); }

	//純粋仮想関数の宣言
	//以下の関数でそれぞれのゲームシーンでの処理を行う

	//ゲームアイテムを更新
	virtual void update() = 0;
	//AI計算を実行
	virtual void ai() = 0;
	//衝突をチェック
	virtual void collisions() = 0;
	//グラフィックスをレンダー
	//graphics->spriteBegin();を呼び出す。
	//スプライトを描画
	//graphics->spriteEnd();を呼び出す
	//スプライト以外を描画
	virtual void render() = 0;

	float getframeTime(){ return frameTime; }
	bool getpause(){ return paused; }
	ObjectDataManager* getODM(){ return ODM; }

	void SaveODM();
	virtual void ResetGame() = 0;
};
#include "ObjectDataManager.h"
#endif