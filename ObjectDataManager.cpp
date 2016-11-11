#include "ObjectDataManager.h"
#include "PrefabManager.h"
ObjectDataManager::ObjectDataManager(){
	initialized = false;
	
}

ObjectDataManager::~ObjectDataManager(){
	delete gameObjectTable;

}

void ObjectDataManager::DataLoad(){

	FILE* textFile;
	PrefabManager p;
	errno_t error;
	if (error = fopen_s(&textFile, ODManagerNS::DATA_PATH, "r") != 0){ //èâä˙âªêßçÏ

		errno_t writeerror;
		if (writeerror = fopen_s(&textFile, ODManagerNS::DATA_PATH, "w") != 0){
		}
		else{

			std::ofstream fs1(ODManagerNS::DATA_PATH);
			fs1 << ODManagerNS::COL << std::endl; //çÄñ⁄êî
			fs1 << 0 << std::endl; //èâä˙âªÇ»ÇÃÇ≈0
			fs1 << "id,pNum,name,px,py,pz,sx,sy,sz" << std::endl;
			fs1.close();
		}
	}
	fclose(textFile);
	try{
		gameObjectTable = new Table(ODManagerNS::DATA_PATH);
		int size = gameObjectTable->getRecordNum();
		for (int i = 0; i < size; i++){
			Table gOTable(ODManagerNS::DATA_PATH);
			char c[1024];
			sprintf_s(c,1024, "%d", i);
			Table::Record &r = gOTable[c];
			int pn = r["pNum"];
			ELEMID id = p.PrefabCreate((PREFAB_MENU)pn);
			gameObject* obj = gameObjectFactory::Instance().GetElem(id);
			obj->setTable_id(i);
			obj->SetName(r["name"]);
			float px = r["px"];
			float py = r["py"];
			float pz = r["pz"];
			float sx = r["sx"];
			float sy = r["sy"];
			float sz = r["sz"];
			obj->setPosition({ px, py, pz });
			obj->setScale({ sx, sy, sz });

		}
	}
	catch (...){

	}
	initialized = true;

}

void ObjectDataManager::DataSave(){
	if (!initialized)
		return;

	std::ofstream fs1(ODManagerNS::DATA_PATH);
	int objSize = gameObjectFactory::Instance().Getsize();
	fs1 << ODManagerNS::COL << std::endl; //çÄñ⁄êî
	fs1 << objSize << std::endl;
	fs1 << "id,pNum,name,px,py,pz,sx,sy,sz" << std::endl;

	gameObjectFactory::gameObjectLink* obj = gameObjectFactory::Instance().GetFirstLink();
	for (int i = 0; i < objSize; i++){
		obj->pointer->setTable_id(i);
		obj->pointer->recordupdate();
		std::string r = obj->pointer->getRecord();
		fs1 << r << std::endl;
		if (obj->next != nullptr)
			obj = obj->next;
		else
			i = objSize + 1;

	}

}
