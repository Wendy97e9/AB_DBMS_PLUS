#include "record.h"

RecordHandler::RecordHandler()
{
    data_buffer_pool_ = theGlobalDiskBufferPool();
    //printf("---------data_buffer_pool_      %p", data_buffer_pool_);
}

RC RecordHandler::insert_record(const char* file_name, const char* table_name, char* record, int record_size, RID& rid)
{
    //1 ���ڴ��е�ҳ�棬���޶�Ӧ���������п���λ�õ�ҳ
    //2 û�У�������е�ҳ����û��δ���ģ�û�У���ô����һ���µ�ҳ��allocate_page
    //3 �ҵ�һ��λ�ã�����record
    //
    //
    int frame_num=-1;
    //�һ�������Page����û�ж�Ӧ��������Frame
    //vector<int> vec_frame_num;
    //data_buffer_pool_->get_this_page_by_name(file_name, table_name, vec_frame_num);

    data_buffer_pool_->get_free_page(file_name, table_name, record_size, frame_num);
    if (frame_num != -1)
    {
        cout << "���������п���Page " << endl;
        data_buffer_pool_->insert_row(file_name, table_name, frame_num, record, record_size, rid.page_num, rid.slot_num);
        //cout << "RID " << rid.page_num << " " << rid.slot_num << endl;
        return RC::SUCCESS;
    }



    //��DMATCH������û��������Ӧ��ҳ������Ϊ��
    //��table_name=?������pid����state����0��1
    vector<DMATCH*> vec_data_match;
    data_buffer_pool_->find_all_page(file_name, table_name, vec_data_match);
    for (int i = 0; i < vec_data_match.size(); i++)
    {
        if (vec_data_match[i]->state == UNFULL)
        {
            data_buffer_pool_->get_this_page(file_name, table_name, vec_data_match[i]->page_num, frame_num);
            break;
        }
    }
    if (frame_num != -1)
    {
        cout << "���������Ѿ�����Ŀ���Page " << endl;
        data_buffer_pool_->insert_row(file_name, table_name, frame_num, record, record_size, rid.page_num, rid.slot_num);
        //cout << "RID " << rid.page_num << " " << rid.slot_num << endl;
        return RC::SUCCESS;
    }


    data_buffer_pool_->allocate_page(file_name, table_name, frame_num);//Frame��
    //���ϲ���ֻ��Ҫ��ȡ��frame_num�Ϳ����ˣ�ͨ��insert_row����ȡ��Ӧ�Ĳ����Rowid
    //table_name frame_num record record_size slot_num
    //
    cout << "�����·���Page " << endl;
    data_buffer_pool_->insert_row(file_name, table_name, frame_num, record, record_size, rid.page_num, rid.slot_num);
    //cout << "RID " << rid.page_num << " " << rid.slot_num << endl;
                                                                
    //BPManager bp_manager_ = data_buffer_pool_->get_BPManager();
    ////����������еõ���frame_num��ҳ���ҿ���λ��
    //int max = BP_PAGE_DATA_SIZE / record_size;
    //cout << "���� " << max << endl;
    //int offset = 0;
    //if (bp_manager_.frame[frame_num].page.numofslot < max)
    //{
    //    if (bp_manager_.frame[frame_num].page.numofslot == bp_manager_.frame[frame_num].page.maxofslot + 1)
    //    {
    //        offset = bp_manager_.frame[frame_num].page.maxofslot + 1;
    //        cout << "�ҵ��ĵ�һ������λ����" << offset;
    //    }
    //    else if (bp_manager_.frame[frame_num].page.numofslot < bp_manager_.frame[frame_num].page.maxofslot + 1)
    //    {
    //        for (int i = 0; i <= bp_manager_.frame[frame_num].page.maxofslot; i++)
    //        {
    //            //��state���ǲ���-1���ǣ�ɾ���Ŀ���λ��
    //        }
    //    }
    //}
    //memcpy(bp_manager_.frame[frame_num].page.data + offset * record_size, record, record_size);
    //rid.page_num = frame_num;
    //rid.slot_num = offset;


    return RC::SUCCESS;
}
