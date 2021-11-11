#include "table.h"

Table::Table()
{
	data_buffer_pool_ = theGlobalDiskBufferPool();
	record_handler_ = new RecordHandler();
}
const char* Table::name() const {
	return table_meta_.name();
}

RC Table::create(const char* table_path_name, CreateTable* ct)
{
	// 1 存tablemeta

	fstream outfile;
	string suffixnameadd = string(table_path_name) + TABLE_META_SUFFIX;
	cout << "suffix 加了 " << suffixnameadd << endl;
	outfile.open(suffixnameadd, ios::out | ios::binary);
	//outfile.write("tablemeta", 10);
	outfile.write((char*)ct, sizeof(CreateTable));
	
	outfile.close();

	open(suffixnameadd.c_str());
	return RC::SUCCESS;
}

RC Table::create_index(const char* file_name, const char* table_name, CreateIndex* ci)
{
	// 1 ci写文件
	// 2 
	BplusTreeIndex* bpt = new BplusTreeIndex();

	select_all_rid(file_name, table_name, ci->attribute_name);
	
	return RC::SUCCESS;
}

//用于建立索引
RC Table::select_all_rid(const char* file_name, const char* table_name, char* attribute_name)
{
	vector<DMATCH*> vec_data_match;
	data_buffer_pool_->find_all_page(file_name, table_name, vec_data_match);
	unordered_map<int, char*> slot_row;
	unordered_map<int, RID*> key_rid;
	RID* tmprid;
	for (int i = 0; i < vec_data_match.size(); i++)
	{
		slot_row.clear();
		int frame_num = data_buffer_pool_->inBuffer(vec_data_match[i]->page_num);
		//要判断frame情况，是不是全部都已经分配了
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!判断在不在缓冲区
		if (frame_num == -1)	//不在Buffer
		{
			data_buffer_pool_->get_this_page("db", table_name, vec_data_match[i]->page_num, frame_num);
		}
		data_buffer_pool_->find_all_row(table_name, frame_num, table_meta_.record_size(), slot_row);
		cout << "    ------------- FRAME " << frame_num << " ------------" << endl;
		for (auto& it : slot_row)
		{
			tmprid = new RID();
			cout << "    ------------- SLOT " << it.first << " -------------" << endl;
			tmprid->page_num = vec_data_match[i]->page_num;
			tmprid->slot_num = it.first;
			int key = find_key(attribute_name, it.second);
			key_rid[key] = tmprid;
			//analyze_record(it.second);
			cout << "---------------------------------------------------" << endl;
		}
		

	}
	///////////////////////////////////////////////////////////////////////////
	BplusTreeIndex* bpt = new BplusTreeIndex();
	vector < Block* > Blocks;
	for (auto& it : key_rid)
	{
		bpt->insertNode(bpt->rootBlock, INTS, 4, (char*)&it.first);
		Blocks.clear();
		Blocks.push_back(bpt->rootBlock);
		bpt->print(Blocks);
		//
		char* row;
		data_buffer_pool_->get_this_row(file_name, table_name, it.second->page_num, it.second->slot_num, table_meta_.record_size(),row);
		analyze_record(row);
	}
	return RC::SUCCESS;
}



RC Table::select_all(const char* file_name, const char* table_name)
{
	vector<DMATCH*> vec_data_match;
	data_buffer_pool_->find_all_page(file_name, table_name, vec_data_match);
	unordered_map<int, char*> slot_row;
	for (int i = 0; i < vec_data_match.size(); i++)
	{
		slot_row.clear();
		int frame_num = data_buffer_pool_->inBuffer(vec_data_match[i]->page_num);
		//要判断frame情况，是不是全部都已经分配了
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!判断在不在缓冲区
		if (frame_num == -1)	//不在Buffer
		{
			data_buffer_pool_->get_this_page("db", table_name, vec_data_match[i]->page_num, frame_num);
		}
		data_buffer_pool_->find_all_row(table_name, frame_num, table_meta_.record_size(), slot_row);
		cout << "    ------------- FRAME " << frame_num << " ------------" << endl;
		for (auto& it : slot_row)
		{
			cout << "    ------------- SLOT " << it.first << " -------------" << endl;
			analyze_record(it.second);
			cout << "---------------------------------------------------" << endl;
		}
		

	}
	return RC();
}

