#pragma once
#ifndef _DISK_BUFFER_POOL_
#define _DISK_BUFFER_POOL_

#include <fcntl.h>
#include <iostream>
#include <sys/types.h>

#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <vector>
#include <fstream>
#include <sstream>

#include <unordered_map>



#include "rc.h"



using namespace std;

typedef int PageNum;	//PID

#define BP_INVALID_PAGE_NUM (-1)	//无效页 -1
//#define BP_PAGE_SIZE (1 << 12)		// 4096
#define BP_PAGE_SIZE (1 << 7)		// 128
#define BP_BUFFER_SIZE 5			//缓冲池大小
#define BP_PAGE_DATA_SIZE (BP_PAGE_SIZE - sizeof(PageNum) - 2*sizeof(int))
#define MAX_OPEN_FILE 1024

//vector<int> LRUCache;

typedef struct {
    PageNum page_num;
    int numofslot;  //0:分配了0个槽，numofslot可能小于maxofslot，由于会有删除
    int maxofslot;  //最大槽号：-1：空闲Page,0:最大槽号是0，分配了1个槽
    char data[BP_PAGE_DATA_SIZE];
} Page;
// sizeof(Page) should be equal to BP_PAGE_SIZE

typedef struct {
    PageNum page_count;
    int allocated_pages;
} BPFileSubHeader;

typedef struct {
    bool dirty;
    //unsigned int pin_count;
    unsigned long acc_time;
    char* relation_name;
    //int file_desc;
    STATE state;
    Page page;
} Frame;

//typedef struct {
//    bool open;
//    Frame* frame;
//} BPPageHandle;



class BPManager {
public:
    BPManager(int size = BP_BUFFER_SIZE) {
        this->size = size;
        frame = new Frame[size];
        allocated = new bool[size];
        for (int i = 0; i < size; i++) {
            allocated[i] = false;
        }
    }

    ~BPManager() {
        delete[] frame;
        delete[] allocated;
        size = 0;
        frame = nullptr;
        allocated = nullptr;
    }

    Frame* alloc() {
        return nullptr; // TODO for test
    }

    Frame* get(int file_desc, PageNum page_num) {
        return nullptr; // TODO for test
    }

    Frame* getFrame() { return frame; }

    bool* getAllocated() { return allocated; }

    RC insert_row(const char* file_name, const char* table_name, int frame_num, char* data, int row_size, int& page_num, int& slot_num);

    RC find_all_row(const char* table_name, int frame_num, int row_size, unordered_map<int, char*>& slot_row);

    //bool isFull(const char* table_name, int frame_num, int row_size)
    //{
    //    if (strncmp(table_name , frame[frame_num].relation_name,20))
    //    {
    //        cout << "NOT MATCH TABLENAME!" << endl;
    //        return true;    //  返回true，不让他插入
    //    }
    //    int max = BP_PAGE_DATA_SIZE / (sizeof(int) + row_size);
    //    if (frame[frame_num].page.numofslot < max)
    //    {
    //        cout << "已分配槽的数目：" << frame[frame_num].page.numofslot << endl;
    //        frame[frame_num].state = UNFULL;
    //        return false;
    //    }
    //    frame[frame_num].state = FULL;  //改失败了
    //    return true;
    //}

    //判断pid是不是已经载入了缓冲区
    bool inBuffer(PageNum page_num)
    {
        for (int i = 0; i < size; i++)
        {
            if (allocated[i] == false)
                continue;
            if (frame[i].page.page_num == page_num)
            {
                return true;    //已经在缓冲区里了
            }
        }
        return false;
    }

    //前提：buffer已满
    //返回，要淘汰的frame_num
    int doLRU()
    {
        int min = frame[0].acc_time;
        int free_frame_num = 0;
        for (int i = 0; i < size; i++)
        {
            if (allocated[i] == false)
            {
                cout << "ERROR BUFFER NOT FULL" << endl;
                return -1;
            }
            if (frame[i].acc_time < min)
            {
                min = frame[i].acc_time;
                free_frame_num = i;
            }
        }
        cout << "即将淘汰 FRAME " << free_frame_num << endl;
        return free_frame_num;
    }

