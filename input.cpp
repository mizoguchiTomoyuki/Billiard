// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 4 graphics.cpp v1.0

#include "input.h"

//==========================================================================
//�f�t�H���g�R���X�g���N�^
//==========================================================================
Input::Input()
{
	//�L�[��������Ă����Ԃ������z����N���A
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		keysDown[i] = false;
	//�L�[�������ꂽ���Ƃ������z����N���A
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		keysPressed[i] = false;

	newLine = true;		//�V�����s���J�n
	textIn = "";	//textIn���N���A
	charIn = 0;		//charIn���N���A
	//�}�E�X�f�[�^
	mouseX = 0;			//�}�E�X�̉�ʈʒu
	mouseY = 0;
	mouseRawX = 0;		//�����׃}�E�X�̉�ʈʒu
	mouseRawY = 0;
	mousePosX = 0;
	mousePosY = 0;
	mouseLButton = false; //�e�{�^���̉������
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
//�f�X�g���N�^
//==========================================================================
Input::~Input()
{
	if (mouseCaptured)
		ReleaseCapture(); //�}�E�X�����
}

//==========================================================================
//Initialize �}�E�X�ƃR���g���[���[�̓��͂�������
//�}�E�X���L���v�`������ꍇ�Acapture=true��ݒ�
//GameError���X���[
//==========================================================================
void Input::initialize(HWND hwnd, bool capture)
{
	try{
		mouseCaptured = capture;
		//�����׃}�E�X��o�^
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = hwnd;
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
		if (mouseCaptured)
			SetCapture(hwnd); //�}�E�X���L���v�`��
		//�R���g���[���[�̏�Ԃ��N���A
		ZeroMemory(
			controllers, sizeof(ControllerState) * MAX_CONTROLLERS);

		checkControllers(); //�ڑ�����Ă���R���g���[���[���`�F�b�N
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error Initializing input system"));
	}
}


//==========================================================================
//���̃L�[�ɂ��āAkeysDown�z���keyPressed�z���true��ݒ�
//==========================================================================
void Input::keyDown(WPARAM wParam)
{
	//�L�[�R�[�h���o�b�t�@�͈͓��ɂ��邱�Ƃ��m�F
	if (wParam < inputNS::KEYS_ARRAY_LEN)
	{
		keysDown[wParam] = true;	//keysDown�z����X�V
		//�L�[�����łɉ�����Ă����Aclear()�ŏ�������Ă���
		keysPressed[wParam] = true;//keysPressed�z����X�V
	}
}

//==========================================================================
//���̃L�[�ɂ��āAkeysDown�z���false��ݒ�
//���s�O�FwParam�ɉ��z�L�[�R�[�h(0�`255)���i�[����Ă���
//==========================================================================
void Input::keyUp(WPARAM wParam)
{
	//�L�[�R�[�h���o�b�t�@�͈͓��ɂ��邱�Ƃ��m�F
	if (wParam < inputNS::KEYS_ARRAY_LEN)
	{
		keysDown[wParam] = false; //��ԃe�[�u�����X�V
	}
}


//==========================================================================
//���͂��ꂽ������textIn������ɕۑ�
//���s�O�FwParam�ɕ����񂪊i�[����Ă���
//==========================================================================
void Input::keyIn(WPARAM wParam)
{
	if (newLine)	//�V�����s�̊J�n�̏ꍇ
	{
		textIn.clear();
		newLine = false;
	}
	if (wParam == '\b') //�o�b�N�X�y�[�X�L�[�̏ꍇ
	{
		if (textIn.length() > 0) //���������݂���ꍇ
			textIn.erase(textIn.size() - 1); //�Ō�ɓ��͂��ꂽ����������

	}
	else
	{
		textIn += wParam;	//������textIn�ɒǉ�
		charIn = wParam;	//�Ō�ɓ��͂��ꂽ������ۑ�
	}
	if ((char)wParam == '\r')
		newLine = true;
}

//==========================================================================
//�w�肳�ꂽ���z�L�[��������Ă���ꍇ��true, ����ȊO�̏ꍇ��false��Ԃ�
//==========================================================================
bool Input::isKeyDown(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysDown[vkey];
	else
		return false;
}
//==========================================================================
//�ŋ߂̃t���[���ɂ����āA�w��̉��z�L�[�������ꂽ���Ƃ�����ꍇ�Atrue��Ԃ��B
//�L�[�̉��������̏�Ԃ́A�e�t���[���̏I�����ɏ�������܂��B
//==========================================================================
bool Input::wasKeyPressed(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysPressed[vkey];
	else
		return false;

}

//==========================================================================
//�ŋ߂̃t���[���ɂ����āA�Ȃ�炩�̃L�[�������ꂽ�ꍇ�Atrue��Ԃ��B
//�L�[�̉��������̏�Ԃ́A�e�t���[���̏I�����ɏ�������܂��B
//==========================================================================
bool Input::anyKeyPressed() const
{
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		if (keysPressed[i] == true)
			return true;
	return false;
}

//==========================================================================
//�w�肳�ꂽ�L�[�̉����������N���A
//==========================================================================
void Input::clearKeyPress(UCHAR vkey)
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		keysPressed[vkey] = false;
}

//==========================================================================
//�w��̓��̓o�b�t�@���N���A
//what�̒l�ɂ��Ă�input.h���Q��
//==========================================================================
void Input::clear(UCHAR what)
{
	//�L�[��������Ă����Ԃ��ǂ������N���A����ꍇ
	if (what & inputNS::KEYS_DOWN)
	{
		for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
			keysDown[i] = false;
	}
	//�L�[�������ꂽ���ǂ������N���A����ꍇ
	if (what & inputNS::KEYS_PRESSED)
	{
		for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
			keysPressed[i] = false;
	}
	//�}�E�X���N���A����ꍇ
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
//�}�E�X�̉�ʈ��ǂݎ��mouseX,mouseY�ɕۑ�
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
//�}�E�X����̃��[�f�[�^��ǂݎ��AmouseRawX,mouseRawY�ɕۑ�
//���̃��[�`���͍����׃}�E�X�ɑΉ����Ă���܂�.
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
//�R���g���[���[�̏�Ԃ�ǂݎ��
//==========================================================================
void Input::readControllers()
{
	DWORD result;
	for (DWORD i = 0; i<MAX_CONTROLLERS; i++)
	{
		if (controllers[i].connected)
		{
			result = XInputGetState(i, &controllers[i].state);
			if (result == ERROR_DEVICE_NOT_CONNECTED)//�ڑ�����Ă��Ȃ��ꍇ
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
//�ڑ�����Ă���R���g���[���[��U��������
//gameclass��run�ɑg�ݍ��܂�Ă��邽�ߏ펞�Ăяo�����
//�Ăяo����Ă���Ƃ�gamePadVibrate�`���g�p����ƐU���ݒ肪controllers�ɏ������܂�
//���̐ݒ莞�ԕ��U������
//==========================================================================
void Input::vibrateControllers(float frameTime)
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		if (controllers[i].connected)
		{
			controllers[i].vibrateTimeLeft -= frameTime; //�R���g���[���[�ɐݒ肳�ꂽvibrateTime��(frameTime)�U����������e�p�����[�^�[��0��
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
			XInputSetState(i, &controllers[i].vibration); //�R���g���[���[i���o�C�u���[�V����
		}
	}
}



