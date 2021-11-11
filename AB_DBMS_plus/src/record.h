#pragma once

#ifndef _RECORD_H_
#define _RECORD_H_

#include "disk_buffer_pool.h"

typedef int SlotNum;
struct PageHeader;
class ConditionFilter;

struct RID
{
    PageNum page_num; // record's page number
    SlotNum slot_num; // record's slot number
    // bool    valid;    // true means a valid record

    bool operator== (const RID& other) const {
        return page_num == other.page_num && slot_num == other.slot_num;
    }
};

//class RidDigest {
//public:
//    size_t operator() (const RID& rid) const {
//        return ((size_t)(rid.page_num) << 32) | rid.slot_num;
//    }
//};

struct Record
{
    // bool valid; // false means the record hasn't been load
    RID  rid;   // record's rid
    char* data; // record's data
};

class RecordHandler
{
public:
    RecordHandler();
    RC insert_record(const char* file_name, const char* table_name, char* record,int record_size,RID &rid);

    


private:
    DiskBufferPool* data_buffer_pool_;
};
#endif
