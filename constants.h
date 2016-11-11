#ifndef _CONSTANTS_H 
#define _CONSTANTS_H  
#define WIN32_LEAN_AND_MEAN
//さまざまな定義を行っておく
#include <windows.h>
//-----------------------------------------------
// Useful macros
//-----------------------------------------------

#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }

#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }

#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }

#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }

#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

//-----------------------------------------------
//                  Constants
//-----------------------------------------------
// window
const char CLASS_NAME[] = "WinMain";
const char GAME_TITLE[] = "Billiards";

inline int SIGN(float a){ return ((a > 0) - (a < 0)); }

const char PRIMITIVE_CUBE[] = "XData\\primitiveCube.x";
const char PRIMITIVE_SPHERE[] = "XData\\primitiveSphere.x";
const char BALL_MESH[] = "XData\\ball.x";
const char NINEBALL_TEXTURE[] = "XData";
const char CUBE_MESH[] = "XData\\Cube2.x";

const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT  GAME_WIDTH = 640;              // width of game in pixels
const UINT  GAME_HEIGHT = 480;              // height of game in pixels

const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;               // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE; // maximum time used in calculations

const UCHAR CONSOLE_KEY = VK_OEM_3;
const UCHAR ESC_KEY = VK_ESCAPE;
const UCHAR ALT_KEY = VK_MENU;
const UCHAR ENTER_KEY = VK_RETURN;
const UCHAR LEFT_KEY = VK_LEFT;
const UCHAR RIGHT_KEY = VK_RIGHT;
const UCHAR UP_KEY = VK_UP;
const UCHAR DOWN_KEY = VK_DOWN;
typedef int ELEMID;	// 要素ID
typedef size_t OBJID;	// オブジェクトID


const enum PREFAB_MENU{
	BALL = 0,
	CAMERA = 1,
	DIRECTIONALLIGHT = 2,
	POINTLIGHT = 3,
	BOX = 4,
	EXIT,
};


#endif