//这个参数已经加了.table后缀
RC Table::open(const char* table_path_name)
{
	cout << "open " << table_path_name << endl;
	// 打开元数据的表，并且初始化table_meta_
	// 如果实现了索引部分，还要添加index_meta_部分，这个部分在table_meta_中进行
	CreateTable* read_table_meta = new CreateTable;	//用createtable结构体去读取元数据的属性，如果有索引，再用CreateIndex，要判断文件结尾
	fstream infile;
	string suffixnameadd = string(table_path_name);
	infile.open(table_path_name, ios::in | ios::binary);
	infile.read((char*)read_table_meta, sizeof(CreateTable));
	//cout << "读取元数据" << read_table_meta->relation_name << endl;
	//cout << read_table_meta->attribute_count << endl;
	//cout << read_table_meta->attributes[0].name << read_table_meta->attributes[0].type << read_table_meta->attributes[0].length << endl;
	//cout << read_table_meta->attributes[1].name << read_table_meta->attributes[1].type << read_table_meta->attributes[1].length << endl;
	table_meta_.init(read_table_meta);
	

	delete read_table_meta;
	infile.close();
	return RC::SUCCESS;
}

RC Table::insert_record(int value_num, const Value* values)
{

	//record_handler_->insert_record(record)
	char* record_data;
	make_record(value_num, values, record_data);
	RID rid;
	//cout << "TABLE NAME " << name() << endl;
	record_handler_->insert_record("db", name(), record_data, table_meta_.record_size(), rid);
	cout << "INSERT RID : PAGE : " << rid.page_num << " SLOT : " << rid.slot_num << endl;
	return RC::SUCCESS;
}

RC Table::make_record(int value_num, const Value* values, char*& record_out)
{
	//cout << "*********************** MAKE RECORD *************************" << endl;
	//检查字段类型是否一致，经过上一次函数，必须一致了

	//转为char*，再转为真正的类型
	int record_size = table_meta_.record_size();
	char* record = new char[record_size];
	int offset = 0;
	vector<AttrInfo*> attributes = table_meta_.get_attributes();
	for (int i = 0; i < value_num; i++)
	{
		memcpy(record + offset, (char *)values[i].data, attributes[i]->length);
		offset += attributes[i]->length;
	}
	////////////////////////////////////////////////////////////
	//int* tmp = new int;
	//memcpy(tmp, record, sizeof(int));
	//cout << "make record tmp   " << *tmp << endl;
	//delete tmp;
	////////////////////////////////////////////////////////////
	record_out = record;
	return RC::SUCCESS;
}

//key int
int Table::find_key(char* attribute_name, char* record)
{
	vector<AttrInfo*> attributes = table_meta_.get_attributes();
	char* attr1;
	int* attr2;
	float* attr3;
	for (int i = 0; i < attributes.size(); i++)
	{
		int type = attributes[i]->type;
		int len = attributes[i]->length;
		if (strcmp(attributes[i]->name, attribute_name) == 0)
		{

			cout << "find key!!!!!!!!!!!!!!!!!!!!!!!!!! " ;
			attr2 = new int;
			memcpy((char*)attr2, record, len);
			cout << *attr2 << endl;
			int ret = *attr2;
			delete attr2;
			return ret;
		}

		record += len;

		//cout << attr << endl;

	}
	return RC::GENERIC_ERROR;
}


//type 1 char 2 int 3 float
RC Table::analyze_record(char* record)
{
	vector<AttrInfo*> attributes = table_meta_.get_attributes();
	char* attr1;
	int* attr2;
	float* attr3;
	for (int i = 0; i < attributes.size(); i++)
	{
		int type = attributes[i]->type;
		int len = attributes[i]->length;
		cout << "\t属性名" << "\t类型" << "\t长度" << "\t值" << endl;
		cout << "\t" << attributes[i]->name << "\t" << type << "\t" << len << "\t";
		switch (type)
		{
		case 0:
			break;
		case 1:
			attr1 = new char[len];
			memcpy(attr1, record, len);
			cout << attr1 << endl;
			delete[] attr1;
			break;
		case 2:
			attr2 = new int;
			memcpy((char*)attr2, record, len);
			cout << *attr2 << endl;
			delete attr2;
			break;
		case 3:
			break;

		}
		record += len;
		
		//cout << attr << endl;

	}
	return RC::SUCCESS;
}

RC Table::analyze_all_record(vector<char*> record)
{
	for (int i = 0; i < record.size(); i++)
	{
		analyze_record(record[i]);
	}
	return RC::SUCCESS;
}

TableMeta Table::get_table_meta()
{
	return table_meta_;
}

void Table::TEST_READ_BUFFER_DATA(const char* dbname, const char* table_name)
{
	vector<int> frame_num;
	data_buffer_pool_->get_this_page_by_name(dbname, table_name, frame_num);
	cout << "\t" << table_name << "对应的FRAME数量是" << frame_num.size() << endl;
	
	for (int i = 0; i < frame_num.size(); i++)
	{
		unordered_map<int, char*> slot_num;
		data_buffer_pool_->find_all_row(table_name, frame_num[i], table_meta_.record_size(), slot_num);
		cout<<"    ------------- FRAME " << frame_num[i] << " ------------" << endl;
		for (auto& it : slot_num)
		{
			cout << "    ------------- SLOT " << it.first << " -------------" << endl;
			analyze_record(it.second);
			cout << "---------------------------------------------------" << endl;
		}
	}
}


