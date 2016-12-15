#ifndef _SLOGO_H
#define _SLOGO_H
#include "SpriteObject.h"
namespace SLogoNS{


	const int N_COLS = 1;
	const int WIDTH = 554;
	const int HEIGHT = 256;
}
class SLogo :public SpriteObject {
public:
	SLogo();
	virtual ~SLogo();
	virtual bool initialize();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};

#endif
