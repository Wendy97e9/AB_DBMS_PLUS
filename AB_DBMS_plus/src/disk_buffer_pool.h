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

#define BP_INVALID_PAGE_NUM (-1)	//��Чҳ -1
//#define BP_PAGE_SIZE (1 << 12)		// 4096
#define BP_PAGE_SIZE (1 << 7)		// 128
#define BP_BUFFER_SIZE 5			//����ش�С
#define BP_PAGE_DATA_SIZE (BP_PAGE_SIZE - sizeof(PageNum) - 2*sizeof(int))
#define MAX_OPEN_FILE 1024

//vector<int> LRUCache;

typedef struct {
    PageNum page_num;
    int numofslot;  //0:������0���ۣ�numofslot����С��maxofslot�����ڻ���ɾ��
    int maxofslot;  //���ۺţ�-1������Page,0:���ۺ���0��������1����
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
    //        return true;    //  ����true������������
    //    }
    //    int max = BP_PAGE_DATA_SIZE / (sizeof(int) + row_size);
    //    if (frame[frame_num].page.numofslot < max)
    //    {
    //        cout << "�ѷ���۵���Ŀ��" << frame[frame_num].page.numofslot << endl;
    //        frame[frame_num].state = UNFULL;
    //        return false;
    //    }
    //    frame[frame_num].state = FULL;  //��ʧ����
    //    return true;
    //}

    //�ж�pid�ǲ����Ѿ������˻�����
    // �ǣ�����ҳ��ţ��񣬷���0
    int inBuffer(PageNum page_num)
    {
        for (int i = 0; i < size; i++)
        {
            if (allocated[i] == false)
                continue;
            if (frame[i].page.page_num == page_num)
            {
                return i;    //�Ѿ��ڻ���������
            }
        }
        return -1;
    }

    //ǰ�᣺buffer����
    //���أ�Ҫ��̭��frame_num
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
        cout << "������̭ FRAME " << free_frame_num << endl;
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
    * ����һ������Ϊָ���ļ����ķ�ҳ�ļ�
    */
    RC create_file(const char* file_name);

    /**
     * �����ļ�����һ����ҳ�ļ��������ļ�ID
     * @return
     */
    RC open_file(const char* file_name, int* file_id);

    /**
     * �����ļ�����ҳ�Ż�ȡָ��ҳ�浽������������ҳ����ָ�롣
     * @return
     */
    RC get_this_page(const char* file_name, const char * table_name, PageNum page_num, int& frame_num);

    /**
     * ��ָ���ļ��з���һ���µ�ҳ�棬��������뻺����������ҳ����ָ�롣
     * ����ҳ��ʱ������ļ����п���ҳ����ֱ�ӷ���һ������ҳ��
     * ����ļ���û�п���ҳ������չ�ļ���ģ�������µĿ���ҳ��
     */
    RC allocate_page(const char* file_name, const char* table_name, int& frame_num);

    RC read_DMATCH(const char* file_name);

    // ����table�ļ������ڴ��е�Page
    RC get_this_page_by_name(const char* file_name, const char* relation_name, vector<int>& vec_frame_num);

    //����table�ļ������ڴ��е�UNFULL Page
    RC get_free_page(const char* file_name, const char* table_name, int row_size, int& frame_num);

    //����table_name���ҵ����е�page_num�����ж�Ӧ��״̬
    RC find_all_page(const char* file_name, const char* relation_name, vector<DMATCH*>& vec_data_match);

    //��ָ��ҳ���Pageд�ش���
    RC flush_this_page(const char* file_name, int frame_num);

    //�����е�ҳд������
    RC flush_pages(const char* file_name);

    //����frame_num��ֵ���ҵ����е�row
    RC find_all_row(const char* table_name, int frame_num, int row_size, unordered_map<int, char*>& slot_row);
   
    //�ж��Ƿ�һ��page�Ѿ���buffer��
    int inBuffer(PageNum page_num)
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

//���ˣ�frame_num>=BP_BUFFER_SIZE��˵����Ҫ�û�Page������Ҫ�û���
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

