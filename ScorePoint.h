#ifndef SCOREPOINT_H
#define SCOREPOINT_H
#include "TextObject.h"
//Scoreの加点表示用出現させて上に少し上昇(0.7s)
//次いで1.2s秒後にopacity=0になってその後Destroy
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
