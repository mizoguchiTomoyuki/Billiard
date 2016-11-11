#include "game.h"

//==========================================
//�R���X�g���N�^
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
//�f�X�g���N�^
//==========================================
Game::~Game()
{
	deleteAll();	//���������
	ShowCursor(true); //�J�[�\����\��
}

//==========================================
//���b�Z�[�W�n���h��
//==========================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	if (initialized)
	{
		//���͌n���̃��b�Z�[�W
		switch (msg)
		{
		case WM_DESTROY:
			//Windows�ɂ��̃v���O�������I������悤�ɓ`����
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN: //�L�[�������ꂽ
			input->keyDown(wParam); //input��KeyDown��wParam��^����(��Ԃ̃Z�b�g�H)
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP: //�L�[�������ꂽ
			input->keyUp(wParam); //input��KeyDown��wParam��^����(��Ԃ̃Z�b�g�H)
			return 0;
		case WM_CHAR:	//�������͂�������
			input->keyIn(wParam); //input��KeyDown��wParam��^����(��Ԃ̃Z�b�g�H)
			return 0;
		case WM_MOUSEMOVE: //�}�E�X���ړ����ꂽ
			input->mouseIn(lParam);//input��MouseIn��lParam��^����(��Ԃ̃Z�b�g�H)
			input->mousePos();
			return 0;
		case WM_INPUT: //�}�E�X����̃��[�f�[�^����
			input->mouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN: //���}�E�X�{�^���������ꂽ
			input->setMouseLButton(true); //�}�E�X��L�{�^���̏�Ԃ̃Z�b�g
			input->mouseIn(lParam);
			return 0;
		case WM_LBUTTONUP: //���}�E�X�{�^���������ꂽ
			input->setMouseLButton(false); //�}�E�X��L�{�^���̏�Ԃ̃Z�b�g
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN: //���}�E�X�{�^���������ꂽ
			input->setMouseMButton(true); //�}�E�X�̃{�^���̏�Ԃ̃Z�b�g
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONUP://���}�E�X�{�^���������ꂽ
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN: //�E�}�E�X�{�^���������ꂽ
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONUP: //�E�}�E�X�{�^���������ꂽ
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;
			//�}�E�X��X�{�^���������ꂽ/�����ꂽ
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return 0;
		case WM_DEVICECHANGE: //�R���g���[���[���`�F�b�N
			input->checkControllers();
			return 0;

		}
	}
		//Windows�ɏ�����C����
		return DefWindowProc(hwnd, msg, wParam, lParam);
}
//==========================================
//������
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
	//�T�E���h�t�@�C������`����Ă���ꍇ
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
	//������\�^�C�}�[�̏��������݂�
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
		QueryPerformanceCounter(&timeStart);//�J�n���Ԃ��擾
	if (dxFont.initialize(graphics, gameNS::POINT_SIZE, false, false, gameNS::FONT) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize DirectX font."));
	dxFont.setFontColor(gameNS::FONT_COLOR);
	initialized = true;
}


//==========================================
//�Q�[���̃����_�[
//==========================================
void Game::renderGame(){
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];


	if (SUCCEEDED(graphics->beginScene()))
	{
		graphics->LightSet();
		render(); //�h���N���X��render���Ăяo��
		
		graphics->spriteBegin();
		if (fpsOn)
		{
			_snprintf_s(buffer, BUF_SIZE, "fps %d", (int)fps); //fps�\�� ������

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
//���������O���t�B�b�N�f�o�C�X������
//===============================================================================
void Game::handleLostGraphicsDevice()
{
	//�f�o�C�X�������e�X�g���A����ɉ����ď��������s
	hr = graphics->getDeviceState();
	if (FAILED(hr)) //�O���t�B�b�N�f�o�C�X���L���ȏ�ԂłȂ��ꍇ
	{
		//�f�o�C�X���������Ă���A���Z�b�g�ł����ԂɂȂ��ꍇ
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100); //CPU���Ԃ𖾂��n��(100�~���b)
			return;
		}
		//�f�o�C�X�͏������Ă��邪���Z�b�g�ł����Ԃɂ���ꍇ

		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();//�O���t�B�b�N�X�h���C�o�̃��Z�b�g�����݂�
			if (FAILED(hr))	//���Z�b�g�����s�����ꍇ
				return;
			resetAll();
		}
		else
			return; //���̃f�o�C�X�G���[
	}

}

//===============================================================================
//DisplayMode�̕ύX
//===============================================================================
void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	releaseAll();                   // free all user created surfaces
	graphics->changeDisplayMode(mode);
	resetAll();                     // recreate surfaces
}

//===============================================================================
//WinMain�Ȃ��̃��C���̃��b�Z�[�W���[�v�ŌJ��Ԃ��Ăяo�����
//===============================================================================
void Game::run(HWND hwnd)
{
	
	if (graphics == NULL) //�O���t�B�b�N�X������������Ă��Ȃ��ꍇ
		return;
	//�Ō�̃t���[������̌o�ߎ��Ԃ��v�Z�AframeTime�ɕۑ�
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
	//�ȓd�̓R�[�h(winmm.lib���K�v)
	//��]����t���[�����[�g�ɑ΂��Čo�ߎ��Ԃ��Z���ꍇ
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1); //1�~���b�̕���\��Windows�^�C�}�[�ɗv��
		Sleep(sleepTime);//sleepTime�̊�CPU�����
		timeEndPeriod(1); //1�~���b�̃^�C�}�[����\���I��
		return;

	}
	if (frameTime > 0.0)
		fps = (fps*0.99f) + (0.01f / frameTime); //����fps

	if (frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;

	timeStart = timeEnd;
	//update(),ai(),collisions()�͏������z�֐�
	if (!paused)//�ꎞ��~���ł͂Ȃ��ꍇ
	{
		update();	//���ׂẴQ�[���A�C�e�����X�V
		ai(); //�l�H�m�\
		collisions();	//�Փ˂�����
		input->vibrateControllers(frameTime);//�R���g���[���[�̐U��������
	}
	renderGame();	//���ׂẴQ�[���A�C�e����`��
	input->readControllers();
#ifdef _DEBUG
	if (input->wasKeyPressed(CONSOLE_KEY))
	{
		console->showHide();
		c_console->showHide();
		//�R���\�[����\������Ă���ԁA�Q�[�����ꎞ��~
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

	//���͂��N���A
	//���ׂẴL�[�`�F�b�N���s��ꂽ�ケ����Ăяo��
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
		command = console->getCommand(); //�R���\�[������̃R�}���h���擾
		if (command == "")		//�R�}���h���Ȃ��ꍇ
			return;
		if (command == "help")	//�uhelp�v�R�}���h�̏ꍇ
		{
			console->print("Console Commands:");
			console->print("fps - toggle display of frames per second");
			console->print("create - Create window activate.");
			console->print("save - Save object database.");
			return;
		}
		if (command == "fps")
		{
			fpsOn = !fpsOn;		//�t���[�����[�g�̕\����؂�ւ���
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

		if (command == "save") //objectData��ۑ�
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
