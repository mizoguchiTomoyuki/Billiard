#ifndef SCOREPOINT_H
#define SCOREPOINT_H
#include "TextObject.h"
//Score�̉��_�\���p�o�������ď�ɏ����㏸(0.7s)
//������1.2s�b���opacity=0�ɂȂ��Ă��̌�Destroy
class ScorePoint : public TextObject{
protected:
	std::string str;
	float Timer;
	bool up;
public:
	ScorePoint();
	virtual ~ScorePoint();
	virtual bool initialize();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();
	void setString(std::string s){ str = s; }
	void setUp(bool b){ up = b; }
};

#endif
