#pragma once
#ifndef _DB_H_
#define _DB_H_

#include <unordered_map>

#include "rc.h"
#include "parse_defs.h"
#include "table.h"

class Database
{
public:
	Database();

	Table* find_table(const char* table_name);

	RC create_table(CreateTable* ct);

	RC default_init();

	string get_dbpath();

	string get_dbname();

	

public:
	RC open_all_tables();

	void print_all_opened_tables();
	
private:
	string dbname_;
	string dbpath_;
	unordered_map<string, Table*>  opened_tables_;

};


#endif