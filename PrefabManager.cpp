#include "PrefabManager.h"

PrefabManager::PrefabManager(){


}

PrefabManager::~PrefabManager(){

}

ELEMID PrefabManager::PrefabCreate(PREFAB_MENU p){
	ELEMID retid = -1;
	switch (p){
	case PREFAB_MENU::BALL:
	{
		Ball* _b = new Ball();
		retid = _b->getELEMID();
	}
	break;

	case PREFAB_MENU::CAMERA:
	{
		Camera* _c = new Camera();
		retid = _c->getELEMID();
	}
	break;
	case PREFAB_MENU::DIRECTIONALLIGHT:
	{
		DirectionalLight* _dl = new DirectionalLight();
		retid = _dl->getELEMID();
	}
	break;

	case PREFAB_MENU::POINTLIGHT:
	{
		PointLight* _pl = new PointLight();
		retid = _pl->getELEMID();
	}
	break;
	case PREFAB_MENU::BOX:
	{
		Box* _pl = new Box();
		retid = _pl->getELEMID();
	}
	break;

	case PREFAB_MENU::BALL_1:
	{
		Ball* _b = new Ball(ONEBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_1");
	}
	break;

	case PREFAB_MENU::BALL_2:
	{
		Ball* _b = new Ball(TWOBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_2");
	}
	break;

	case PREFAB_MENU::BALL_3:
	{
		Ball* _b = new Ball(THREEBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_3");
	}
	break;

	case PREFAB_MENU::BALL_4:
	{
		Ball* _b = new Ball(FOURBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_4");
	}
	break;

	case PREFAB_MENU::BALL_5:
	{
		Ball* _b = new Ball(FIVEBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_5");
	}
	break;

	case PREFAB_MENU::BALL_6:
	{
		Ball* _b = new Ball(SIXBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_6");
	}
	break;

	case PREFAB_MENU::BALL_7:
	{
		Ball* _b = new Ball(SEVENBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_7");
	}
	break;

	case PREFAB_MENU::BALL_8:
	{
		Ball* _b = new Ball(EIGHTBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_8");
	}
	break;

	case PREFAB_MENU::BALL_9:
	{
		Ball* _b = new Ball(NINEBALL_MESH);
		retid = _b->getELEMID();
		gameObjectFactory::Instance().GetElem(retid)->SetName("Ball_9");
	}
	break;

	case PREFAB_MENU::EXIT:

		break;

	default:

		break;


	}
	return retid;
}