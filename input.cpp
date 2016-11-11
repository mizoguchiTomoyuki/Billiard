// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 4 graphics.cpp v1.0

#include "input.h"

//==========================================================================
//デフォルトコンストラクタ
//==========================================================================
Input::Input()
{
	//キーが押されている状態を示す配列をクリア
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		keysDown[i] = false;
	//キーが押されたことを示す配列をクリア
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		keysPressed[i] = false;

	newLine = true;		//新しい行を開始
	textIn = "";	//textInをクリア
	charIn = 0;		//charInをクリア
	//マウスデータ
	mouseX = 0;			//マウスの画面位置
	mouseY = 0;
	mouseRawX = 0;		//高精細マウスの画面位置
	mouseRawY = 0;
	mousePosX = 0;
	mousePosY = 0;
	mouseLButton = false; //各ボタンの押下状態
	mouseMButton = false;
	mouseRButton = false;
	mouseX1Button = false;
	mouseX2Button = false;
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		controllers[i].vibrateTimeLeft = 0;
		controllers[i].vibrateTimeRight = 0;
	}

	for (int i = 0; i < inputNS::END; i++){
		is_Drag[i] = false;
		TrackStart_x[i] = 0;
		TrackStart_y[i] = 0;
		DragDistance_x[i] = 0;
		DragDistance_y[i] = 0;
		Time_MouseClick[i] = 0.0f;
	}

}
//==========================================================================
//デストラクタ
//==========================================================================
Input::~Input()
{
	if (mouseCaptured)
		ReleaseCapture(); //マウスを解放
}

//==========================================================================
//Initialize マウスとコントローラーの入力を初期化
//マウスをキャプチャする場合、capture=trueを設定
//GameErrorをスロー
//==========================================================================
void Input::initialize(HWND hwnd, bool capture)
{
	try{
		mouseCaptured = capture;
		//高精細マウスを登録
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = hwnd;
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
		if (mouseCaptured)
			SetCapture(hwnd); //マウスをキャプチャ
		//コントローラーの状態をクリア
		ZeroMemory(
			controllers, sizeof(ControllerState) * MAX_CONTROLLERS);

		checkControllers(); //接続されているコントローラーをチェック
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error Initializing input system"));
	}
}


//==========================================================================
//このキーについて、keysDown配列とkeyPressed配列にtrueを設定
//==========================================================================
void Input::keyDown(WPARAM wParam)
{
	//キーコードがバッファ範囲内にあることを確認
	if (wParam < inputNS::KEYS_ARRAY_LEN)
	{
		keysDown[wParam] = true;	//keysDown配列を更新
		//キーがすでに押されていた、clear()で消去されていた
		keysPressed[wParam] = true;//keysPressed配列を更新
	}
}

//==========================================================================
//このキーについて、keysDown配列とfalseを設定
//実行前：wParamに仮想キーコード(0～255)が格納されている
//==========================================================================
void Input::keyUp(WPARAM wParam)
{
	//キーコードがバッファ範囲内にあることを確認
	if (wParam < inputNS::KEYS_ARRAY_LEN)
	{
		keysDown[wParam] = false; //状態テーブルを更新
	}
}


//==========================================================================
//入力された文字をtextIn文字列に保存
//実行前：wParamに文字列が格納されている
//==========================================================================
void Input::keyIn(WPARAM wParam)
{
	if (newLine)	//新しい行の開始の場合
	{
		textIn.clear();
		newLine = false;
	}
	if (wParam == '\b') //バックスペースキーの場合
	{
		if (textIn.length() > 0) //文字が存在する場合
			textIn.erase(textIn.size() - 1); //最後に入力された文字を消去

	}
	else
	{
		textIn += wParam;	//文字をtextInに追加
		charIn = wParam;	//最後に入力された文字を保存
	}
	if ((char)wParam == '\r')
		newLine = true;
}

//==========================================================================
//指定された仮想キーが押されている場合はtrue, それ以外の場合はfalseを返す
//==========================================================================
bool Input::isKeyDown(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysDown[vkey];
	else
		return false;
}
//==========================================================================
//最近のフレームにおいて、指定の仮想キーが押されたことがある場合、trueを返す。
//キーの押し下げの状態は、各フレームの終了時に消去されます。
//==========================================================================
bool Input::wasKeyPressed(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysPressed[vkey];
	else
		return false;

}

//==========================================================================
//最近のフレームにおいて、なんらかのキーが押された場合、trueを返す。
//キーの押し下げの状態は、各フレームの終了時に消去されます。
//==========================================================================
bool Input::anyKeyPressed() const
{
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		if (keysPressed[i] == true)
			return true;
	return false;
}

//==========================================================================
//指定されたキーの押し下げをクリア
//==========================================================================
void Input::clearKeyPress(UCHAR vkey)
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		keysPressed[vkey] = false;
}

//==========================================================================
//指定の入力バッファをクリア
//whatの値についてはinput.hを参照
//==========================================================================
void Input::clear(UCHAR what)
{
	//キーが押されている状態かどうかをクリアする場合
	if (what & inputNS::KEYS_DOWN)
	{
		for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
			keysDown[i] = false;
	}
	//キーが押されたかどうかをクリアする場合
	if (what & inputNS::KEYS_PRESSED)
	{
		for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
			keysPressed[i] = false;
	}
	//マウスをクリアする場合
	if (what &inputNS::MOUSE)
	{
		mouseX = 0;
		mouseY = 0;
		mouseRawX = 0;
		mouseRawY = 0;
	}
	if (what & inputNS::TEXT_IN)
		clearTextIn();
}

