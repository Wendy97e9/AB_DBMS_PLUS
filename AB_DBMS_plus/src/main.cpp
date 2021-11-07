#include "disk_buffer_pool.h"
#include "default_handler.h"
#include "parse_defs.h"

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
	cout << c << endl;	//ÂÒÂë
	delete b;
	delete[] data;

	
}

void create_data()
{

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
		cout << "********** Ñ¡ Ôñ ²Ù ×÷ **********" << endl;
		cout << "* 1	CREATE TABLE" << endl;
		cout << "* 2	INSERT RECORD" << endl;
		cout << "* 3	READ BUFFER DATA" << endl;
		cout << "* 4	FLUSH ALL PAGES" << endl;
		cout << "* 5	TEST LRU" << endl;
		cout << "* 6	READ DMATCH FILE" << endl;
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
			//²âÊÔLRU
			break;
		case 6:
			bp->read_DMATCH("db");
			
		}
	}
	delete dh;
	delete bp;
}

int main()
{
	//test();
	func_test();
	//DiskBufferPool* bp = theGlobalDiskBufferPool();
	////printf("---------data_buffer_pool_      %p", bp);

	////bp->test("test.data");

	////²âÊÔ»º³åÇø¼°´ÅÅÌ²¿·Ö
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

	////²âÊÔÊý¾Ý×Öµä²¿·Ö
	////dh->create_table();
	//

	//dh->TEST_READ_BUFFER_DATA();
	//delete dh;
	//bp->flush_pages("db");
	//delete bp;
	
	return 0;
}