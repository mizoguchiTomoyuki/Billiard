#ifndef SSCORE_H
#define SSCORE_H
#include "TextObject.h"

class SScore : public TextObject{
protected:
	std::string str;
public:
	SScore();
	virtual ~SScore();
	virtual bool initialize();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();
	void setString(std::string s){ str = s; }
};

#endif