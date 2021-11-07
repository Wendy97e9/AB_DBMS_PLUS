#include "default_handler.h"


DefaultHandler::DefaultHandler()
{
	db = new Database;
}

RC DefaultHandler::create_table()
{
	/*
	1 ����Ƿ��Ѿ�����
	2 ������
	3 ����.table�ļ�������Ԫ����
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
	// �������ǽ���һ��Ĭ�ϵı�
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

Table* DefaultHandler::find_table(const char* dbname, const char* table_name) const {
	if (dbname == nullptr || table_name == nullptr) {
		
		return nullptr;
	}
	//Ĭ��DB


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
		cout << "û���ҵ�table" << table_name << endl;
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
	vector<AttrInfo*> attributes = table_meta.get_attributes();//��
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
			//cout << (char*)inserts->values[i].data << endl;	//�ַ���
			//delete[] attr1;
			break;
		case 2:
			attr2 = new int;
			cin >> *attr2;
			inserts->values[i].data = attr2;
			//cout << *(int*)inserts->values[i].data << endl;	//1
			//cout << *attr2 << endl;	//1
			//delete attr2;	//����ɾ��void*ָ��
			break;
		case 3:
			//floatδʵ��
			break;
		}
	}
	/////////////////////////////////////////////////////////////////////////
	inserts->value_num = attributes.size();
	table->insert_record(inserts->value_num, inserts->values);
	
	
	return RC();
}
