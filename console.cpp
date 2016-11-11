#include "console.h"
//======================================================================================
//�R���X�g���N�^
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
//�R���\�[���̏�����
//======================================================================================
bool Console::initialize(Graphics *g, Input *in)
{
	try{
		graphics = g;  //�O���t�B�b�N�f�o�C�X
		input = in;
		//����
		vtx[0].x = x;
		vtx[0].y = y;
		vtx[0].z = 0.0f;
		vtx[0].rhw = 1.0f;
		vtx[0].dwColor = backColor;
		//�E��
		vtx[1].x = x + consoleNS::WIDTH;
		vtx[1].y = y;
		vtx[1].z = 0.0f;
		vtx[1].rhw = 1.0f;
		vtx[1].dwColor = backColor;
		//�E��
		vtx[2].x = x + consoleNS::WIDTH;
		vtx[2].y = y + consoleNS::HEIGHT;
		vtx[2].z = 0.0f;
		vtx[2].rhw = 1.0f;
		vtx[2].dwColor = backColor;
		//����
		vtx[3].x = x;
		vtx[3].y = y + consoleNS::HEIGHT;
		vtx[3].z = 0.0f;
		vtx[3].rhw = 1.0f;
		vtx[3].dwColor = backColor;
		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
		//DirectX�t�H���g��������
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
//�R���\�[����`��
//���s�O�FBeginScene��EndScene�̓����ɂ���
//======================================================================================
const void Console::draw()
{
	if (!visible || graphics == NULL || !initialized)
		return;

	DebugWindow::draw();
}

const void Console::Appenddraw(){
	if (rowHeight <= 0)	//�G���[�悯
		rowHeight = 20;

	textRect.bottom = (long)(y + consoleNS::HEIGHT - consoleNS::MARGIN);
	textRect.top = textRect.bottom - rowHeight;
	std::string prompt = ">";		//�v�����v�g���������쐬
	if (Active){
		prompt = prompt + input->getTextIn();
	}
	dxFont.print(prompt, textRect, DT_LEFT); //�v�����v�g�ƃR�}���h��\��
}
//======================================================================================
//�R���\�[���R�}���h��߂�
//�V���O���L�[�̃R���\�[���R�}���h����������
//���̑��̃R�}���h�͂��ׂăQ�[���ɖ߂�
//======================================================================================
std::string Console::getCommand()
{
	if (!Active)
		return "";
	//�R���\�[��������������Ă��Ȃ��ꍇ�A�܂��͕\������Ă��Ȃ��ꍇ
	if (!initialized || !visible)
		return "";
	//�R���\�[���L�[���`�F�b�N
	if (input->wasKeyPressed(CONSOLE_KEY))
		hide();
	//Exc�L�[���`�F�b�N
	if (input->wasKeyPressed(ESC_KEY))
		return "";
	//�X�N���[�����`�F�b�N
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
	commandStr = input->getTextIn();	//���[�U�[�����͂����e�L�X�g���擾
	//�L�[���Q�[���ɓn���Ȃ�
	input->clear(inputNS::KEYS_DOWN | inputNS::KEYS_PRESSED | inputNS::MOUSE);
	if (commandStr.length() == 0)	//�R�}���h�����͂���Ă��Ȃ��ꍇ
		return "";
	//Enter�L�[��������Ă��Ȃ��ꍇ
	if (commandStr.at(commandStr.length() - 1) != '\r')
		return "";					//�߂�(�R�}���h�łȂ�)
	//�R�}���h�̖�������u\r�v������
	commandStr.erase(commandStr.length() - 1);
	input->clearTextIn();	//���͍s���N���A
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
