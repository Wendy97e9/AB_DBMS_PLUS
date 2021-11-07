#include "record.h"

RecordHandler::RecordHandler()
{
    data_buffer_pool_ = theGlobalDiskBufferPool();
    //printf("---------data_buffer_pool_      %p", data_buffer_pool_);
}

RC RecordHandler::insert_record(const char* file_name, const char* table_name, char* record, int record_size, RID& rid)
{
    //1 找内存中的页面，有无对应于这个表的有空闲位置的页
    //2 没有：找外存中的页，有没有未满的，没有：那么分配一个新的页面allocate_page
    //3 找到一个位置，插入record
    //
    //
    int frame_num = -1;
    //找缓冲区的Page，有没有对应于这个表的Frame
    //vector<int> vec_frame_num;
    //data_buffer_pool_->get_this_page_by_name(file_name, table_name, vec_frame_num);

    data_buffer_pool_->get_free_page(file_name, table_name, record_size, frame_num);
    if (frame_num != -1)
    {
        cout << "缓冲区中有空闲Page " << endl;
        data_buffer_pool_->insert_row(file_name, table_name, frame_num, record, record_size, rid.page_num, rid.slot_num);
        
        //cout << "RID " << rid.page_num << " " << rid.slot_num << endl;
        return RC::SUCCESS;
    }



    //找DMATCH，看有没有这个表对应的页，并且为空,
    //!!!!!!并且此时不在缓冲区中，否则会发生错误，都以为是空闲页，都往里写了数据
    //找table_name=?的所有pid，看state，找0、1
    vector<DMATCH*> vec_data_match;
    data_buffer_pool_->find_all_page(file_name, table_name, vec_data_match);
    for (int i = 0; i < vec_data_match.size(); i++)
    {
        if (vec_data_match[i]->state == UNFULL)
        {
            //要判断frame情况，是不是全部都已经分配了
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!判断在不在缓冲区
            if (data_buffer_pool_->inBuffer(vec_data_match[i]->page_num))
            {
                continue;
            }
            data_buffer_pool_->get_this_page(file_name, table_name, vec_data_match[i]->page_num, frame_num);
            break;
        }
    }
    if (frame_num != -1)
    {
        cout << "磁盘中有已经分配的空闲Page " << endl;
        data_buffer_pool_->insert_row(file_name, table_name, frame_num, record, record_size, rid.page_num, rid.slot_num);
        //cout << "RID " << rid.page_num << " " << rid.slot_num << endl;
        return RC::SUCCESS;
    }


    data_buffer_pool_->allocate_page(file_name, table_name, frame_num);//Frame中
    //以上部分只需要获取到frame_num就可以了，通过insert_row来获取对应的插入的Rowid
    //table_name frame_num record record_size slot_num
    //
    cout << "磁盘新分配Page " << endl;
    data_buffer_pool_->insert_row(file_name, table_name, frame_num, record, record_size, rid.page_num, rid.slot_num);
    //cout << "RID " << rid.page_num << " " << rid.slot_num << endl;
                                                                
    //BPManager bp_manager_ = data_buffer_pool_->get_BPManager();
    ////从以上情况中得到的frame_num的页面找空闲位置
    //int max = BP_PAGE_DATA_SIZE / record_size;
    //cout << "最多放 " << max << endl;
    //int offset = 0;
    //if (bp_manager_.frame[frame_num].page.numofslot < max)
    //{
    //    if (bp_manager_.frame[frame_num].page.numofslot == bp_manager_.frame[frame_num].page.maxofslot + 1)
    //    {
    //        offset = bp_manager_.frame[frame_num].page.maxofslot + 1;
    //        cout << "找到的第一个空闲位置是" << offset;
    //    }
    //    else if (bp_manager_.frame[frame_num].page.numofslot < bp_manager_.frame[frame_num].page.maxofslot + 1)
    //    {
    //        for (int i = 0; i <= bp_manager_.frame[frame_num].page.maxofslot; i++)
    //        {
    //            //看state，是不是-1，是：删除的空闲位置
    //        }
    //    }
    //}
    //memcpy(bp_manager_.frame[frame_num].page.data + offset * record_size, record, record_size);
    //rid.page_num = frame_num;
    //rid.slot_num = offset;


    return RC::SUCCESS;
}
