#ifndef _OBJECTDATA_MANAGER_H
#define _OBJECTDATA_MANAGER_H
#include <fstream>
#include "gameObjectFactory.h"
#include "Table.h"
//gameObject�̃f�[�^���̂����t�@�C���̓ǂݍ��݁A�������݂��s��
namespace ODManagerNS{
	const char DATA_PATH[] = "ObjectData/gameObject.txt";
	const int COL = 9; //Table�̍��ڐ�

}
class ObjectDataManager{
protected:
	Table* gameObjectTable;
	bool initialized;
public:
	ObjectDataManager();
	~ObjectDataManager();

	void DataLoad();
	void DataSave();

};

#endif