#pragma once
#ifndef _DEFAULT_HANDLER_H_
#define _DEFAULT_HANDLER_H_

#include "rc.h"
#include "parse_defs.h"
#include "db.h"

class DefaultHandler
{
	// 使用默认的db ../abdbmsplus/db
public:
	DefaultHandler();

	RC create_table();
	Table* find_table(const char* dbname, const char* table_name) const;

	//测试读取当前缓冲区的对应table_name的所有的Page
	void TEST_READ_BUFFER_DATA();
	
	RC insert_record();
	RC delete_record();
	RC find_record();

	RC create_index();
	RC drop_index();
public:
	Database* db;

};
#endif // !_DEFAULT_HANDLER_H_



