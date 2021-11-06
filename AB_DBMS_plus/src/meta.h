#pragma once

#ifndef _META_H_
#define _META_H_

#include <vector>
#include "rc.h"
#include "parse_defs.h"

// field_meta
class FieldMeta {
public:
	FieldMeta();
	~FieldMeta() = default;
	RC init(const char* name, AttrType attr_type, int attr_offset, int attr_len, bool visible);

public:
	const char* name() const;
	AttrType    type() const;
	int         offset() const;
	int         len() const;
	bool        visible() const;

public:
	void desc(std::ostream& os) const;


private:
	std::string  name_;
	AttrType     attr_type_;
	int          attr_offset_;
	int          attr_len_;
};

// index_meta
class IndexMeta {
public:
	IndexMeta() = default;

	RC init(const char* name, const FieldMeta& field);

public:
	const char* name() const;
	const char* field() const;

	void desc(std::ostream& os) const;


private:
	std::string       name_;
	std::string       field_;
};

// table_meta
class TableMeta {
public:
	TableMeta() = default;
	~TableMeta() = default;
	RC init(CreateTable* ct);

	const char* name() const;
	int record_size();

	vector<AttrInfo*> get_attributes();
	vector<IndexInfo*> get_indexes();
private:
	string   name_;
	int record_size_;
	vector<AttrInfo*> attributes;
	vector<IndexInfo*> indexes;

};



#endif
