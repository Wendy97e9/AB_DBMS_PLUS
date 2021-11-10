#include "disk_buffer_pool.h"


//#include <time.h>
DiskBufferPool* theGlobalDiskBufferPool()
{
	static DiskBufferPool* instance = new DiskBufferPool();
	//cout << "theGlobalDiskBufferPool" << endl;
	return instance;
}

//unsigned long current_time()
//{
//	struct timespec tp;
//	clock_gettime(CLOCK_MONOTONIC, &tp);
//	return tp.tv_sec * 1000 * 1000 * 1000UL + tp.tv_nsec;
//}



RC DiskBufferPool::test(const char* file_name)
{
	//fstream outfile;
	//cerr << DEFAULT_SAVE_PATH + string(file_name)<<endl;

	////outfile.open("abdbmsplus/db/test", ios::out | ios::binary);
	//outfile.open(DEFAULT_SAVE_PATH + string(file_name), ios::out|ios::binary);
	//if (!outfile)
	//{
	//	cerr << "open file error!\n" << endl;
	//}
	Page* first_page = new Page;
	//first_page->page_num = 0;
	//first_page->numofslot = 0;
	//first_page->maxofslot = -1;//��ʼ

	Page* second_page = new Page;
	//second_page->page_num = 1;
	//second_page->numofslot = 0;
	//second_page->maxofslot = -1;

	///// /////////////////////////////////////////////////////////


	//int a = 3;

	//char c[10];

	//memcpy(c, &a, 4);

	//int b = 0;

	//memcpy(&b, c, 4);

	//cout << "c" << c << endl;
	//cout << "b" << b << endl;

	///// 
	///// 
	///// 
	///// 
	///// 
	int offset = 10;	//int 4, char 6
	//int tmpint = 99;
	//char tmpchar6[6] = "999";
	//char* data = new char[offset];
	////char data[11];
	//cout << "char*";
	//cout << sizeof((char*)tmpint) << endl;
	//cout << "data " << sizeof(data) << endl;
	////memcpy(data, (char*)tmpint, sizeof(int));


	//memcpy(data, &tmpint, sizeof(int));
	//memcpy(data + sizeof(int), tmpchar6, sizeof(char) * 6);
	//memcpy(first_page->data, data, offset);
	//delete[] data;

	//outfile.write((char*)first_page, sizeof(Page));
	//outfile.write((char*)second_page, sizeof(Page));
	//outfile.close();
	delete first_page;
	delete second_page;
	fstream infile;
	infile.open(DEFAULT_SAVE_PATH+string(file_name), ios::in | ios::binary);
	if (!infile)
	{
		cerr << "open file error!\n" << endl;
	}
	first_page = new Page();
	second_page = new Page();
	infile.read((char*)first_page, sizeof(Page));
	cout << first_page->page_num << endl;
	cout << first_page->numofslot << endl;
	cout << first_page->maxofslot << endl;
	char* readdata = new char[offset];
	memcpy(readdata, first_page->data, offset);
	int resultint;
	char resultchar[6];
	memcpy(&resultint, readdata, 4);
	memcpy(&resultchar, readdata + 4, 6);
	cout << resultint << endl;
	cout << resultchar << endl;
	delete[] readdata;
	infile.seekg(BP_PAGE_SIZE ,ios::beg);
	infile.read((char*)second_page, sizeof(Page));
	cout << second_page->page_num << endl;
	cout << second_page->numofslot << endl;
	cout << second_page->maxofslot << endl;
	infile.close();
	delete first_page;
	delete second_page;

	cout << isExist(file_name) << endl;
	cout << isExist("wendy.data") << endl;

	
	return RC::SUCCESS;
}


