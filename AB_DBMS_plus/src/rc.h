#pragma once

#ifndef __RC_H__
#define __RC_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <io.h>
//#include <stdafx.h>
#include <windows.h>
#include <time.h>

using namespace std;

#define DEFAULT_SAVE_PATH "abdbmsplus/db/"
#define DEFAULT_DB_PATH "abdbmsplus"
#define DEFAULT_DB_NAME "db"


static const char* TABLE_META_SUFFIX = ".table";
static const char* TABLE_META_FILE_PATTERN = ".*\\.table$";
static const char* TABLE_DATA_SUFFIX = ".data";
static const char* TABLE_INDEX_SUFFIX = ".index";
static const char* TABLE_DATAMATCH_SUFFIX = ".dmatch";

// 暂时两种状态，成功：0，不成功：1
enum RC {
	SUCCESS = 0,
	GENERIC_ERROR = 1
};

enum STATE {
	UNFULL = 0,
	FULL = 1
};

typedef struct
{
	int		page_num;
	char	table_name[20];
	STATE	state;	//空，半满，满。。。0 1 2
}DMATCH;

extern bool isExist(const char* filename);

void GetAllFormatFiles(string path, vector<string>& files, string format);

unsigned int GetCurTime();
#endif