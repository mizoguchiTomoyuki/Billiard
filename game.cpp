#include "game.h"

//==========================================
//コンストラクタ
//==========================================
Game::Game()
{
	input = new Input();
	paused = false;
	graphics = NULL;
//	audio = NULL;
	console = NULL;
	c_console = NULL;
	fps = 100;
	fpsOn = false;
	initialized = false;
}

//==========================================
//デストラクタ
//==========================================
Game::~Game()
{
	deleteAll();	//メモリ解放
	ShowCursor(true); //カーソルを表示
}

//==========================================
//メッセージハンドラ
//==========================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	if (initialized)
	{
		//入力系統のメッセージ
		switch (msg)
		{
		case WM_DESTROY:
			//Windowsにこのプログラムを終了するように伝える
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN: //キーが押された
			input->keyDown(wParam); //inputのKeyDownにwParamを与える(状態のセット？)
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP: //キーが離された
			input->keyUp(wParam); //inputのKeyDownにwParamを与える(状態のセット？)
			return 0;
		case WM_CHAR:	//文字入力があった
			input->keyIn(wParam); //inputのKeyDownにwParamを与える(状態のセット？)
			return 0;
		case WM_MOUSEMOVE: //マウスが移動された
			input->mouseIn(lParam);//inputのMouseInにlParamを与える(状態のセット？)
			input->mousePos();
			return 0;
		case WM_INPUT: //マウスからのローデータ入力
			input->mouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN: //左マウスボタンが押された
			input->setMouseLButton(true); //マウスのLボタンの状態のセット
			input->mouseIn(lParam);
			return 0;
		case WM_LBUTTONUP: //左マウスボタンが離された
			input->setMouseLButton(false); //マウスのLボタンの状態のセット
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN: //中マウスボタンが押された
			input->setMouseMButton(true); //マウスのボタンの状態のセット
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONUP://中マウスボタンが離された
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN: //右マウスボタンが押された
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONUP: //右マウスボタンが離された
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;
			//マウスのXボタンが押された/離された
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return 0;
		case WM_DEVICECHANGE: //コントローラーをチェック
			input->checkControllers();
			return 0;

		}
	}
		//Windowsに処理を任せる
		return DefWindowProc(hwnd, msg, wParam, lParam);
}
//==========================================
//初期化
//==========================================

void Game::initialize(HWND hw)
{
	hwnd = hw;
	graphics = new Graphics();
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);
	c_console = new CreateConsole();
	c_console->initialize(graphics, input);

	c_console->print("--Create--");
	c_console->setActive(false);
	console = new Console();
	console->initialize(graphics, input);
	console->print("---Console---");
	console->setActive(true);
	//audio = new Ausio();
	//サウンドファイルが定義されている場合
/*	if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0')
	{
		if (FAILED(hr = audio->initialize()))
		{
			if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
				throw(GameError(gameErrorNS::FATAL_ERROR,
				"Failed to initialize sound system "\
				"because madia file not found."));
			else
				throw(GameError(gameErrorNS::FATAL_ERROR,
				"Failed to initialize sound system."));
		}
	}*/
	//高分解能タイマーの準備を試みる
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
		QueryPerformanceCounter(&timeStart);//開始時間を取得
	if (dxFont.initialize(graphics, gameNS::POINT_SIZE, false, false, gameNS::FONT) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize DirectX font."));
	dxFont.setFontColor(gameNS::FONT_COLOR);
	initialized = true;
}


//==========================================
//ゲームのレンダー
//==========================================
void Game::renderGame(){
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];


	if (SUCCEEDED(graphics->beginScene()))
	{
		graphics->LightSet();
		render(); //派生クラスのrenderを呼び出す
		
		graphics->spriteBegin();
		if (fpsOn)
		{
			_snprintf_s(buffer, BUF_SIZE, "fps %d", (int)fps); //fps表示 未実装

		}
		graphics->spriteEnd();
		console->draw();
		c_console->draw();
		graphics->endScene();
	}
	handleLostGraphicsDevice();
	graphics->showBackbuffer();

}

//===============================================================================
//消失したグラフィックデバイスを処理
//===============================================================================
void Game::handleLostGraphicsDevice()
{
	//デバイス消失をテストし、それに応じて処理を実行
	hr = graphics->getDeviceState();
	if (FAILED(hr)) //グラフィックデバイスが有効な状態でない場合
	{
		//デバイスが消失しており、リセットできる状態にない場合
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100); //CPU時間を明け渡す(100ミリ秒)
			return;
		}
		//デバイスは消失しているがリセットできる状態にある場合

		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();//グラフィックスドライバのリセットを試みる
			if (FAILED(hr))	//リセットが失敗した場合
				return;
			resetAll();
		}
		else
			return; //他のデバイスエラー
	}

}

