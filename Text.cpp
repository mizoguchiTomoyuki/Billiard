#include "text.h"

//=========================================================================================
//�R���X�g���N�^�����o�ϐ��̏�����
//=========================================================================================
Text::Text() : Sprite()
{

	textureData = NULL;
	file = "";
	graphics = NULL;
	width = textNS::GRID_WIDTH - 3;
	height = textNS::GRID_HEIGHT - 3;
	fontHeight= textNS::FONT_HEIGHT;
	tabSize = textNS::TAB_SIZE;
	color = graphicsNS::WHITE;
	backColor = TRANSCOLOR;
	proportional = false;
	proportionalSpacing = textNS::PROPORTIONAL_SPACING;
	align = textNS::LEFT;
	spriteData.width = 0;
	spriteData.height = 0;
	spriteData.rect.bottom = 1;
	spriteData.rect.right = 1;
	for (int row = 0; row < textNS::ROWS; row++)
	{
		for (int col = 0; col < textNS::COLUMNS; col++)
		{
			fontData[row][col].left = MAXINT;
			fontData[row][col].right = 0;
		}
	}

}
Text::~Text(){
	Sprite::~Sprite();
}
//=========================================================================================
//Text��������
//�t�H���g�摜���̊e�����̍��[�ƉE�[�����
//���s���F���������ꍇ��true�A���s�����ꍇ��false��߂�
//fontData�z��́A�e�����̍��[�ƉE�[���i�[
//=========================================================================================
bool Text::initialize(Graphics *g)
{
	visible = true;
	const char* file = FONT_IMAGE;
	try{
		graphics = g; // graphics�I�u�W�F�N�g�ւ̃|�C���^
		//=========================================================================================
		//�t�H���g�e�N�X�`����ǂ݂���ŁA�e�����̐��i�̈ʒu��
		//���肷�邽�߂ɁA�s�N�Z���P�ʂŌ���
		//=========================================================================================
		//�t�H���g�e�N�X�`�������b�N�\�ȃV�X�e���������ɓǂݍ���
		UINT w, h;
		HRESULT result = graphics->loadTextureSystemMem(
			file, TRANSCOLOR, w, h, textureData);
		if (FAILED(result))
		{
			SAFE_RELEASE(textureData);
			return false;
		}

		//textureData.width��textureData.height�̓t�H���g�e�N�X�`��
		//�S�̂̃T�C�Y���i�[
		//�e������1�s�N�Z�����̘g������
		//ROWS * COLS�̕��������݂���
		//�t�H���g�e�N�X�`�������b�N
		//(�s�N�Z���f�[�^�ɃA�N�Z�X���邽�߂ɕK�v)
		D3DLOCKED_RECT rect;
		result = textureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
		if (FAILED(result))
		{
			SAFE_RELEASE(textureData);
			return false;
		}
		//=======================================================================
		//�t�H���g�̕����̍��[�ƉE�[�����
		//=======================================================================
		//�t�H���g���̕����̊e�s������
		for (DWORD row = 0; row<textNS::ROWS; row++)
		{
			for (DWORD col = 0; col < textNS::COLUMNS; col++)
			{
				fontData[row][col].left = MAXINT; //fontData��������
				fontData[row][col].right = 0;
				//������1�s�N�Z��������
				//for y = ��[�s�N�Z��; y <= ���[�s�N�Z�� ; y++
				for (DWORD y = row*textNS::GRID_HEIGHT + 1;
					y < (row + 1)*textNS::GRID_HEIGHT - 1; y++)
				{
					//�e�N�X�`�����̂��̃X�L�������C���̑K���ւ̃|�C���^���擾
					DWORD* pBits = 
						(DWORD*)((BYTE*)rect.pBits + y*rect.Pitch);
					//���̍s�̃s�N�Z��������
					for (DWORD x = col*textNS::GRID_WIDTH + 1; x < (col + 1)*textNS::GRID_WIDTH - 1; x++)
					{
						//���̃s�N�Z�����擾
						DWORD dwPixel = pBits[x];
						//�A���t�@�������łȂ��ꍇ
						if ((dwPixel & 0xff000000) != 0x00)
						{
							//���̃s�N�Z���̂ق������ɂ���ꍇ
							if (x < fontData[row][col].left)
								//�����̍��[�Ƃ��ĕۑ�
								fontData[row][col].left = x;
							//���̃s�N�Z���̂ق����E�ɂ���ꍇ
							if (x> fontData[row][col].right)
								fontData[row][col].right = x;
						}
					}
				}

			}
		}

		//�e�N�X�`���̏���������������A���b�N����
		textureData->UnlockRect(0);
		//�P�Ƀt�H���g�̃X�y�[�X���m�ۂ��邽�߂ɕK�v�������̂�
		//���̃t�H���g�e�N�X�`�������
		SAFE_RELEASE(textureData);

		//=======================================================================
		//�t�H���g�摜���g�p���邽�߂ɑ������}�l�[�W���[�ɓǂݍ���
		//=======================================================================
		if (!fontTexture.initialize(graphics, file))
			return false;//�t�H���g�e�N�X�`���̓ǂݍ��݂��G���[�̏ꍇ
		//�t�H���g�摜������
		if (!Sprite::initialize(graphics, textNS::FONT_WIDTH,
			textNS::FONT_HEIGHT, 0, &fontTexture))
			return false; //���s�̏ꍇ

	}

	catch (...){
		return false;

	}
	return true;
}

