#include "MouseExtend.h"

MouseExtend::MouseExtend(){
	for (int i = 0; i < MouseExtendNS::END; i++){
		is_Drag[i] = false;
		TrackStart_x[i] = 0;
		TrackStart_y[i] = 0;
		DragDistance_x[i] = 0;
		DragDistance_y[i] = 0;
		Time_MouseClick[i] = 0.0f;
	}

}

MouseExtend::~MouseExtend(){

}

void MouseExtend::start(gameObject* t){
	Task::start(t);


}

void MouseExtend::update(){
	Input* input = GameSceneManager::Instance().GetGameptr()->getInput(); //現在のゲームシーンからinputのポインタを取得

	float frameTime = GameSceneManager::Instance().GetGameptr()->getframeTime();


	if (input->getMouseLButton()){
		if (!is_Drag[MouseExtendNS::M_LEFT]){
			is_Drag[MouseExtendNS::M_LEFT] = true;
			TrackStart_x[MouseExtendNS::M_LEFT] = input->getMouseX();
			TrackStart_y[MouseExtendNS::M_LEFT] = input->getMouseY();
		}
	}
	else{
		is_Drag[MouseExtendNS::M_LEFT] = false;
		TrackStart_x[MouseExtendNS::M_LEFT] = 0;
		TrackStart_y[MouseExtendNS::M_LEFT] = 0;
		Time_MouseClick[MouseExtendNS::M_LEFT] = 0.0f;
	}

	if (input->getMouseRButton()){
		if (!is_Drag[MouseExtendNS::M_RIGHT]){
			is_Drag[MouseExtendNS::M_RIGHT] = true;
			TrackStart_x[MouseExtendNS::M_RIGHT] = input->getMouseX();
			TrackStart_y[MouseExtendNS::M_RIGHT] = input->getMouseY();
		}
	}
	else{
		is_Drag[MouseExtendNS::M_RIGHT] = false;
		TrackStart_x[MouseExtendNS::M_RIGHT] = 0;
		TrackStart_y[MouseExtendNS::M_RIGHT] = 0;
		Time_MouseClick[MouseExtendNS::M_RIGHT] = 0.0f;
	}


	if (input->getMouseMButton()){
		if (!is_Drag[MouseExtendNS::M_MID]){
			is_Drag[MouseExtendNS::M_MID] = true;
			TrackStart_x[MouseExtendNS::M_MID] = input->getMouseX();
			TrackStart_y[MouseExtendNS::M_MID] = input->getMouseY();
		}
	}
	else{

		is_Drag[MouseExtendNS::M_MID] = false;
		TrackStart_x[MouseExtendNS::M_MID] = 0;
		TrackStart_y[MouseExtendNS::M_MID] = 0;
		Time_MouseClick[MouseExtendNS::M_MID] = 0.0f;
	}

	

	for (int i = 0; i < MouseExtendNS::END; i++){
		if (is_Drag[i]){
			DragDistance_x[i] = TrackStart_x[i] - input->getMouseX();
			DragDistance_y[i] = TrackStart_y[i] - input->getMouseY();
			Time_MouseClick[i] += frameTime;
			Div_x[i] = StampPoint_x[i] - input->getMouseX();
			Div_y[i] = StampPoint_y[i] - input->getMouseY();
		}
		else{
			DragDistance_x[i] = 0;
			DragDistance_y[i] = 0;
		}

		StampPoint_x[i] = input->getMouseX();
		StampPoint_y[i] = input->getMouseY();

	}



}