RC DiskBufferPool::create_file(const char* file_name)
{
	fstream outfile;
	fstream matchfile;
	
	string dbdatafile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATA_SUFFIX);
	string dbmatchfile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATAMATCH_SUFFIX);

	cerr << dbdatafile << endl;
	cerr << dbmatchfile << endl;
	if (isExist(dbdatafile.c_str()))
	{
		cerr << "�����ļ��Ѿ�����" << endl;
		return RC::SUCCESS;
	}
	if (isExist(dbmatchfile.c_str()))
	{
		cerr << "����ƥ���ļ��Ѿ�����" << endl;
		return RC::SUCCESS;
	}
	//outfile.open("abdbmsplus/db/test", ios::out | ios::binary);
	outfile.open(dbdatafile, ios::out | ios::binary);
	matchfile.open(dbmatchfile, ios::out | ios::binary);
	if (!outfile||!matchfile)
	{
		cerr << "open file error!\n" << endl;
	}
	Page* init_page = new Page;
	init_page->page_num = 0;
	init_page->numofslot = 0;
	init_page->maxofslot = -1;//��ʼ
	DMATCH* init_data_match = new DMATCH;
	init_data_match->page_num = 0;
	strcpy(init_data_match->table_name, "NULL");
	init_data_match->state = UNFULL;
	for (int i = 0; i < 10; i++)
	{
		outfile.write((char*)init_page, sizeof(Page));
		matchfile.write((char*)init_data_match, sizeof(DMATCH));
		init_page->page_num++;
		init_data_match->page_num++;
	}
	delete init_page;
	outfile.close();
	matchfile.close();
	return RC::SUCCESS;
}



RC DiskBufferPool::open_file(const char* file_name, int* file_id)
{
	return RC::GENERIC_ERROR;
}

//ͨ��pagenum��frame_Num
//�Ѵ��̵�page���ص�����frame�У��޸�allocated
RC DiskBufferPool::get_this_page(const char* file_name, const char* table_name, PageNum page_num, int& frame_num)
{
	string dbdatafile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATA_SUFFIX);
	string dbmatchfile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATAMATCH_SUFFIX);

	fstream datafile;
	fstream matchfile;

	datafile.open(dbdatafile, ios::in | ios::binary);
	matchfile.open(dbmatchfile, ios::in | ios::binary);

	bool flag = false;
	for (int i = 0; i < bp_manager_.size; i++)
	{
		if (bp_manager_.allocated[i] == false)
		{
			//relationname�ڵ��õĵط���
			frame_num = i;
			flag = true;//�ҵ��ˣ�����
			break;
		}
	}
	//frameȫ�������ˣ���LRU�û�
	if (flag == false)
	{
		cout << "*************** LRU ***************" << endl;
		//bp_manager_.print_all_frame();
		frame_num = bp_manager_.doLRU();
		//flush
		if (bp_manager_.frame[frame_num].dirty == true)
		{
			cout << "*************** LRU FLUSH ***************" << endl;
			flush_this_page(file_name, frame_num);
		}
		bp_manager_.frame[frame_num].dirty = false;
		bp_manager_.frame[frame_num].acc_time = GetCurTime();
		bp_manager_.frame[frame_num].relation_name = NULL;
		bp_manager_.frame[frame_num].state = UNFULL;
		bp_manager_.allocated[frame_num] = false;
	}
	cout << "����PAGE " << page_num << " �� FRAME " << frame_num << endl;
	//cout << "get_this_page frame_num : " << frame_num << endl;
	bp_manager_.allocated[frame_num] = true;
	datafile.seekg((page_num)*BP_PAGE_SIZE, ios::beg);
	Page* tmppage = new Page;
	datafile.read((char*)tmppage, sizeof(Page));
	//��bp_manager_.frame[i].page
	bp_manager_.frame[frame_num].page = *tmppage;
	bp_manager_.frame[frame_num].acc_time = GetCurTime();
	int len = strlen(table_name);
	char* chartable_name = new char[len];
	strcpy(chartable_name, table_name);
	bp_manager_.frame[frame_num].relation_name = chartable_name;
	//�ж��ǲ�������page��������frame��
	matchfile.seekg((page_num) * sizeof(DMATCH), ios::beg);
	DMATCH* data_match = new DMATCH;
	matchfile.read((char*)data_match, sizeof(DMATCH));
	bp_manager_.frame[frame_num].state = data_match->state;
	delete data_match;
	datafile.close();
	matchfile.close();
	//bp_manager_.print_all_frame();
	return RC();
}

