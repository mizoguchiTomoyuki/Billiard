#ifndef _TEXTOBJECT_H
#define _TEXTOBJECT_H
#include "SpriteObject.h"
#include "Text.h"
class TextObject : public SpriteObject{
protected:

	Text _text; //�e�L�X�g�\���p

public:
	TextObject();
	virtual ~TextObject();
	virtual bool initialize();
	virtual void update(){};
	virtual void draw(){};
	virtual void ai(){};
	virtual void collision(){};

};

#endif