//==========================================================================
//マウスの画面一を読み取りmouseX,mouseYに保存
//==========================================================================
void Input::mouseIn(LPARAM lParam)
{
	mouseX = GET_X_LPARAM(lParam);
	mouseY = GET_Y_LPARAM(lParam);



}

void Input::mousePos()
{

	POINT pt;
	GetCursorPos(&pt);
	mousePosX = pt.x;
	mousePosY = pt.y;
}
void Input::Mouseupdate(){
	int X = mousePosX;
	int Y = mousePosY;

	if (getMouseLButton()){
		if (!is_Drag[inputNS::M_LEFT]){
			is_Drag[inputNS::M_LEFT] = true;
			TrackStart_x[inputNS::M_LEFT] = X;
			TrackStart_y[inputNS::M_LEFT] = Y;
		}
	}
	else{
		if (is_Drag[inputNS::M_LEFT]){
			is_Drag[inputNS::M_LEFT] = false;
			TrackStart_x[inputNS::M_LEFT] = 0;
			TrackStart_y[inputNS::M_LEFT] = 0;
			Time_MouseClick[inputNS::M_LEFT] = 0.0f;
		}
	}

	if (getMouseRButton()){
		if (!is_Drag[inputNS::M_RIGHT]){
			is_Drag[inputNS::M_RIGHT] = true;
			TrackStart_x[inputNS::M_RIGHT] = X;
			TrackStart_y[inputNS::M_RIGHT] = Y;
		}
	}
	else{
		if (is_Drag[inputNS::M_LEFT]){
			is_Drag[inputNS::M_RIGHT] = false;
			TrackStart_x[inputNS::M_RIGHT] = 0;
			TrackStart_y[inputNS::M_RIGHT] = 0;
			Time_MouseClick[inputNS::M_RIGHT] = 0.0f;
		}
	}


	if (getMouseMButton()){
		if (!is_Drag[inputNS::M_MID]){
			is_Drag[inputNS::M_MID] = true;
			TrackStart_x[inputNS::M_MID] = X;
			TrackStart_y[inputNS::M_MID] = Y;
		}
	}
	else{
		if (is_Drag[inputNS::M_LEFT]){
			is_Drag[inputNS::M_MID] = false;
			TrackStart_x[inputNS::M_MID] = 0;
			TrackStart_y[inputNS::M_MID] = 0;
			Time_MouseClick[inputNS::M_MID] = 0.0f;
		}
	}



	for (int i = 0; i < inputNS::END; i++){
		if (is_Drag[i]){
			DragDistance_x[i] = TrackStart_x[i] - X;
			DragDistance_y[i] = TrackStart_y[i] - Y;
			Div_x[i] = X*0.01f;
			Div_y[i] = Y*0.01f;
		}
		else{
			DragDistance_x[i] = 0;
			DragDistance_y[i] = 0;
			Div_x[i] = 0.0f;
			Div_y[i] = 0.0f;
		}

		StampPoint_x[i] = X;
		StampPoint_y[i] = Y;

	}

}
//==========================================================================
//マウスからのローデータを読み取り、mouseRawX,mouseRawYに保存
//このルーチンは高精細マウスに対応しております.
//==========================================================================
void Input::mouseRawIn(LPARAM lParam)
{
	UINT dwSize = 40;
	static BYTE lpb[40];
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
		lpb, &dwSize, sizeof(RAWINPUTHEADER));
	RAWINPUT* raw = (RAWINPUT*)lpb;
	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		mouseRawX = raw->data.mouse.lLastX;
		mouseRawY = raw->data.mouse.lLastY;
	}
}


//==========================================================================
//コントローラーの状態を読み取る
//==========================================================================
void Input::readControllers()
{
	DWORD result;
	for (DWORD i = 0; i<MAX_CONTROLLERS; i++)
	{
		if (controllers[i].connected)
		{
			result = XInputGetState(i, &controllers[i].state);
			if (result == ERROR_DEVICE_NOT_CONNECTED)//接続されていない場合
				controllers[i].connected = false;
		}
	}
	Mouseupdate();
}

//=============================================================================
// Check for connected controllers
//=============================================================================
void Input::checkControllers()
{
	DWORD result;
	for (DWORD i = 0; i <MAX_CONTROLLERS; i++)
	{
		result = XInputGetState(i, &controllers[i].state);
		if (result == ERROR_SUCCESS)
			controllers[i].connected = true;
		else
			controllers[i].connected = false;
	}
}

//==========================================================================
//接続されているコントローラーを振動させる
//gameclassのrunに組み込まれているため常時呼び出される
//呼び出されているときgamePadVibrate～を使用すると振動設定がcontrollersに書き込まれ
//その設定時間分振動する
//==========================================================================
void Input::vibrateControllers(float frameTime)
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		if (controllers[i].connected)
		{
			controllers[i].vibrateTimeLeft -= frameTime; //コントローラーに設定されたvibrateTime分(frameTime)振動させたら各パラメーターを0に
			if (controllers[i].vibrateTimeLeft < 0)
			{
				controllers[i].vibrateTimeLeft = 0;
				controllers[i].vibration.wLeftMotorSpeed = 0;
			}
			controllers[i].vibrateTimeRight -= frameTime;
			if (controllers[i].vibrateTimeRight < 0)
			{
				controllers[i].vibrateTimeRight = 0;
				controllers[i].vibration.wRightMotorSpeed = 0;
			}
			XInputSetState(i, &controllers[i].vibration); //コントローラーiをバイブレーション
		}
	}
}



