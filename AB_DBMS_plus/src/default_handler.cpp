#include "default_handler.h"


DefaultHandler::DefaultHandler()
{
	db = new Database;
}

RC DefaultHandler::create_table()
{
	/*
	1 查表是否已经存在
	2 不存在
	3 建立.table文件，保存元数据
	4 
	
	
	
	*/
	//if(isExist())
	CreateTable* ct = new CreateTable;
	cout << "********** CREATE TABLE **********" << endl;
	cout << "********** RelationName **********" << endl;
	char* table_name = new char[20];
	cin >> table_name ;
	//ct->relation_name = table_name;
	strcpy(ct->relation_name, table_name);
	cout << ct->relation_name << endl;
	if (isExist(ct->relation_name))
	{
		cout << "Table Existed." << endl;
		delete[] table_name;
		return GENERIC_ERROR;
	}
	// 接下来是建立一个默认的表
	//////////////////////////////////////////////////////////////
	// id name
	ct->attribute_count = 2;
	strcpy(ct->attributes[0].name, "id");
	ct->attributes[0].type = INTS;
	ct->attributes[0].length = 4;
	strcpy(ct->attributes[1].name, "name");
	ct->attributes[1].type = CHARS;
	ct->attributes[1].length = 10;
	//////////////////////////////////////////////////////////////
	//// name sex age
	//ct->attribute_count = 3;
	//strcpy(ct->attributes[0].name, "name");
	//ct->attributes[0].type = CHARS;
	//ct->attributes[0].length = 10;
	//strcpy(ct->attributes[1].name, "sex");
	//ct->attributes[1].type = CHARS;
	//ct->attributes[1].length = 3;
	//strcpy(ct->attributes[2].name, "age");
	//ct->attributes[2].type = INTS;
	//ct->attributes[2].length = 4;
	////////////////////////////////////////////////////////////

	
	db->create_table(ct);
	db->print_all_opened_tables();
	delete[] table_name;
	delete ct;


	return RC::GENERIC_ERROR;
}

RC DefaultHandler::select_table()
{
	cout << "********** SELECT TABLE * **********" << endl;
	cout << "********** RelationName **********" << endl;
	char table_name[20];
	cin >> table_name;
	Table* table = find_table("db", table_name);
	// 1 dmatch找到所有的块
	// 2 in buffer?
	// 3 no: load
	// 4 print frame[frame_num]
	// 5 
	table->select_all("db", table_name);
	//table->analyze_all_record
	return RC();
}

Table* DefaultHandler::find_table(const char* dbname, const char* table_name) const {
	if (dbname == nullptr || table_name == nullptr) {
		
		return nullptr;
	}
	//默认DB


	return db->find_table(table_name);
}

void DefaultHandler::TEST_READ_BUFFER_DATA()
{
	cout << "********** TEST_READ_BUFFER_DATA **********" << endl;
	cout << "********** Relation Name **********" << endl;
	char table_name[20];
	cin >> table_name;
	Table* table = db->find_table(table_name);
	if (table == NULL)
		cout << "没有找到table" << table_name << endl;
	else
		table->TEST_READ_BUFFER_DATA("db", table_name);
}

void DefaultHandler::TEST_LRU()
{
	DiskBufferPool* bp = theGlobalDiskBufferPool();
	bp->TEST_LRU();
}

RC DefaultHandler::insert_record()
{
	cout << "********** INSERT RECORD **********" << endl;
	cout << "********** Relation Name **********" << endl;

	db->print_all_opened_tables();

	Inserts* inserts = new Inserts;
	char relation_name[20];
	cin >> relation_name;
	inserts->relation_name = relation_name;
	char temp[100];
	Table* table = find_table("db", inserts->relation_name);

	TableMeta table_meta = table->get_table_meta();
	vector<AttrInfo*> attributes = table_meta.get_attributes();//√
	char* attr1;
	int* attr2;
	float* attr3;
	/////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < attributes.size(); i++)
	{
		cout << attributes[i]->name << "\t" << attributes[i]->type << "\t" << attributes[i]->length << endl;
		inserts->values[i].type = attributes[i]->type;
		int type = attributes[i]->type;
		int len = attributes[i]->length;
		switch (type)
		{
		case 0:
			break;
		case 1:
			attr1 = new char[len];
			cin >> attr1;
			inserts->values[i].data = attr1;
			//cout << (char*)inserts->values[i].data << endl;	//字符串
			//delete[] attr1;
			break;
		case 2:
			attr2 = new int;
			cin >> *attr2;
			inserts->values[i].data = attr2;
			//cout << *(int*)inserts->values[i].data << endl;	//1
			//cout << *attr2 << endl;	//1
			//delete attr2;	//不能删，void*指向
			break;
		case 3:
			//float未实现
			break;
		}
	}
	/////////////////////////////////////////////////////////////////////////
	inserts->value_num = attributes.size();
	table->insert_record(inserts->value_num, inserts->values);
	return RC();
}

//const char* dbname, const char* relation_name, const char* index_name, const char* attribute_name
RC DefaultHandler::create_index() {
	cout << "********** INSERT INDEX **********" << endl;
	cout << "********** Relation Name **********" << endl;

	CreateIndex* ci = new CreateIndex;
	strcpy(ci->relation_name , "T1");
	strcpy(ci->index_name, "ixid");
	strcpy(ci->attribute_name, "id");
	Table* table = find_table("db", ci->relation_name);
	if (nullptr == table) {
		return RC::GENERIC_ERROR;
	}
	table->create_index("db", ci->relation_name, ci);

	RC ret = GENERIC_ERROR;
	return ret;
}
