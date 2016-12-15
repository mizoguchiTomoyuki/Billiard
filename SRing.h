#ifndef _SRING_H
#define _SRING_H
#include "SpriteObject.h"
namespace SRingNS{


	const int N_COLS = 1;
	const int WIDTH = 256;
	const int HEIGHT = 256;
}
class SRing :public SpriteObject {
public:
	SRing();
	virtual ~SRing();
	virtual bool initialize();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};

#endif
