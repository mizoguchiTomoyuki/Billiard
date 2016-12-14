#ifndef TEXT_H
#define TEXT_H
#define WIN32_LEAN_AND_MEAN
#include <string>
#include "Sprite.h"
#include "constants.h"
namespace textNS{
	enum Alignment { LEFT, CENTER, RIGHT, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM };
	struct FontData
	{
		UINT left;
		UINT right;
	};
	const int FONT_BORDER =1; //�e�����̎��̘͂g���̃T�C�Y,�����Șg2�s�N�Z�����O���b�h�̘g������3�s�N�Z��
	const int FONT_WIDTH = 31; //������
	const int FONT_HEIGHT = 50;// �����̍���
	const int GRID_WIDTH = FONT_WIDTH + FONT_BORDER; //��������̃O���b�h�̉���
	const int GRID_HEIGHT = FONT_HEIGHT + FONT_BORDER; //��������̃O���b�h�̍�����
	const int COLUMNS = 4;	//�t�H���g�摜���̗�
	const int ROWS = 5; //�t�H���g�摜���̍s��
	const int FRAMES = 1; //�e�����̃A�j���[�V�����̃t���[����(1 = �A�j���[�V�����Ȃ�)
	const double ANIM_DELAY = 0.0; //�A�j���[�V�����t���[���Ԃ̎��ԊԊu
	const int MAX_FONT_HEIGHT = 1000; //�t�H���g�̍ő�̍���
	const int MIN_CHAR = 0x0029; //������Ɉʒu���镶���̕����R�[�h�B(' ')��������MAX_CHAR�܂�
	const int MAX_CHAR = 0x0039; //���E���Ɉʒu���镶���̕����R�[�h
	const int PROPORTIONAL_SPACING = 5; //�v���|�[�V���i���X�y�[�X�ŕ\�����镶���Ԃ̃X�y�[�X�̍L��
	const int TAB_SIZE = 8; //�����ʒu�ɂ���Ă���킳���^�u�ʒu�̃T�C�Y
	const char UNDERLINE = '_';	//������\������ۂɎg���镶��ASCII�l��0x5F(����̓e�N�X�`���̃O���b�h�ƑΉ�)
	const char SOLID = 0x7F; //�P�F�œh��Ԃ����u���b�N��\�����邽�߂Ɏg�p����镶��0x7F�͍����(MIN_CHAR)����̕����̌��ɑ���������
	const int BOLD_SIZE = 4; //���������ɂ��炵�ĕ\��
}
class Text : public Sprite{
private:
	TextureManager fontTexture;
	LP_TEXTURE textureData;
	char *file;
	Graphics *graphics;
	textNS::FontData fontData[textNS::ROWS][textNS::COLUMNS];
	textNS::Alignment align;
	UINT width, height;
	UINT fontHeight;
	UINT tabSize;
	COLOR_ARGB color;
	COLOR_ARGB backColor;
	UINT proportionalSpacing;
	bool proportional;

public:
	Text();
	virtual ~Text();
	virtual void setXY(int x2, int y2);
	virtual void print(const std::string &str);
	virtual void print(const std::string &str, int x, int y);
	virtual void print(const std::string & str, int x, int y, textNS::Alignment align);
	virtual bool initialize(Graphics *g);
	virtual void doAlign(const std::string &str);
	virtual void drawChar(UCHAR ch);
	virtual void onResetDevice();
	virtual void onLostDevice();
	//Set Function
	virtual void setFontHeight(UINT height)
	{
		if (height == 0 || height > textNS::MAX_FONT_HEIGHT)
			return;
		fontHeight = height;
		spriteData.scale = (float)height / (float)textNS::FONT_HEIGHT;
	}
	virtual void setFontColor(COLOR_ARGB c){ color = c; }
	virtual void setBackColor(COLOR_ARGB bc){ backColor = bc; }
	virtual void setProportional(bool p){ proportional = p; }
	virtual void setProportionalSpacing(UINT s){ proportionalSpacing = s; }
	virtual void setTabSize(UINT size)
	{
		if (size == 0)
			return;
		tabSize = size;
	}

	//getFunction
	virtual UINT getFontHeight(){ return height; }
	virtual COLOR_ARGB getFontColor(){ return color; }
	virtual COLOR_ARGB getBackColor(){ return backColor; }
	virtual void getWidthHeight(const std::string &str, UINT &w, UINT &h);
	virtual bool getProportional(){ return proportional; }
	virtual UINT getProportionalSpacing(){ return proportionalSpacing; }
	virtual UINT getTabSize(){ return tabSize; }

};

#endif