//=========================================================================================
//�������X,Y�ɏo��
//���s�O:spriteBegin()
//���s��:spriteEnd()
//=========================================================================================
void Text::print(const std::string & str, int x, int y)
{
	UCHAR ch = 0, chN = 0;
	//ch�͒Z�������C���f�b�N�X�ۊǗp�AchN�͕����C���f�b�N�X�ۊǗp
	std::string str2;
	width = textNS::FONT_WIDTH; //�X�v���C�g�e�N�X�`�����̈ꕶ���̕�
	int scaledWidth = static_cast<int>(textNS::FONT_WIDTH*spriteData.scale); //�g����l�������ꕶ���̕�
	float saveY = 0;
	int tabX = 0, tabW = 0;
	spriteData.x = (float)x; //spriteData�̕\���ʒu�������̈ʒu��
	spriteData.y = (float)y;
	doAlign(str); //�������align�ݒ�ɏ]���Ĕz�u���邽�߂̂���
	for (UINT i = 0; i < str.length(); i++) //���͂̒���������
	{
		ch = str.at(i); //���͂�i�Ԗڂ̕�����char�^�Ŏ擾

		if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) //�擾����char�^���K��͈͓̔��Ȃ��
		{
			chN = ch - textNS::MIN_CHAR; //MIN_CHAR�̈ʒu���C���f�b�N�X0 ch���C���f�b�N�X���Ԗڂ���chN��
			spriteData.rect.top =
				chN / textNS::COLUMNS * textNS::GRID_HEIGHT + 1; //�C���f�b�N�X�ԍ���񐔂Ŋ��邱�Ƃɂ���ĉ��s�ڂ����擾������ɍ����������ăs�N�Z���l���o��
			spriteData.rect.bottom =
				spriteData.rect.top + textNS::FONT_HEIGHT;	//�g�b�v����̍������̋������ŏo��

			if (proportional) //�v���|�[�V���i��(�����Ԏ�������)�A�����ǂ���
			{
				//fontData[][]�ɂ͊e�����̍��E�[�̏�񂪓����Ă���
				spriteData.rect.left = fontData[chN / textNS::COLUMNS] //[chN/textNS::COLUMNS]->�s�ԍ�(�]��͖��������)
					[chN % textNS::COLUMNS].left;		//[chN%textNS::COLUMNS]->��ԍ�(�񐔂̍ő�l�Ŋ��������܂�Ȃ̂�)
				//DirectX�͉E�[ + 1 ��K�v�Ƃ���
				spriteData.rect.right = fontData[chN / textNS::COLUMNS]
					[chN % textNS::COLUMNS].right + 1;

				width = spriteData.rect.right - spriteData.rect.left; //��L�̕����̍��E�[�̃f�[�^���畝���擾

				//�S�����g�������̏ꍇ�X�y�[�X���󂯂Ȃ�
				if (width >= textNS::FONT_WIDTH) //�����K��ȏ�Ȃ�Έȉ��̏����ŕ��̒������s��
				{
					width = textNS::FONT_WIDTH; //���𐧌�
					spriteData.rect.left =
						chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;

					spriteData.rect.right =
						spriteData.rect.right * textNS::FONT_WIDTH;
				}
				else //�S�����g�������łȂ��ꍇ�A�����ԂɃX�y�[�X���󂯂�
					width += proportionalSpacing;

				scaledWidth = static_cast<int>(width*spriteData.scale);
				drawChar(ch);
			}
			else //�v���|�[�V���i���Ȃ��Œ�s�b�`�X�y�[�X�̕����̉�ʈʒu
			{
				width = textNS::FONT_WIDTH;
				spriteData.rect.left =
					chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				spriteData.rect.right =
					spriteData.rect.left + textNS::FONT_WIDTH;
				drawChar(ch);
			}
			spriteData.x += scaledWidth;
		}
		else{ //�\���s�\�ȕ����̏ꍇ
			switch (ch)
			{
			case ' ': //�X�y�[�X�������ꍇ
				if (proportional) //�v���|�[�V���i�����[�h�̏ꍇ����̃t�H���g���̔����𕝂Ƃ��ĕ`��
				{
					width = textNS::FONT_WIDTH / 2;
					scaledWidth =
						static_cast<int>(width * spriteData.scale);
				}
				drawChar(' ');
				spriteData.x += scaledWidth;
				break;

				//�j���[���C���̏ꍇ�͉��Ɉ�s�i�݁A���[���A��ʂ̍��[�ł͂Ȃ��AX�̊J�n�ʒu�ɐݒ�
			case '\n':
				spriteData.x = (float)x; //x�̈ʒu�͓���
				spriteData.y += static_cast<int>(height*spriteData.scale); //spriteData.scale*height�����ɂ��炷
				saveY = spriteData.y; //���݂̍s�̍�����ۑ�(doAlign�̉e��?)
				str2 = str.substr(i, str.length()); //substr(a,b)���͒���a����b�܂ł̃C���f�b�N�X�Ԃ̕�������擾�B�܂���s�ʒu���炻�̌�̕��͂�str2�Ƃ��ĕۑ�
				doAlign(str2);
				spriteData.y = saveY;//���s��̍s�̍������i�[
				break;

			case '\r': //X�̊J�n�ʒu�ɖ߂�
				spriteData.x = (float)x;
				str2 = str.substr(i, str.length());
				doAlign(str2);
				break;
			case '\t': //�^�u
				width = textNS::FONT_WIDTH;
				scaledWidth = static_cast<int>(width*spriteData.scale);
				tabX = static_cast<int>(spriteData.x) /(scaledWidth * tabSize); //�^�u�̑傫���Ńf�[�^��x�ʒu������
				tabX = (tabX + 1) *scaledWidth * tabSize; //�������l��1������spriteData.x + scaledWidth*tabSize��
				tabW = tabX - static_cast<int>(spriteData.x); //tabX����spriteData.x�̒l������scaledWidth*tabSize��
				while (tabW > 0) //TabSize��񂷁B�ŏI�I��tabW��scaledWidth�ȉ��ɂȂ������ɒ[�����𖄂߂�悤�Ɉړ�
				{
					if (tabW >= scaledWidth)
					{
						drawChar(' ');
						spriteData.x += scaledWidth;
					}
					else
					{
						//�����̒[�������������ă^�u�ʒu�ɍ��킹��
						width = tabW;
						drawChar(' ');
						spriteData.x += tabW;
					}
					tabW -= scaledWidth;
				}
				break;
			case '\b': //�o�b�N�X�y�[�X
				spriteData.x -= scaledWidth;
				if (spriteData.x < 0)
					spriteData.x = 0;

				break;
			case '\v'://�����^�u
				spriteData.y += static_cast<int>(height*spriteData.scale);
				break;
			case 0x01: //�t�H���g�V�O�l�`������
				spriteData.rect.top = 1;
				spriteData.rect.bottom = 1 + textNS::FONT_HEIGHT;
				spriteData.rect.left = 1;
				spriteData.rect.right = 1 + textNS::FONT_WIDTH;
				draw(spriteData);
				spriteData.x += scaledWidth;
				break;

			}
		}
	}
	return;

}