//�������ļ������µ�һҳ�������
RC DiskBufferPool::allocate_page(const char* file_name, const char* table_name, int& frame_num)
{
	string dbdatafile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATA_SUFFIX);
	string dbmatchfile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATAMATCH_SUFFIX);

	fstream matchfile;
	matchfile.open(dbmatchfile, ios::in | ios::out | ios::binary);
	string l;
	DMATCH* data_match;
	while (matchfile.peek() != EOF)
	{
		data_match = new DMATCH;
		matchfile.read((char*)data_match, sizeof(DMATCH));
		cout << "��ȡƥ���ļ� " << data_match->page_num << " " << data_match->table_name << " " << data_match->state << endl;
		//cout << strcmp(data_match->table_name, "NULL") << endl;
		if (strcmp(data_match->table_name, "NULL") == 0)
		{
			// ��ȡ��Ӧdata_match->page_num����һPage��BPManager��Frame�У��޸�DMATCH�ļ�����Page״̬��Ϊ��������table

			get_this_page(file_name, table_name, data_match->page_num, frame_num);
			//cout << "���� FRAME " << frame_num << endl;
			strcpy(data_match->table_name, table_name);
			matchfile.seekp(sizeof(DMATCH) * (data_match->page_num), ios::beg);
			matchfile.write((char*)data_match, sizeof(DMATCH));
			delete data_match;
			break;
		}
		delete data_match;
	}
	matchfile.close();
	
	//read_DMATCH(file_name);
	return RC::SUCCESS;
}

RC DiskBufferPool::read_DMATCH(const char* file_name)
{
	cout << "********** READ DATAMATCH **********" << endl;
	string dbmatchfile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATAMATCH_SUFFIX);
	fstream matchfile;
	matchfile.open(dbmatchfile, ios::in | ios::binary);
	matchfile.seekg(0, ios::beg);
	DMATCH* data_match;
	while (matchfile.peek() != EOF)
	{
		data_match = new DMATCH;
		matchfile.read((char*)data_match, sizeof(DMATCH));
		cout << "matchfile read: " << data_match->page_num << " " << data_match->table_name << " " << data_match->state << endl;
		delete data_match;
	}
	matchfile.close();
	return RC::SUCCESS;
}

//DMATCH��ɽṹ���д֮ǰ�����ж�д�ĺ���
//RC DiskBufferPool::allocate_page(const char* file_name, const char* table_name, int& frame_num)
//{
//	string dbdatafile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATA_SUFFIX);
//	string dbmatchfile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATAMATCH_SUFFIX);
//
//	fstream matchfile;
//	matchfile.open(dbmatchfile, ios::in | ios::binary);
//	string l;
//	while (!matchfile.eof())
//	{
//		getline(matchfile, l);
//		//cout << l << endl;
//		stringstream input;
//		input << l;
//		string result;
//		input >> result;
//		string tmppid = result;
//		input >> result;
//		string tmptbname = result;
//		cout << tmppid << endl;
//		cout << tmptbname << endl;
//		if (tmptbname == "NULL")
//		{
//			// ��ȡ��Ӧtmppid����һPage��BPManager��Frame�У��޸�DMATCH�ļ�����Page״̬��Ϊ��������table
//			int page_num = atoi(tmppid.c_str());
//			get_this_page(file_name, table_name, page_num, frame_num);
//			cout << "allocate_page frame_num" << frame_num << endl;
//			tmptbname = table_name;
//			matchfile.tellg();
//			matchfile << tmppid << " " << tmptbname;
//			break;
//		}
//	}
//	matchfile.seekg(0, ios::beg);
//	while (!matchfile.eof())
//	{
//		getline(matchfile, l);
//		//cout << l << endl;
//		stringstream input;
//		input << l;
//		string result;
//		input >> result;
//		string tmppid = result;
//		input >> result;
//		string tmptbname = result;
//		cout << tmppid << endl;
//		cout << tmptbname << endl;
//	}
//
//	matchfile.close();
//
//	return RC();
//}

//������frame
RC DiskBufferPool::get_this_page_by_name(const char* file_name, const char* relation_name, vector<int>& vec_frame_num)
{

	cout << "\t��ϵ��" << "\tҳ��(page_num)" << endl;

	for (int i = 0; i < bp_manager_.size; i++)
	{
		if (bp_manager_.allocated[i] == false)
			continue;
		if (strcmp(bp_manager_.frame[i].relation_name, relation_name)==0)
		{
			vec_frame_num.push_back(i);
			cout << "\t" << relation_name << "\t" << bp_manager_.frame[i].page.page_num << endl;
		}
	}
	return RC::SUCCESS;
}

