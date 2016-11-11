//げーむつくろー
//http://marupeke296.com/NGDV_No2_CSVTable.html
//より参照

#ifndef _TABLE_H
#define _TABLE_H
#include <string>
#include <map>
class Table{
public:
	//Valriant レコード内データを表現
	struct Variant{
		int iVal;
		float fVal;
		std::string str;
		Variant() : iVal(), fVal(){}
		Variant(const std::string &s) : iVal(atoi(s.c_str())), fVal((float)atof(s.c_str())), str(s){}
		//各型にキャストして返す
		operator std::string(){ return str; }
		operator int(){ return iVal; }
		operator float(){ return fVal; }
	};

	//Recordクラス テーブルの項目を示す
	class Record{
		static Variant nullVariant;
		std::map<std::string, Variant*> vals;
	public:
		Record(){}
		~Record();
		Table::Variant& operator [](const char* key);
		void add(const char* key, Variant &val);

	};

	//以下Tableクラスの宣言
private:
	Table(const Table &src);
	Table& operator =(const Table &src);
protected:
	static Record nullRecord;
	std::map<std::string, Record*> records;
	int RecordNum;//行数,テーブルをまとめて読み込むのに使う

public:
	Table(const char* filePath);
	virtual ~Table();

	Record& operator[](const char* key);
	int getRecordNum(){ return RecordNum; }

};

#endif