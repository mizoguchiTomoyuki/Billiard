#ifndef _OBJECTDATA_MANAGER_H
#define _OBJECTDATA_MANAGER_H
#include <fstream>
#include "gameObjectFactory.h"
#include "Table.h"
//gameObjectのデータをのせたファイルの読み込み、書き込みを行う
namespace ODManagerNS{
	const char DATA_PATH[] = "ObjectData/gameObject.txt";
	const int COL = 9; //Tableの項目数

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