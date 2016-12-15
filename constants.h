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
#define TRANSCOLOR  SETCOLOR_ARGB(255,0,0,0)  // 透明色

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

//-----------------------------------------------
//                  Constants
//-----------------------------------------------
// window
const char CLASS_NAME[] = "WinMain";
const char GAME_TITLE[] = "Billiards";

inline int SIGN(float a){ return ((a > 0) - (a < 0)); }

const char ARROW_IMAGE[] = "pictures\\arrow.png";
const char SHADOW_IMAGE[] = "pictures\\shadow.png";
const char RING_IMAGE[] = "pictures\\ring.png";
const char LOGO_IMAGE[] = "pictures\\logo.png";
const char ISHOOT_IMAGE[] = "pictures\\infoshoot.png";
const char ITARGET_IMAGE[] = "pictures\\infotarget.png";
const char ISETUP_IMAGE[] = "pictures\\infosetUp.png";
const char GAMEOVER_IMAGE[] = "pictures\\Gameover.png";
const char ISCORE_IMAGE[] = "pictures\\infoScore.png";
const char FONT_IMAGE[] = "pictures\\font3251.png"; 
const char PRIMITIVE_CUBE[] = "XData\\primitiveCube.x";
const char BILLTABLE_CUBE[] = "XData\\billiardtable.x";
const char BILLMAT_CUBE[] = "XData\\billiardmat.x";
const char PRIMITIVE_SPHERE[] = "XData\\primitiveSphere.x";
const char ONEBALL_MESH[] = "XData\\one.x";
const char TWOBALL_MESH[] = "XData\\two.x";
const char THREEBALL_MESH[] = "XData\\three.x";
const char FOURBALL_MESH[] = "XData\\four.x";
const char FIVEBALL_MESH[] = "XData\\five.x";
const char SIXBALL_MESH[] = "XData\\six.x";
const char SEVENBALL_MESH[] = "XData\\seven.x";
const char EIGHTBALL_MESH[] = "XData\\eight.x";
const char NINEBALL_MESH[] = "XData\\nine.x";
const char BALL_MESH[] = "XData\\myball.x";
const char SHADOW_MESH[] = "XData\\shadow.x";
const char NINEBALL_TEXTURE[] = "XData";
const char CUBE_MESH[] = "XData\\Cube2.x";
const char WAVE_BANK[] = "audio\\Win\\WaveBank.xwb";
const char SOUND_BANK[] = "audio\\Win\\SoundBank.xsb";

const char SE_CHARGE[] = "B_Charge";
const char SE_RESTART[] = "B_Restart";
const char SE_POINTDOWN[] = "B_PointDown";
const char SE_POINT[] = "B_point";
const char SE_SHOOT[] = "B_Shoot";
const char SE_SPOT[] = "B_Spot";
const char SE_TARGET[] = "B_Target";
const char SE_REFRECT[] = "B_Refrect";

const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT  GAME_WIDTH = 1280;              // width of game in pixels
const UINT  GAME_HEIGHT = 960;              // height of game in pixels

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
	BALL_1 = 5,
	BALL_2 = 6,
	BALL_3 = 7,
	BALL_4 = 8,
	BALL_5 = 9,
	BALL_6 = 10,
	BALL_7 = 11,
	BALL_8 = 12,
	BALL_9 = 13,
	BILLIARDTABLE = 14,
	DARKHOLE = 15,
	EXIT,
};


#endif