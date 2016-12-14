#ifndef INPUT_H
#define INPUT_H

#define WIN32_LEAN_AND_MEAN
class Input;

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>
#include "constants.h"
#include "gameError.h"

// for high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif
//--------------------------
namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256; 


	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;


	enum M_BUTTONTYPE{
		M_LEFT = 0,
		M_RIGHT = 1,
		M_MID = 2,
		END
	};
}

const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0X7FFF);
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;
const DWORD MAX_CONTROLLERS = 4;

const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;


struct ControllerState
{
	XINPUT_STATE        state;
	XINPUT_VIBRATION    vibration;
	float               vibrateTimeLeft;    // mSec
	float               vibrateTimeRight;   // mSec
	bool                connected;
};
class Input
{
private:
	//指定のキーが押されている状態の場合にtrue
	bool keysDown[inputNS::KEYS_ARRAY_LEN];
	//指定のキーが押された場合にtrue
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];
	std::string textIn; //ユーザーが入力したテキスト
	char charIn;		//最後に入力された文字
	bool newLine;	//新しい行の開始時にtrue
	int mouseX, mouseY;	//マウスの画面座標
	int mousePosX, mousePosY;	//マウスの画面座標
	int mouseRawX, mouseRawY; //高精細マウスのデータ
	RAWINPUTDEVICE Rid[1]; //高精細マウス用
	bool mouseCaptured;	//マウスがキャプチャされている場合にtrue
	bool mouseLButton;	//左マウスボタンが押されている場合にtrue
	bool mouseMButton; //中央マウスボタンが押されている場合にtrue
	bool mouseRButton; //右マウスボタンが押されている場合にtrue
	bool mouseX1Button;	//X1マウスボタンが押されている場合にtrue
	bool mouseX2Button;	//X2マウスボタンが押されている場合にtrue
	ControllerState controllers[MAX_CONTROLLERS]; //コントローラーの状態

	//Mouseの機能拡張
	bool is_Drag[inputNS::END]; //左ボタン押しっぱなし状態か
	int TrackStart_x[inputNS::END];
	int TrackStart_y[inputNS::END];
	int DragDistance_x[inputNS::END];
	int DragDistance_y[inputNS::END];

	int StampPoint_x[inputNS::END];
	int StampPoint_y[inputNS::END];
	int Div_x[inputNS::END];
	int Div_y[inputNS::END];

	float Time_MouseClick[inputNS::END];


public:
	Input();//コンストラクタ
	~Input();//デストラクタ
	void Mouseupdate();
	void initialize(HWND hwnd, bool capture);
	void keyDown(WPARAM wParam);
	void keyUp(WPARAM wParam);
	void keyIn(WPARAM wParam);
	bool isKeyDown(UCHAR vkey) const; //キーの押下判定用の関数
	bool wasKeyPressed(UCHAR vkey) const;
	bool anyKeyPressed() const;
	std::string getTextIn(){ return textIn; }//テキスト入力を文字列として返す
	char getCharIn(){ return charIn; } //最後に入力された文字を返す
	void clearKeyPress(UCHAR vkey);
	void clearTextIn(){ textIn.clear(); }
	void clear(UCHAR what);
	void clearAll(){ clear(inputNS::KEYS_MOUSE_TEXT); }
	void mouseIn(LPARAM lParam);
	void mouseRawIn(LPARAM lParam);
	void mousePos();
	//マウスボタンの状態を保存
	void setMouseLButton(bool b){ mouseLButton = b; }
	//マウスボタンの状態を保存
	void setMouseRButton(bool b){ mouseRButton = b; }
	//マウスボタンの状態を保存
	void setMouseMButton(bool b){ mouseMButton = b; }
	void setMouseXButton(WPARAM wParam)
	{
		mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}
	//マウスの入力位置取得
	int getMouseX() const{ return mouseX; }
	int getMouseY() const{ return mouseY; }
	//高精細マウスに対応
	//マウスの位置の移動のローデータを返す。左(上)への移動は<0、右(下)への移動は>0
	int getMouseRawX() const { return mouseRawX; }
	int getMouseRawY() const { return mouseRawY; }
	int getMousePosX() const { return mousePosX; }
	int getMousePosY() const { return mousePosY; }
	bool getMouseLButton() const { return mouseLButton; }
	bool getMouseMButton() const { return mouseMButton; }
	bool getMouseRButton() const { return mouseRButton; }
	bool getMouseX1Button() const { return mouseX1Button; }
	bool getMouseX2Button() const { return mouseX2Button; }
	bool isDraging(inputNS::M_BUTTONTYPE m){ return is_Drag[m]; }
	//コントローラーの状態を読み取る
	void readControllers();
	void checkControllers();
	//指定のコントローラーの状態(ControllerState)のアドレスを返す
	const ControllerState* getControllerState(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}
	//コントローラーのボタンの状態を返す
	const WORD getGamepadButtons(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.wButtons;
	}
	//====================================================================================================
	//以下コントローラーのテスト関数。各関数に応じてボタンの押下の判定ができる
	bool getGameopadDPadUp(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
	}
	//Down
	bool getGamepadDPadDown(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
	}

	//Left
	bool getGamepadDPadLeft(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
	}

	//Right
	bool getGamepadDPadRight(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
	}

	//Start
	bool getGamepadStart(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
	}

	//Back
	bool getGamepadBack(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
	}

	//Left Thumb
	bool getGamepadLeftThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
	}

	//Right Thumb
	bool getGamepadRightThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
	}

	//Left Shoulder
	bool getGamepadLeftShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
	}

	//Right Shoulder button
	bool getGamepadRightShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	//A button
	bool getGamepadA(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
	}

	//B button
	bool getGamepadB(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
	}

	//X button
	bool getGamepadX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
	}

	//Y button
	bool getGamepadY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
	}

	//Left Trigger
	BYTE getGamepadLeftTrigger(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bLeftTrigger);
	}

	//Right Trigger
	BYTE getGamepadRightTrigger(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bRightTrigger);
	}

	//Left Thumbstick X
	SHORT getGamepadThumbLX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLX);
	}

	//Left Thumbstick Y
	SHORT getGamepadThumbLY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLY);
	}

	//Right Thumbstick X
	SHORT getGamepadThumbRX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRX);
	}

	//Right Thumbstick Y
	SHORT getGamepadThumbRY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRY);
	}
	//====================================================================================================

	//コントローラーnの左モーターを振動させる
	//左は低周波振動
	//speed: 0 = オフ 65536 = 100パーセント
	//sec:振動させるう時間（秒）
	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}
	//コントローラーnの右モーターを振動させる
	//右は高周波振動
	//speed : 0 = オフ　65536 = 100パーセント
	//sec : 振動させる時間(秒)
	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrateTimeRight = sec;
	}

	void vibrateControllers(float frameTime);

	int getDragDistance_X(inputNS::M_BUTTONTYPE type){ return DragDistance_x[(int)type]; };
	int getDragDistance_Y(inputNS::M_BUTTONTYPE type){ return DragDistance_y[(int)type]; };

	int getDragStart_X(inputNS::M_BUTTONTYPE type){ return TrackStart_x[(int)type]; };
	int getDragStart_Y(inputNS::M_BUTTONTYPE type){ return TrackStart_y[(int)type]; };

	float getTime_MouseClick(inputNS::M_BUTTONTYPE type){ return Time_MouseClick[(int)type]; }

	int getDiv_X(inputNS::M_BUTTONTYPE type){ return Div_x[(int)type]; }
	int getDiv_Y(inputNS::M_BUTTONTYPE type){ return Div_y[(int)type]; }

};
#endif