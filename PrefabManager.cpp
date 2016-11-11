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
	case PREFAB_MENU::EXIT:

		break;

	default:

		break;


	}
	return retid;
}