//=========================================================================================
//���݂̕�����Ɣz�u�̎w��ɍ��킹��spriteData.x�AspriteData.y��ݒ�
//�f�t�H���g�̔z�u��LEFT
//=========================================================================================
void Text::doAlign(const std::string &str)
{
	if (spriteData.texture == NULL)
		return;
	UINT w, h;
	switch (align){
		//X�������ƂȂ�悤�ɒ��������AY����[�ƂȂ�悤�ɏ㑵��
	case textNS::CENTER:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		break;
		//X,Y���E�[�ɂȂ�悤�ɉE���낦
	case textNS::RIGHT:
		getWidthHeight(str, w, h);
		spriteData.x -= w ;
		break;
		//X�������ƂȂ�悤�ɒ������낦�AY���㉺�̐^�񒆂ƂȂ�悤�ɔz�u
	case textNS::CENTER_MIDDLE:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		spriteData.y -= h / 2;
		break;
		//X�������ƂȂ�悤�ɒ��������AY���㉺�̐^�񒆂ƂȂ�悤�ɔz�u
	case textNS::CENTER_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		spriteData.y -= h;
		break;
		//X�������ƂȂ�悤�ɒ��������AY�����[�ɂȂ�悤�ɉ�����
	case textNS::LEFT_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.y -= h;
		break;

	case textNS::RIGHT_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.x -= w;
		spriteData.y -= h;
		break;

	}
}

