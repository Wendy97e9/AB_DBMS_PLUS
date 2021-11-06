#include "meta.h"

//没有初始化索引，可以加参数CreateIndex* ci
RC TableMeta::init(CreateTable* ct)
{
    name_ = ct->relation_name;
    record_size_ = 0;
    for (int i = 0; i < ct->attribute_count; i++)
    {
        AttrInfo* attr = new AttrInfo;
        //attr->name = ct->attributes[i].name;
        strcpy(attr->name, ct->attributes[i].name);
        //cout << "init tablemeta attrname" << attr->name << endl;
        attr->type = ct->attributes[i].type;
        attr->length = ct->attributes[i].length;
        attributes.push_back(attr);
        record_size_ += attr->length;
    }
    return RC::SUCCESS;
}

const char* TableMeta::name() const {
    return name_.c_str();
}

int TableMeta::record_size()
{
    return record_size_;
}

vector<AttrInfo*> TableMeta::get_attributes()
{
    return attributes;
}

vector<IndexInfo*> TableMeta::get_indexes()
{
    return indexes;
}

const char* FieldMeta::name() const
{
    return name_.c_str();
}

AttrType FieldMeta::type() const
{
    return AttrType();
}
