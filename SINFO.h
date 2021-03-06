#ifndef _SINFO_H
#define _SINFO_H
#include "SpriteObject.h"
namespace SInfoNS{


	const int N_COLS = 1;
}
class SInfo :public SpriteObject {
protected:
	bool start;
	float Counter;
public:
	SInfo();
	virtual ~SInfo();
	virtual bool initialize(std::string str, int width, int height);//Info関係は処理が同じものが多いのでクラスを共通化
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();
	void setStart();
	void Unset();

};

#endif
