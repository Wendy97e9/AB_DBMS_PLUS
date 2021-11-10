#pragma once

#ifndef _PARSE_DEFS_
#define _PARSE_DEFS_


#define MAX_NUM 20
#define MAX_REL_NAME 20
#define MAX_ATTR_NAME 20
#define MAX_ERROR_MESSAGE 20
#define MAX_DATA 50
#define INIT_DATAFILE_SIZE 10   //ÿ������10��Page��С

//���Խṹ��
typedef struct {
    char* relation_name;   // relation name (may be NULL) ����
    char* attribute_name;  // attribute name              ������
} RelAttr;

typedef enum {
    EQUAL_TO,     //"="     0
    LESS_EQUAL,   //"<="    1
    NOT_EQUAL,    //"<>"    2
    LESS_THAN,    //"<"     3
    GREAT_EQUAL,  //">="    4
    GREAT_THAN,   //">"     5
    NO_OP
} CompOp;

//����ֵ����
typedef enum { UNDEFINED, CHARS, INTS, FLOATS } AttrType;

//����ֵ
typedef struct _Value {
    AttrType type;  // type of value
    void* data;     // value
} Value;

typedef struct _Condition {
    int left_is_attr;    // TRUE if left-hand side is an attribute
                         // 1ʱ���������������������0ʱ��������ֵ
    Value left_value;    // left-hand side value if left_is_attr = FALSE
    RelAttr left_attr;   // left-hand side attribute
    CompOp comp;         // comparison operator
    int right_is_attr;   // TRUE if right-hand side is an attribute
                         // 1ʱ���������ұ�����������0ʱ��������ֵ
    RelAttr right_attr;  // right-hand side attribute if right_is_attr = TRUE �ұߵ�����
    Value right_value;   // right-hand side value if right_is_attr = FALSE
} Condition;

// struct of select
typedef struct {
    size_t    attr_num;               // Length of attrs in Select clause
    RelAttr   attributes[MAX_NUM];    // attrs in Select clause
    size_t    relation_num;           // Length of relations in Fro clause
    char* relations[MAX_NUM];     // relations in From clause
    size_t    condition_num;          // Length of conditions in Where clause
    Condition conditions[MAX_NUM];    // conditions in Where clause
} Selects;

// struct of insert
typedef struct {
    char* relation_name;    // Relation to insert into
    size_t value_num;       // Length of values
    Value values[MAX_NUM];  // values to insert
} Inserts;

// struct of delete
typedef struct {
    char* relation_name;            // Relation to delete from
    size_t condition_num;           // Length of conditions in Where clause
    Condition conditions[MAX_NUM];  // conditions in Where clause
} Deletes;

// struct of update
typedef struct {
    char* relation_name;            // Relation to update
    char* attribute_name;           // Attribute to update
    Value value;                    // update value
    size_t condition_num;           // Length of conditions in Where clause
    Condition conditions[MAX_NUM];  // conditions in Where clause
} Updates;

//typedef struct {
//    char* name;     // Attribute name
//    AttrType type;  // Type of attribute
//    size_t length;  // Length of attribute
//} AttrInfo;

typedef struct {
    char name[20];     // Attribute name
    AttrType type;  // Type of attribute
    size_t length;  // Length of attribute
} AttrInfo;

//ָ��Ӧ�ø��ˣ��洢���ԣ�����
typedef struct {
    char* name;     //������
    char* field;    //�ֶ�����Ҫ�������ȥtable_meta�ҳ��Ⱥ����ͣ�����tablemetaҪ�нӿ�֧��
} IndexInfo;

//// struct of craete_table
//typedef struct {
//    char* relation_name;           // Relation name
//    size_t attribute_count;        // Length of attribute
//    AttrInfo attributes[MAX_NUM];  // attributes
//} CreateTable;

// struct of create_table
typedef struct {
    char relation_name[20];           // Relation name
    size_t attribute_count;        // Length of attribute
    AttrInfo attributes[MAX_NUM];  // attributes
} CreateTable;

// struct of drop_table
typedef struct {
    char* relation_name;  // Relation name
} DropTable;

// struct of create_index
typedef struct {
    char index_name[20];      // Index name
    char relation_name[20];   // Relation name
    char attribute_name[20];  // Attribute name
} CreateIndex;

// struct of  drop_index
typedef struct {
    const char* index_name;  // Index name
} DropIndex;

typedef struct {
    const char* relation_name;
} DescTable;

typedef struct {
    const char* relation_name;
    const char* file_name;
} LoadData;

union Queries {
    Selects selection;
    Inserts insertion;
    Deletes deletion;
    Updates update;
    CreateTable create_table;
    DropTable drop_table;
    CreateIndex create_index;
    DropIndex drop_index;
    DescTable desc_table;
    LoadData load_data;
    char* errors;
};

// �޸�yacc��������ֱ���Ϊ�궨��
enum SqlCommandFlag {
    SCF_ERROR = 0,
    SCF_SELECT,
    SCF_INSERT,
    SCF_UPDATE,
    SCF_DELETE,
    SCF_CREATE_TABLE,
    SCF_DROP_TABLE,
    SCF_CREATE_INDEX,
    SCF_DROP_INDEX,
    SCF_SYNC,
    SCF_SHOW_TABLES,
    SCF_DESC_TABLE,
    SCF_BEGIN,
    SCF_COMMIT,
    SCF_ROLLBACK,
    SCF_LOAD_DATA,
    SCF_HELP,
    SCF_EXIT
};
// struct of flag and sql_struct
typedef struct Query {
    enum SqlCommandFlag flag;
    union Queries sstr;
} Query;

#endif