    void print_all_frame()
    {
        cout << "***************** PRINT ALL FRAME *****************" << endl;
        for (int i = 0; i < size; i++)
        {
            if (allocated[i] == false)
                continue;
            cout << "FRAME " << i << "\t" << frame[i].page.page_num << "\t" << frame[i].acc_time << endl;
            
        }
    }

public:
    int size;
    Frame* frame = nullptr;
    bool* allocated = nullptr;
};

class DiskBufferPool {
public:
    RC test(const char* file_name);
    /**
    * 创建一个名称为指定文件名的分页文件
    */
    RC create_file(const char* file_name);

    /**
     * 根据文件名打开一个分页文件，返回文件ID
     * @return
     */
    RC open_file(const char* file_name, int* file_id);

    /**
     * 根据文件名和页号获取指定页面到缓冲区，返回页面句柄指针。
     * @return
     */
    RC get_this_page(const char* file_name, const char * table_name, PageNum page_num, int& frame_num);

    /**
     * 在指定文件中分配一个新的页面，并将其放入缓冲区，返回页面句柄指针。
     * 分配页面时，如果文件中有空闲页，就直接分配一个空闲页；
     * 如果文件中没有空闲页，则扩展文件规模来增加新的空闲页。
     */
    RC allocate_page(const char* file_name, const char* table_name, int& frame_num);

    RC read_DMATCH(const char* file_name);

    // 根据table文件名找内存中的Page
    RC get_this_page_by_name(const char* file_name, const char* relation_name, vector<int>& vec_frame_num);

    //根据table文件名找内存中的UNFULL Page
    RC get_free_page(const char* file_name, const char* table_name, int row_size, int& frame_num);

    //根据table_name，找到所有的page_num，还有对应的状态
    RC find_all_page(const char* file_name, const char* relation_name, vector<DMATCH*>& vec_data_match);

    //将指定页框的Page写回磁盘
    RC flush_this_page(const char* file_name, int frame_num);

    //将所有的页写到磁盘
    RC flush_pages(const char* file_name);

    //根据frame_num的值，找到所有的row
    RC find_all_row(const char* table_name, int frame_num, int row_size, unordered_map<int, char*>& slot_row);
   
    //判断是否一个page已经在buffer中
    bool inBuffer(PageNum page_num)
    {
        return bp_manager_.inBuffer(page_num);
    }

    //table_name frame_num record record_size slot_num
    //
    RC insert_row(const char* file_name, const char* table_name, int frame_num, char* data, int row_size, int& page_num, int& slot_num);

    void TEST_LRU();
private:
    BPManager get_BPManager();
    
private:
    BPManager bp_manager_;

};

DiskBufferPool* theGlobalDiskBufferPool();

//满了，frame_num>=BP_BUFFER_SIZE，说明需要置换Page，返回要置换的
//int SortFrame(int frame_num)
//{
//    if (LRUCache.size() < BP_BUFFER_SIZE)
//    {
//        LRUCache.push_back(frame_num);
//    }
//    else
//    {
//        bool flag = false;
//        vector<int>::iterator iter = LRUCache.begin();
//        for (iter=LRUCache.begin(); iter!=LRUCache.end(); iter++)
//        {
//            if (*iter == frame_num)
//            {
//                flag = true;
//                break;
//            }
//        }
//        if (flag)
//        {
//            LRUCache.erase(iter);
//            LRUCache.push_back(frame_num);
//        }
//        else
//        {
//            int tmp = LRUCache.front();
//            LRUCache.erase(LRUCache.begin());
//            LRUCache.push_back(tmp);
//            return tmp;
//        }
//    }
//    return BP_BUFFER_SIZE;
//
//}


#endif // !_DISK_BUFFER_POOL_

