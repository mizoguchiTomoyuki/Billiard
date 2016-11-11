#ifndef _MOUSE_EXTEND_H
#define _MOUSE_EXTEND_H
#include "input.h"
#include "GameSceneManager.h"
#include "Task.h"
namespace MouseExtendNS{
	enum M_BUTTONTYPE{
		M_LEFT = 0,
		M_RIGHT = 1,
		M_MID = 2,
		END
	};
}
class MouseExtend : public Task{
protected:
	bool is_Drag[MouseExtendNS::END]; //ç∂É{É^ÉìâüÇµÇ¡ÇœÇ»ÇµèÛë‘Ç©
	int TrackStart_x[MouseExtendNS::END];
	int TrackStart_y[MouseExtendNS::END];
	int DragDistance_x[MouseExtendNS::END];
	int DragDistance_y[MouseExtendNS::END];

	int StampPoint_x[MouseExtendNS::END];
	int StampPoint_y[MouseExtendNS::END];
	int Div_x[MouseExtendNS::END];
	int Div_y[MouseExtendNS::END];

	float Time_MouseClick[MouseExtendNS::END];

public:
	MouseExtend();
	~MouseExtend();
	virtual void start(gameObject* t);
	virtual void update();
	int getDragDistance_X(MouseExtendNS::M_BUTTONTYPE type){ return DragDistance_x[(int)type]; };
	int getDragDistance_Y(MouseExtendNS::M_BUTTONTYPE type){ return DragDistance_y[(int)type]; };

	int getDragStart_X(MouseExtendNS::M_BUTTONTYPE type){ return TrackStart_x[(int)type]; };
	int getDragStart_Y(MouseExtendNS::M_BUTTONTYPE type){ return TrackStart_y[(int)type]; };

	float getTime_MouseClick(MouseExtendNS::M_BUTTONTYPE type){ return Time_MouseClick[(int)type]; }

	int getDiv_X(MouseExtendNS::M_BUTTONTYPE type){ return Div_x[(int)type]; }
	int getDiv_Y(MouseExtendNS::M_BUTTONTYPE type){ return Div_y[(int)type]; }

};

#endif