//�ӵ�ǰ�Ѿ������frame�У��Ҷ�Ӧtable_name�Ŀ���ҳ
RC DiskBufferPool::get_free_page(const char* file_name, const char* table_name, int row_size, int& frame_num)
{
	cout << "********** get_free_page **********" << endl;
	for (int i = 0; i < bp_manager_.size; i++)
	{
		if (bp_manager_.allocated[i] == false)
			continue;
		//cout << "compare " << bp_manager_.frame[i].relation_name << " " << table_name << endl;
		int max = BP_PAGE_DATA_SIZE / (sizeof(int) + row_size);
		if (bp_manager_.frame[i].page.numofslot < max)
		{
			bp_manager_.frame[i].state = UNFULL;
		}
		else {
			bp_manager_.frame[i].state = FULL;  
		}
		if (bp_manager_.frame[i].state==UNFULL&& strncmp(table_name, bp_manager_.frame[i].relation_name, 20) == 0)
		{
			frame_num = i;
			cout << "��ǰ�������Ŀ���Page�ǣ�(pid)  " << bp_manager_.frame[i].page.page_num << endl;
			return RC::SUCCESS;
		}
	}
	frame_num = -1;
	return RC::GENERIC_ERROR;
}

//��DMATCH��
RC DiskBufferPool::find_all_page(const char* file_name, const char* relation_name, vector<DMATCH*>& vec_data_match)
{
	string dbmatchfile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATAMATCH_SUFFIX);
	fstream matchfile;
	matchfile.open(dbmatchfile, ios::in | ios::binary);
	matchfile.seekg(0, ios::beg);
	DMATCH* data_match;
	while (matchfile.peek() != EOF)
	{
		data_match = new DMATCH;
		matchfile.read((char*)data_match, sizeof(DMATCH));
		if (strcmp(relation_name, data_match->table_name) == 0)
		{
			//cout << "find_all_page  read: " << data_match->page_num << data_match->table_name << data_match->state << endl;
			vec_data_match.push_back(data_match);
		}
		
		//delete data_match;	//��ɾ�𣿣�����Ȼ�����ˣ�����
	}
	//cout << "find_all_page : " << vec_data_match.size() << endl;
	matchfile.close();
	return RC::SUCCESS;
}

//�����������ģ����FULL����DMATCH�޸�
//!!!!!!!allocated�ģ�����
RC DiskBufferPool::flush_this_page(const char* file_name, int frame_num)
{
	if (bp_manager_.allocated[frame_num] == false)
	{
		cout << "ERROR! ��ͼFLUSHδ�����FRAME " << endl;
		return RC::GENERIC_ERROR;
	}
	cout << "********** flush_this_page **********" << endl;
	string dbdatafile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATA_SUFFIX);
	int page_num = bp_manager_.frame[frame_num].page.page_num;
	fstream datafile;
	
	// ios::in��������������ļ���д����������ָ��λ���޸�
	datafile.open(dbdatafile,ios::in | ios::out | ios::binary);
	datafile.seekp(sizeof(Page) * page_num, ios::beg);
	//cout << "page_num * sizeof(Page)" << page_num * sizeof(Page) << endl;
	datafile.write((char*)&bp_manager_.frame[frame_num].page, sizeof(Page));
	datafile.close();

	string dbmatchfile = string(DEFAULT_DB_PATH) + "/" + string(DEFAULT_DB_NAME) + "/" + string(file_name) + string(TABLE_DATAMATCH_SUFFIX);
	fstream matchfile;
	matchfile.open(dbmatchfile, ios::in | ios::out | ios::binary);
	
	//����Ļ������⣬��Ϊframe��relationName����ĳ�NULL��
	DMATCH* data_match = new DMATCH;
	data_match->page_num = bp_manager_.frame[frame_num].page.page_num;
	data_match->state = bp_manager_.frame[frame_num].state;
	strcpy(data_match->table_name, bp_manager_.frame[frame_num].relation_name);
	//cout << "DMATCH : TABLE NAME  " << data_match->table_name << endl;
	matchfile.seekp(page_num * sizeof(DMATCH), ios::beg);
	matchfile.write((char*)data_match, sizeof(DMATCH));

	//�ָ�frame״̬
	strcpy(bp_manager_.frame[frame_num].relation_name, "NULL");
	bp_manager_.frame[frame_num].dirty = false;
	bp_manager_.frame[frame_num].state = UNFULL;
	bp_manager_.allocated[frame_num] = false;
	return RC::SUCCESS;
}

RC DiskBufferPool::flush_pages(const char* file_name)
{
	
	for (int i = 0; i < bp_manager_.size; i++)
	{
		if (bp_manager_.allocated[i] == false)
			continue;
		flush_this_page(file_name, i);
	}
	return RC::SUCCESS;
}

