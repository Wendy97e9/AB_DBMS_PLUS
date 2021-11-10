#pragma once

#ifndef _TABLE_H_
#define _TABLE_H_

#include "meta.h"
#include "disk_buffer_pool.h"
#include "record.h"
#include "bplustree_index.h"

class Table
{
public:
	Table();

	const char* name() const;

	RC create(const char* table_path_name, CreateTable* ct);

	RC create_index(const char* file_name, const char* table_name, CreateIndex* ci);

	RC select_all_rid(const char* file_name, const char* table_name, char* attribute_name);

	RC select_all(const char* file_name, const char* table_name);

	RC open(const char* table_path_name);

	RC insert_record(int value_num, const Value* values);

	RC make_record(int value_num, const Value* values, char*& record_out);

	int find_key(char* attribute_name, char* record);

	//根据table_meta的attributes，解析record
	RC analyze_record(char* record);

	RC analyze_all_record(vector<char*> record);

	TableMeta get_table_meta();

	void TEST_READ_BUFFER_DATA(const char* dbname, const char* table_name);

private:
	DiskBufferPool* data_buffer_pool_;
	TableMeta table_meta_;
	RecordHandler* record_handler_;
};

#endif
