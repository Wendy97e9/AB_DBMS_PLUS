#include "disk_buffer_pool.h"
#include "default_handler.h"
#include "parse_defs.h"
#include "bplustree_index.h"

void test()
{
	//int a = 1;
	//char* data = new char[10];
	//memcpy(data, (void*)&a, sizeof(int));
	//int* b;
	//b = new int;
	//memcpy(b, data, sizeof(int));
	//cout << *b << endl;
	//delete b;
	//delete[] data;

	int a = 1;
	void* p=(int*)&a;
	char* data = new char[10];
	memcpy(data, p, sizeof(int));
	int* b = new int;
	memcpy(b, data, sizeof(int));
	cout << *(int *)p << endl;	//1
	char c[10];
	memcpy(c, data, sizeof(int));
	cout << *b << endl;	//1
	cout << c << endl;	//乱码
	delete b;
	delete[] data;

	
}

void bplustree_test()
{
	BplusTreeIndex* bpt = new BplusTreeIndex();
	vector < Block* > Blocks;
	bpt->insertNode(bpt->rootBlock, INTS, 4, "4");
	Blocks.clear();
	Blocks.push_back(bpt->rootBlock);
	bpt->print(Blocks);
	bpt->insertNode(bpt->rootBlock, INTS, 4, "7");
	Blocks.clear();
	Blocks.push_back(bpt->rootBlock);
	bpt->print(Blocks);
	bpt->insertNode(bpt->rootBlock, INTS, 4, "2");
	Blocks.clear();
	Blocks.push_back(bpt->rootBlock);
	bpt->print(Blocks);	
	bpt->insertNode(bpt->rootBlock, INTS, 4, "5");
	Blocks.clear();
	Blocks.push_back(bpt->rootBlock);
	bpt->print(Blocks);	
	bpt->insertNode(bpt->rootBlock, INTS, 4, "6");
	Blocks.clear();
	Blocks.push_back(bpt->rootBlock);
	bpt->print(Blocks);	
	bpt->insertNode(bpt->rootBlock, INTS, 4, "3");
	Blocks.clear();
	Blocks.push_back(bpt->rootBlock);
	bpt->print(Blocks);	
	bpt->insertNode(bpt->rootBlock, INTS, 4, "1");
	Blocks.clear();
	Blocks.push_back(bpt->rootBlock);
	bpt->print(Blocks);
}
void func_test()
{
	DiskBufferPool* bp = theGlobalDiskBufferPool();
	bp->create_file("db");
	DefaultHandler* dh = new DefaultHandler();
	//for (int i = 0; i < 200; i++)
	//{
	//	dh->insert_record();
	//}
	int op = -1;
	while (op != 0)
	{
		cout << "****************************************" << endl;
		cout << "********** 选 择 操 作 **********" << endl;
		cout << "* 1	CREATE TABLE" << endl;
		cout << "* 2	INSERT RECORD" << endl;
		cout << "* 3	READ BUFFER DATA" << endl;
		cout << "* 4	FLUSH ALL PAGES" << endl;
		cout << "* 5	SELECT TABLE *" << endl;
		cout << "* 6	READ DMATCH FILE" << endl;
		cout << "* 7	CREATE INTEX" << endl;
		cout << "* 0	EXIT" << endl;
		cout << "*********************************" << endl;
		cin >> op;
		switch (op) {
		case 0:
			bp->flush_pages("db");
			break;
		case 1:
			dh->create_table();
			break;
		case 2:
			dh->insert_record();
			break;
		case 3:
			dh->TEST_READ_BUFFER_DATA();
			break;
		case 4:
			bp->flush_pages("db");
			break;
		case 5:
			//读取一个表的所有数据
			dh->select_table();
			
			break;
		case 6:
			bp->read_DMATCH("db");
			break;
		case 7:
			dh->create_index();
			break;
			
		}
	}
	delete dh;
	delete bp;
}

int main()
{
	//test();
	cout << sizeof(Block) << endl;
	//bplustree_test();
	func_test();
	//DiskBufferPool* bp = theGlobalDiskBufferPool();
	////printf("---------data_buffer_pool_      %p", bp);

	////bp->test("test.data");

	////测试缓冲区及磁盘部分
	//bp->create_file("db");
	////int i;
	////bp->allocate_page("db","1332",i);
	////cout << "i " << i << endl;
	////vector<DMATCH*> vec_data_match;
	////bp->find_all_page("db", "1331", vec_data_match);
	////cout << "main " << vec_data_match[0]->page_num << endl;
	//DefaultHandler* dh = new DefaultHandler;
	////dh->create_table();
	//dh->insert_record();
	//dh->insert_record();	
	//dh->insert_record();
	//dh->insert_record();
	//dh->insert_record();

	////测试数据字典部分
	////dh->create_table();
	//

	//dh->TEST_READ_BUFFER_DATA();
	//delete dh;
	//bp->flush_pages("db");
	//delete bp;
	
	return 0;
}