RC DiskBufferPool::find_all_row(const char* table_name, int frame_num, int row_size, unordered_map<int, char*>& slot_row)
{
	bp_manager_.find_all_row(table_name, frame_num, row_size, slot_row);
	return RC::SUCCESS;
}

RC DiskBufferPool::insert_row(const char* file_name, const char* table_name, int frame_num, char* data, int row_size, int& page_num, int& slot_num)
{
	bp_manager_.insert_row(file_name, table_name, frame_num, data, row_size, page_num, slot_num);
	//cout << "DiskBufferPool BPMANAGER FRAME " << frame_num << " " <<page_num<< bp_manager_.frame[frame_num].dirty << endl;
	return RC::SUCCESS;
}

void DiskBufferPool::TEST_LRU()
{
}

BPManager DiskBufferPool::get_BPManager()
{
	return bp_manager_;
}

RC BPManager::insert_row(const char* file_name, const char* table_name, int frame_num, char* data, int row_size, int& page_num, int& slot_num)
{
	if (allocated[frame_num] == false)
	{
		cout << "ERROR! ��ͼ����δ�����FRAME" << endl;
		return RC::GENERIC_ERROR;
	}
	//����������еõ���frame_num��ҳ���ҿ���λ��
	int max = BP_PAGE_DATA_SIZE / (sizeof(int) + row_size);
	cout << "���� " << max << endl;
	int offset = 0;
	//��û�з�����Page
	if (frame[frame_num].state == FULL)
	{
		cout << "STATE��ʾ���Ѿ�����" << endl;
	}
	if (frame[frame_num].state == UNFULL)
	{
		if (frame[frame_num].page.numofslot == frame[frame_num].page.maxofslot + 1)
		{
			offset = frame[frame_num].page.maxofslot + 1;
			cout << "�ҵ��ĵ�һ������slot�� " << offset << endl;
			frame[frame_num].page.maxofslot++;
			frame[frame_num].page.numofslot++;
		}
		else if (frame[frame_num].page.numofslot < frame[frame_num].page.maxofslot + 1)
		{
			for (int i = 0; i <= frame[frame_num].page.maxofslot; i++)
			{
				//��length���ǲ���-1���ǣ�ɾ���Ŀ���λ��

				//maxofslot����
				frame[frame_num].page.numofslot++;
			}
		}
	}
	else
	{
		cout << "�Ѿ�����" << endl;
		return RC::GENERIC_ERROR;
	}
	memcpy(frame[frame_num].page.data + offset * (sizeof(int) + row_size), (void *)&row_size, sizeof(int));

	memcpy(frame[frame_num].page.data + offset * (sizeof(int) + row_size) + sizeof(int), data, row_size);

	frame[frame_num].dirty = true;
	if (frame[frame_num].page.numofslot >= max)
	{
		frame[frame_num].state = FULL;
	}
	//isFull(table_name, frame_num, row_size);	//�޸�frame.state
	frame[frame_num].acc_time = GetCurTime();
	page_num = frame[frame_num].page.page_num;
	slot_num = offset;
	return RC::SUCCESS;
}

//���ʣ��޸�acctime
RC BPManager::find_all_row(const char* table_name, int frame_num, int row_size, unordered_map<int, char*>& slot_row)
{
	if (allocated[frame_num] == false)
	{
		cout << "ERROR!��δ�����FRAME�в���" << endl;
		return RC::GENERIC_ERROR;
	}
	if (strncmp(table_name, frame[frame_num].relation_name, 20))
	{
		cout << "ERROR USE FIND ALL ROW! NOT MATCH TABLENAME!" << endl;
		return RC::GENERIC_ERROR;    //  ����true������������
	}
	char* data = frame[frame_num].page.data;
	// size: ÿ��slot�ĳ��ȣ�����length �� ����
	int size = sizeof(int) + row_size;
	char* row;
	int* length;	//��¼״̬��-1���Ѿ�ɾ����>0��������row
	for (int i = 0; i <= frame[frame_num].page.maxofslot; i++)
	{
		length = new int;
		memcpy((char*)length, data + i * size, sizeof(int));
		cout << "\tslot : " << i << "\t״̬��" << *length << endl;
		if (*length != -1 && *length == row_size)
		{
			row = new char[row_size];
			memcpy(row, data + i * size + sizeof(int), row_size);
			slot_row[i] = row;
		}
		
	}
	frame[frame_num].acc_time = GetCurTime();
	
	return RC::SUCCESS;
}
