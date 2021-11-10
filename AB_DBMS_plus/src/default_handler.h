#pragma once
#ifndef _DEFAULT_HANDLER_H_
#define _DEFAULT_HANDLER_H_

#include "rc.h"
#include "parse_defs.h"
#include "db.h"

class DefaultHandler
{
	// ʹ��Ĭ�ϵ�db ../abdbmsplus/db
public:
	DefaultHandler();

	RC create_table();

	//����table���������е�Ԫ��
	RC select_table();
	Table* find_table(const char* dbname, const char* table_name) const;

	//���Զ�ȡ��ǰ�������Ķ�Ӧtable_name�����е�Page
	void TEST_READ_BUFFER_DATA();

	void TEST_LRU();
	
	RC insert_record();
	RC delete_record();
	RC find_record();

	RC create_index();
	RC drop_index();
public:
	Database* db;

};
#endif // !_DEFAULT_HANDLER_H_



