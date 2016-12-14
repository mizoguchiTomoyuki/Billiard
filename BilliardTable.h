#ifndef BILLIARDTABLE_H
#define BILLIARDTABLE_H
#include "gameObject.h";
#include "MeshRenderer.h"


class BilliardTable : public gameObject{

private:
	MeshRenderer _mesh;
	MeshRenderer _tablemat;
public:
	BilliardTable();
	virtual ~BilliardTable();
	virtual void update();
	virtual void draw();
	virtual void ai();
	virtual void collision();

};
#endif