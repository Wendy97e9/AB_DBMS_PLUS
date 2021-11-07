#include "db.h"

Database::Database()
{
    default_init();
}

Table* Database::find_table(const char* table_name)
{
    std::unordered_map<std::string, Table*>::const_iterator iter = opened_tables_.find(table_name);
    if (iter != opened_tables_.end()) {
        return iter->second;
    }
    return nullptr;
}

RC Database::create_table(CreateTable* ct)
{
    if (isExist(ct->relation_name))
    {
        
        return RC::GENERIC_ERROR;
    }
    Table* table = new Table();
    string table_path_name = dbpath_ + "/" + dbname_ + "/" + ct->relation_name;
    //cout << "table_path_name  " << table_path_name << endl;
    table->create(table_path_name.c_str(),ct);
    //cout << "opened_tables_[table->name()] " << table->name() << endl;
    opened_tables_[table->name()] = table;
    //if (opened_tables_.find(table->name())!=opened_tables_.end())
    //{
    //    cout << "FOUND!" << endl;
    //}
    //else
    //{
    //    cout << "NOT FOUND!" << endl;
    //}
    //print_all_opened_tables();
    
    return RC::SUCCESS;
}

RC Database::default_init()
{
    dbpath_ = "abdbmsplus";
    dbname_ = "db";
    //return RC::SUCCESS;
    return open_all_tables();
}

string Database::get_dbpath()
{
    
    return dbpath_;
}

string Database::get_dbname()
{
    return dbname_;
}

RC Database::open_all_tables()
{
    string filePath = dbpath_+"/"+dbname_;
    vector<string> files;
    //��ȡ���и�ʽΪ.table���ļ�
    string format = ".table";
    GetAllFormatFiles(filePath, files, format);
    int size = files.size();
    for (int i = 0; i < size; i++)
    {
        //cout << ".table  " << files[i] << endl;
        // ��ȡ��table�У�Ȼ�����opened_table_
        Table* table = new Table();
        table->open(files[i].c_str());
        opened_tables_[table->name()] = table;
    }
    return RC::SUCCESS;
}

void Database::print_all_opened_tables()
{
    cout << "�����Ѿ��򿪵�table : " << endl;
    for (auto& it : opened_tables_)
    {
        cout << it.first << " " << endl;
    }
    cout << "��ӡ��ϣ�" << endl;
}
