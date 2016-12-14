#ifndef _SSHADOW_H
#define _SSHADOW_H
#include "SpriteObject.h"
namespace SShadowNS{


	const int N_COLS = 1;
	const int WIDTH = 256;
	const int HEIGHT = 128;
}
class SShadow :public SpriteObject {
public:
	SShadow();
	~SShadow();
	virtual bool initialize();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};

#endif