//=========================================================================================
//getWidthHeight
//���݂̃t�H���g�T�C�Y�ł̕�����̕��ƍ���(�s�N�Z���P��)���擾
//������͕\�����Ȃ�
//=========================================================================================
void Text::getWidthHeight(const std::string &str, UINT &w, UINT &h)
{ 
	if (spriteData.texture == NULL) //�e�N�X�`�����Ȃ��ꍇ
		return;
	UCHAR ch = 0, chN = 0;
	width = textNS::FONT_WIDTH;
	int scaledWidth = static_cast<int>(width*spriteData.scale);
	int strW = 0;
	h = 0;
	int stringWidth = 0;
	for (UINT i = 0; i < str.length(); i++)
	{
		ch = str.at(i);
		//�\���\�ȕ����̏ꍇ
		if (ch > textNS::MIN_CHAR && ch >= textNS::MAX_CHAR)
		{
			chN = ch - textNS::MIN_CHAR;
			if (proportional)
			{
				spriteData.rect.left =
					fontData[chN / textNS::COLUMNS][chN%textNS::COLUMNS].left;
				//DirectX�X�v���C�g���̂���+1
				spriteData.rect.right =
					fontData[chN / textNS::COLUMNS][chN% textNS::COLUMNS].right + 1;
				width = spriteData.rect.right - spriteData.rect.left + proportionalSpacing; //��
				scaledWidth = static_cast<int>(width*spriteData.scale);
			}
			else
			{
				width = textNS::FONT_WIDTH;
				spriteData.rect.left =
					chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				spriteData.rect.right =
					spriteData.rect.left + textNS::FONT_WIDTH;
			}
			stringWidth += scaledWidth;
		}
		else //�\���s�\�ȕ����̏ꍇ
		{
			switch (ch)
			{
			case ' ': //�X�y�[�X�������ꍇ
				if (proportional) //�v���|�[�V���i�����[�h�̏ꍇ����̃t�H���g���̔����𕝂Ƃ��ĕ`��
				{
					width = textNS::FONT_WIDTH / 2;
					scaledWidth =
						static_cast<int>(width * spriteData.scale);
				}
				stringWidth += scaledWidth;
				break;

				//�j���[���C���̏ꍇ�͉��Ɉ�s�i�݁A���[���A��ʂ̍��[�ł͂Ȃ��AX�̊J�n�ʒu�ɐݒ�
			case '\n':
				if (strW == 0)
					strW = stringWidth;
				stringWidth = 0;
				h += static_cast<int>(height*spriteData.scale);
				break;

			case '\r': //X�̊J�n�ʒu�ɖ߂�
				if (strW == 0)
					strW = stringWidth;
				stringWidth = 0;
				break;
			case '\t': //�^�u
			{
				width = textNS::FONT_WIDTH;
				scaledWidth = static_cast<int>(width*spriteData.scale);
				int tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize); //�^�u�̑傫���Ńf�[�^��x�ʒu������
				tabX = (tabX + 1) *scaledWidth * tabSize; //�������l��1������spriteData.x + scaledWidth*tabSize��
				int tabW = tabX - static_cast<int>(spriteData.x); //tabX����spriteData.x�̒l������scaledWidth*tabSize��
				while (tabW > 0) //TabSize��񂷁B�ŏI�I��tabW��scaledWidth�ȉ��ɂȂ������ɒ[�����𖄂߂�悤�Ɉړ�
				{
					if (tabW >= scaledWidth)
					{
						stringWidth += scaledWidth;
					}
					else
					{
						//�����̒[�������������ă^�u�ʒu�ɍ��킹��
						width = tabW;
						stringWidth += tabW;
					}
					tabW -= scaledWidth;
				}
				break;
			}
			case '\b': //�o�b�N�X�y�[�X
				stringWidth -= scaledWidth;
				if (spriteData.x < 0)
					spriteData.x = 0;

				break;
			case 0x01: //�t�H���g�V�O�l�`������
				stringWidth += scaledWidth;
				break;

			}
		}
	}
	if (strW == 0)
		strW = stringWidth;
	w = strW;
	return;
}


