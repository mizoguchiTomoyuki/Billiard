#ifndef _SARROW_H
#define _SARROW_H
#include "SpriteObject.h"
namespace SArrowNS{


	const int N_COLS = 1;
	const int WIDTH = 256;
	const int HEIGHT = 128;
}
class SArrow:public SpriteObject {
protected:
public:
	SArrow();
	virtual ~SArrow();
	virtual bool initialize();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();
	void PowerColor(float); //力のため具合で色を変化させる(できたら移動拡大なども)

};

#endif
