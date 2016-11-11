#include "Table.h"
#include <fstream>
#include <vector>
#include "Context.h"
Table::Record Table::nullRecord;

Table::Table(const char* filePath){
	std::ifstream ifs(filePath);
	if (ifs.is_open() == true){
		int col = 0,row = 0;
		char s[2048] = {};
		ifs.getline(s, 2048); //�ŏ��ɗ�
		col = atoi(s);
		ifs.getline(s, 2048); //���ɍs
		row = atoi(s);
		RecordNum = row;
		//�f�[�^key�擾
		std::vector<std::string> dataKeys;
		Context<2048> ct; //Context�ň�s�̃e�L�X�g�����
		ifs.getline(ct.str, 2048);
		for (int c = 0; c < col; c++)
			dataKeys.push_back(ct.getAndNext());

		ct.reset();
		for (int r = 0; r < row; r++){
			ifs.getline(ct.str, 2048);
			Record *rec = new Record();
			std::string key = ct.getAndNext();
			for (int c = 1; c < col; c++)
				rec->add(dataKeys[c].c_str(), Variant(ct.getAndNext()));
			records[key] = rec;
			ct.reset();
		}

	}

}

Table::~Table(){
	std::map<std::string, Record*>::iterator it;
	for (it = records.begin(); it != records.end(); it++)
		delete it->second;
}

Table::Record& Table::operator[](const char* key){
	if (records.find(key) == records.end()) //Table[]�\�L�ŏ��L���Ă��郌�R�[�h��������o��
		return nullRecord;
	return *records[key];
}

//Record�N���X�̎���
Table::Variant Table::Record::nullVariant;

Table::Record::~Record(){
	std::map<std::string, Variant*>::iterator it;
	for (it = vals.begin(); it != vals.end(); it++)
		delete it->second;
}

Table::Variant& Table::Record::operator[](const char* key){
	if (vals.find(key) == vals.end())
		return nullVariant;
	return *vals[key];
}

void Table::Record::add(const char* key, Variant &val){
	if (vals.find(key) == vals.end())
		vals[key] = new Variant(val);
	else
		(*vals[key]) = val;
}