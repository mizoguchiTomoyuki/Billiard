//���[�ނ���[
//http://marupeke296.com/NGDV_No2_CSVTable.html
//���Q��

#ifndef _TABLE_H
#define _TABLE_H
#include <string>
#include <map>
class Table{
public:
	//Valriant ���R�[�h���f�[�^��\��
	struct Variant{
		int iVal;
		float fVal;
		std::string str;
		Variant() : iVal(), fVal(){}
		Variant(const std::string &s) : iVal(atoi(s.c_str())), fVal((float)atof(s.c_str())), str(s){}
		//�e�^�ɃL���X�g���ĕԂ�
		operator std::string(){ return str; }
		operator int(){ return iVal; }
		operator float(){ return fVal; }
	};

	//Record�N���X �e�[�u���̍��ڂ�����
	class Record{
		static Variant nullVariant;
		std::map<std::string, Variant*> vals;
	public:
		Record(){}
		~Record();
		Table::Variant& operator [](const char* key);
		void add(const char* key, Variant &val);

	};

	//�ȉ�Table�N���X�̐錾
private:
	Table(const Table &src);
	Table& operator =(const Table &src);
protected:
	static Record nullRecord;
	std::map<std::string, Record*> records;
	int RecordNum;//�s��,�e�[�u�����܂Ƃ߂ēǂݍ��ނ̂Ɏg��

public:
	Table(const char* filePath);
	virtual ~Table();

	Record& operator[](const char* key);
	int getRecordNum(){ return RecordNum; }

};

#endif