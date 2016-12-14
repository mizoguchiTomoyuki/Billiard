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
	//�w��̃L�[��������Ă����Ԃ̏ꍇ��true
	bool keysDown[inputNS::KEYS_ARRAY_LEN];
	//�w��̃L�[�������ꂽ�ꍇ��true
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];
	std::string textIn; //���[�U�[�����͂����e�L�X�g
	char charIn;		//�Ō�ɓ��͂��ꂽ����
	bool newLine;	//�V�����s�̊J�n����true
	int mouseX, mouseY;	//�}�E�X�̉�ʍ��W
	int mousePosX, mousePosY;	//�}�E�X�̉�ʍ��W
	int mouseRawX, mouseRawY; //�����׃}�E�X�̃f�[�^
	RAWINPUTDEVICE Rid[1]; //�����׃}�E�X�p
	bool mouseCaptured;	//�}�E�X���L���v�`������Ă���ꍇ��true
	bool mouseLButton;	//���}�E�X�{�^����������Ă���ꍇ��true
	bool mouseMButton; //�����}�E�X�{�^����������Ă���ꍇ��true
	bool mouseRButton; //�E�}�E�X�{�^����������Ă���ꍇ��true
	bool mouseX1Button;	//X1�}�E�X�{�^����������Ă���ꍇ��true
	bool mouseX2Button;	//X2�}�E�X�{�^����������Ă���ꍇ��true
	ControllerState controllers[MAX_CONTROLLERS]; //�R���g���[���[�̏��

	//Mouse�̋@�\�g��
	bool is_Drag[inputNS::END]; //���{�^���������ςȂ���Ԃ�
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
	Input();//�R���X�g���N�^
	~Input();//�f�X�g���N�^
	void Mouseupdate();
	void initialize(HWND hwnd, bool capture);
	void keyDown(WPARAM wParam);
	void keyUp(WPARAM wParam);
	void keyIn(WPARAM wParam);
	bool isKeyDown(UCHAR vkey) const; //�L�[�̉�������p�̊֐�
	bool wasKeyPressed(UCHAR vkey) const;
	bool anyKeyPressed() const;
	std::string getTextIn(){ return textIn; }//�e�L�X�g���͂𕶎���Ƃ��ĕԂ�
	char getCharIn(){ return charIn; } //�Ō�ɓ��͂��ꂽ������Ԃ�
	void clearKeyPress(UCHAR vkey);
	void clearTextIn(){ textIn.clear(); }
	void clear(UCHAR what);
	void clearAll(){ clear(inputNS::KEYS_MOUSE_TEXT); }
	void mouseIn(LPARAM lParam);
	void mouseRawIn(LPARAM lParam);
	void mousePos();
	//�}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseLButton(bool b){ mouseLButton = b; }
	//�}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseRButton(bool b){ mouseRButton = b; }
	//�}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseMButton(bool b){ mouseMButton = b; }
	void setMouseXButton(WPARAM wParam)
	{
		mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}
	//�}�E�X�̓��͈ʒu�擾
	int getMouseX() const{ return mouseX; }
	int getMouseY() const{ return mouseY; }
	//�����׃}�E�X�ɑΉ�
	//�}�E�X�̈ʒu�̈ړ��̃��[�f�[�^��Ԃ��B��(��)�ւ̈ړ���<0�A�E(��)�ւ̈ړ���>0
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
	//�R���g���[���[�̏�Ԃ�ǂݎ��
	void readControllers();
	void checkControllers();
	//�w��̃R���g���[���[�̏��(ControllerState)�̃A�h���X��Ԃ�
	const ControllerState* getControllerState(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}
	//�R���g���[���[�̃{�^���̏�Ԃ�Ԃ�
	const WORD getGamepadButtons(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.wButtons;
	}
	//====================================================================================================
	//�ȉ��R���g���[���[�̃e�X�g�֐��B�e�֐��ɉ����ă{�^���̉����̔��肪�ł���
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

	//�R���g���[���[n�̍����[�^�[��U��������
	//���͒���g�U��
	//speed: 0 = �I�t 65536 = 100�p�[�Z���g
	//sec:�U�������邤���ԁi�b�j
	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}
	//�R���g���[���[n�̉E���[�^�[��U��������
	//�E�͍����g�U��
	//speed : 0 = �I�t�@65536 = 100�p�[�Z���g
	//sec : �U�������鎞��(�b)
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