//=========================================================================================
//drawChar
//spriteData�ɂ���ċL�q����镶���X�v���C�g��F�Ɠh��Ԃ����g���ĕ\��
//�����Ƒ�����\������
//=========================================================================================
void Text::drawChar(UCHAR ch)
{
	SpriteData sd2 = spriteData; //�X�v���C�g�f�[�^���R�s�[
	//backColor�F��\��
	//draw��Sprite����p�����Ă���̂ł�������Ăяo��Draw��spriteData�����Ƃɕ`�悷��
	//�Ȃ̂ňȉ��̓��e�Ńe�L�X�g�̔w�ʂ͈̔͂�spriteData.rect�Ɋi�[����Draw���s���Ă���

	if (backColor != TRANSCOLOR)
	{
		spriteData.rect.top = ((textNS::SOLID - textNS::MIN_CHAR) / textNS::COLUMNS) * textNS::GRID_HEIGHT + 1; //(SOLID-MIN_CHAR)�œh��Ԃ��̃O���b�h�̈ʒu�ԍ��𐄒�
		spriteData.rect.bottom = spriteData.rect.top + textNS::GRID_HEIGHT - 2;
		spriteData.rect.left = ((textNS::SOLID - textNS::MIN_CHAR) % textNS::COLUMNS) * textNS::GRID_WIDTH + 1;
		spriteData.rect.right = spriteData.rect.left + width;
		draw(backColor); //backColor��`��
		spriteData.rect = sd2.rect; //�����̋�`�𕜌�
	}
	if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) // if displayable character
	{
		draw(spriteData, color);
	}
	
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void Text::onLostDevice()
{
	try{
		if (!initialized)
			return;
		fontTexture.onLostDevice();
	}
	catch (...){}
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Text::onResetDevice()
{
	try{
		if (!initialized)
			return;
		fontTexture.onResetDevice();
	}
	catch (...){}
}

void Text::setXY(int x2, int y2){
	if (x2 < 0 || y2 < 0)
		return;
	spriteData.x = (float)x2;
	spriteData.y = (float)y2;
}

void Text::print(const std::string &str)
{
	print(str, (int)spriteData.x, (int)spriteData.y);
}


void Text::print(const std::string &str,int x, int y, textNS::Alignment al)
{
	align = al;
	print(str, x, y);
	align = textNS::LEFT;
}