//===============================================================================
//DisplayModeの変更
//===============================================================================
void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	releaseAll();                   // free all user created surfaces
	graphics->changeDisplayMode(mode);
	resetAll();                     // recreate surfaces
}

//===============================================================================
//WinMainないのメインのメッセージループで繰り返し呼び出される
//===============================================================================
void Game::run(HWND hwnd)
{
	
	if (graphics == NULL) //グラフィックスが初期化されていない場合
		return;
	//最後のフレームからの経過時間を計算、frameTimeに保存
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
	//省電力コード(winmm.libが必要)
	//希望するフレームレートに対して経過時間が短い場合
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1); //1ミリ秒の分解能をWindowsタイマーに要求
		Sleep(sleepTime);//sleepTimeの間CPUを解放
		timeEndPeriod(1); //1ミリ秒のタイマー分解能を終了
		return;

	}
	if (frameTime > 0.0)
		fps = (fps*0.99f) + (0.01f / frameTime); //平均fps

	if (frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;

	timeStart = timeEnd;
	//update(),ai(),collisions()は純粋仮想関数
	if (!paused)//一時停止中ではない場合
	{
		update();	//すべてのゲームアイテムを更新
		ai(); //人工知能
		collisions();	//衝突を処理
		input->vibrateControllers(frameTime);//コントローラーの振動を処理
	}
	renderGame();	//すべてのゲームアイテムを描画
	input->readControllers();
#ifdef _DEBUG
	if (input->wasKeyPressed(CONSOLE_KEY))
	{
		console->showHide();
		c_console->showHide();
		//コンソールを表示されている間、ゲームを一時停止
		paused = console->getVisible();
	}
	consoleCommand();
#endif


	// if Alt+Enter toggle fullscreen/window
	if (input->isKeyDown(ALT_KEY) && input->wasKeyPressed(ENTER_KEY))
		setDisplayMode(graphicsNS::TOGGLE); // toggle fullscreen/window

	// if Esc key, set window mode
	if (input->isKeyDown(ESC_KEY))
		setDisplayMode(graphicsNS::WINDOW); // set window mode

	//入力をクリア
	//すべてのキーチェックが行われた後これを呼び出す
	input->clear(inputNS::KEYS_PRESSED);
	input->clear(inputNS::MOUSE);
	/*	input->clear(inputNS::KEYS_DOWN);
	
	input->clear(inputNS::TEXT_IN);*/
	//audio->run();
}

void Game::releaseAll()
{
	SAFE_ON_LOST_DEVICE(console);
	SAFE_ON_LOST_DEVICE(c_console);
	dxFont.onLostDevice();
	return;
}

void Game::resetAll()
{

	dxFont.onResetDevice();
	SAFE_ON_RESET_DEVICE(console);
	SAFE_ON_RESET_DEVICE(c_console);
	return;
}

void Game::deleteAll()
{
	releaseAll();               // call onLostDevice() for every graphics item
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	//SAFE_DELETE(audio);
	SAFE_DELETE(console);
	SAFE_DELETE(c_console);
	initialized = false;
}

void Game::consoleCommand()
{
	if (console->GetActive()){
		command = console->getCommand(); //コンソールからのコマンドを取得
		if (command == "")		//コマンドがない場合
			return;
		if (command == "help")	//「help」コマンドの場合
		{
			console->print("Console Commands:");
			console->print("fps - toggle display of frames per second");
			console->print("create - Create window activate.");
			console->print("save - Save object database.");
			return;
		}
		if (command == "fps")
		{
			fpsOn = !fpsOn;		//フレームレートの表示を切り替える
			if (fpsOn)
				console->print("fps On");
			else
				console->print("fps Off");
		}
		if (command == "create")
		{
			c_console->setActive(true);
			console->setActive(false);
		}

		if (command == "save") //objectDataを保存
		{
			SaveODM();
			console->print("Save Complete.");
		}
	}

	if (c_console->GetActive()){
		command = c_console->getCommand();


		if (command == "exit")
		{
			c_console->setActive(false);
			console->setActive(true);
		}

	}

}

void Game::SaveODM(){
	ODM->DataSave();

}
