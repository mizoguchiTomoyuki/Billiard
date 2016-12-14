#ifndef _PREFAB_MANAGER_H
#define _PREFAB_MANAGER_H
#include "constants.h"
class Ball;
class Camera;
class DirectionalLight;
class PointLight;
class Box;
class BilliardTable;
class DarkHole;
//Prefabを作る機能だけをまとめたクラス
class PrefabManager{
public:
	PrefabManager();
	~PrefabManager();
	ELEMID PrefabCreate(PREFAB_MENU p);


};
#include "Ball.h"
#include "camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Box.h"
#include "Billiardtable.h"
#include "DarkHole.h"
#endif