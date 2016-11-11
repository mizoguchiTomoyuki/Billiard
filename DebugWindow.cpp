#include "DebugWindow.h"
//======================================================================================
//�R���X�g���N�^
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
	dxFont.print("|", textRect, DT_CALCRECT);//�u|�v��S�������������Ƃ��Ďg�p
	rowHeight = textRect.bottom + 2; //��s�̍���(+2�͍s�ԃX�y�[�X)
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
//�R���\�[���̏�����
//======================================================================================
bool DebugWindow::initialize(Graphics *g, Input *in)
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
		vtx[1].x = x + DebugWindowNS::WIDTH;
		vtx[1].y = y;
		vtx[1].z = 0.0f;
		vtx[1].rhw = 1.0f;
		vtx[1].dwColor = backColor;
		//�E��
		vtx[2].x = x + DebugWindowNS::WIDTH;
		vtx[2].y = y + DebugWindowNS::HEIGHT;
		vtx[2].z = 0.0f;
		vtx[2].rhw = 1.0f;
		vtx[2].dwColor = backColor;
		//����
		vtx[3].x = x;
		vtx[3].y = y + DebugWindowNS::HEIGHT;
		vtx[3].z = 0.0f;
		vtx[3].rhw = 1.0f;
		vtx[3].dwColor = backColor;
		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
		//DirectX�t�H���g��������
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
//�R���\�[����`��
//���s�O�FBeginScene��EndScene�̓����ɂ���
//======================================================================================
const void DebugWindow::draw()
{
	//��{�I�ȕ`��
	if (!visible || graphics == NULL || !initialized)
		return;

	if (Activate = true){
		SetActiveColor();
		Activate = false;
	}

	graphics->spriteBegin();
	graphics->drawQuad(vertexBuffer); //�w�i��`��
	if (text.size() == 0)
		return;

	//�R���\�[���Ƀe�L�X�g��\��
	textRect.left = 0;
	textRect.top = 0;
	//textRect�̉��[����s�̍�����
	dxFont.print("|", textRect, DT_CALCRECT);//�u|�v��S�������������Ƃ��Ďg�p
	rowHeight = textRect.bottom + 2; //��s�̍���(+2�͍s�ԃX�y�[�X)
	if (rowHeight <= 0)	//�G���[�悯
		rowHeight = 20;

	rows = (DebugWindowNS::HEIGHT - 2 * DebugWindowNS::MARGIN) / rowHeight;
	rows -= 2; //�����ɁA���̓v�����v�g�p�̃X�y�[�X���󂯂�
	if (rows <= 0) //�����true�ɂȂ�Ȃ��Ƒz�肳���
		rows = 5;	//�@�\����l�������I�ɐݒ�

	//��s�̃e�L�X�g�\��rect��ݒ�
	textRect.left = (long)(x + DebugWindowNS::MARGIN);
	textRect.right = (long)(textRect.right + DebugWindowNS::WIDTH - DebugWindowNS::MARGIN);
	textRect.bottom = (long)(y + DebugWindowNS::HEIGHT - 2 * DebugWindowNS::MARGIN - 2 * rowHeight);
	for (int r = scrollAmount; r < rows + scrollAmount && r < (int)(text.size()); r++)
	{
		//���̍s�̃e�L�X�g�\��rect�̏�[��ݒ�
		textRect.top = textRect.bottom - rowHeight;
		//�e�L�X�g�̈�s��\��
		dxFont.print(text[r], textRect, DT_LEFT);
		//�e�L�X�g�\��rect�̉��[�����̍s�p�ɒ���
		textRect.bottom -= rowHeight;

	}
	//�h���N���X�ŕ`�������`��
	Appenddraw();
	graphics->spriteEnd();	//�X�v���C�g�̕`����I��
}

//======================================================================================
//�R���\�[���̕\���Ɣ�\��
//======================================================================================
void DebugWindow::showHide()
{
	if (!initialized)
		return;
	visible = !visible;
	input->clear(inputNS::KEYS_PRESSED | inputNS::TEXT_IN); //�Â����͂�����
}

//======================================================================================
//�R���\�[���Ƀe�L�X�g��ǉ�
//str�̐擪�s�̃e�L�X�g�̂ݕ\�������
//======================================================================================
void DebugWindow::print(const std::string &str) //�R���\�[���Ƀe�L�X�g��ǉ�
{
	if (!initialized)
		return;
	text.push_front(str); //str���A�f�b�Ntext�ɒǉ�
	if (text.size() > DebugWindowNS::MAX_LINE)
		text.pop_back(); //�ł��Â��s���폜
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
//�w�i�̎l�p�`�̐F��ύX����Active��Active������
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

//����K���string�𕪉����ăR���\�[���ɕ\������
//�Ƃ肠����";"�ŉ��s����.
//y�͕`��J�n�ʒu
//�Ō�ɍs����Ԃ�
int DebugWindow::Decode(std::string s, int y){
	int rowNum = 0;
	std::string substr = "";
	int index = 0;
	int next_index = s.find(";", index);

	while (next_index != std::string::npos){
		rowNum++;

		textRect.bottom = (long)((y + rowNum)*rowHeight + y + rowHeight);
		textRect.top = textRect.bottom - rowHeight;
		substr = s.substr(index, next_index-index); //CreateMenu�Ɍ�����Menu
		dxFont.print(substr, textRect, DT_LEFT);
		index = next_index+1;
		next_index = s.find(";", index);

	}
	return rowNum;
}