#pragma once
#include "mylib.h"
#define INTERCODES//通过这个宏定义来控制是否删除符号表中的元素
// #include<malloc.h>
// #include<stdio.h>
// #include<stdarg.h>
// #include<stdlib.h>
// #include<string.h>
#include<stdbool.h>
#include<assert.h>
#define  TABLE_SIZE 0x3ff
extern int yylineno;
//代码实现上，你可以使用如下定义的Type结构来表示C--语言中的类型：
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
//

enum semErr{none, varUk,funUk,varReDef,funReDef,asNotMatch,asRval,opNotMatch,
    returnNotMatch,argsNotMatch,arrVisErr,funCallErr,arrVisFlt,struVisErr,struUkArea,
    struAreaReDef,struReDef,struUk, funcNotDef, funDefCom};
struct Type_{
//如果类型是函数，不仅要标志其返回值类型，还要标记它的参数个数、返回值类型
    enum { BASIC, ARRAY, STRUCTURE, FUNCTION} kind;
    enum {rval, lval} val_kind;
    union{
        // 基本类型
        int basic;
        // 数组类型信息包括元素类型与数组大小构成
        struct { Type elem; int size; } array;
        // 结构体类型信息是一个链表
        FieldList structure;
        // 函数
        //函数则记录返回值，其类型和上述三种类型相同
        //有可能有上述类型之外的返回值吗，比如指针？
        struct {
            Type rType;
            int nArgs;//参数个数
            FieldList args;//参数：名字+类型+下一个参数的指针。参数个数为0则空
        }func;
        //caution:函数的返回值类型不会为函数，
        // Type return_type;
    } u;
};
typedef struct Type_ Type_;
struct FieldList_//用作Type里的结构体类型
{
    char* name; // 域的名字
    Type type; // 域的类型

    FieldList tail; // 下一个域
};
typedef struct FieldList_ FieldList_;
//想了想，还是不要把Var和fieldlist放在一起

struct Var_//符号表的元素
{
    char* name; // 变量的名字
    Type type; // 变量的类型
    bool isVar;//var or fun/struct def
    int lineno;

};
typedef struct Var_ Var_;
typedef struct Var_* Var;


typedef struct TableNode TableNode;
typedef struct TableNode* pTableNode;
//每个TableNode是将一串符号连起来的
struct TableNode{
    Var node;
    pTableNode next;
};



typedef struct StackNode StackNode;
typedef struct StackNode* pStackNode;
typedef struct StackNode* LinkStack;
//每一个stackNode中的node都是指向一个链表,而且是链表头
struct StackNode{
    pTableNode node;//
    pStackNode next;//栈里下一个元素
};

pTableNode funcList;
void printArra(const Var t);
extern int typeSize(Type );
void initFuncList();
void insertFunc(Var v);
bool removeFunc(char* name);
void printUNfunc();
void setNewScope();
Var findFunc(char* name);
void printStack();
LinkStack s;
void initStack();
void push( pStackNode t);
bool pop( pStackNode* t);
bool isEmpty();
pStackNode top();
bool findInScope(char* name);
unsigned int hash_pjw(char* name);
pTableNode* hashTable; 
void initHashTalbe();
bool hash_insert(const Var v);
bool newScope;
void printHashTable();

//有必要返回的变量里存有变量的名称吗，理论上来讲没用
Var hash_find(char* name);
//正常情况下，从某个作用域退出后，需要删除的都是在哈希表里桶的第一个位置
bool hash_del();
void del_ptn(pTableNode ptn);
void semError(int type, int lineno, char* info);
// extern pNode root; //没必要
void semAnaly(pNode root);
void Program(pNode root);
void ExtDefList(pNode root);
void ExtDef(pNode root);
void ExtDecList(pNode root, const Type type);
Type Specifier(pNode root);
Type StructSpecifier(pNode root);
char* OptTag(pNode root);
char* Tag(pNode root);
Var VarDec(pNode root,const Type type);
Var FunDec(pNode root, bool def);
void VarList(pNode root, const Var fun);
Var ParamDec(pNode root);
void CompSt(pNode root, const Type rType);
void StmtList(pNode root, const Type rType);
void Stmt(pNode root, const Type rType);
void DefList(pNode root, bool inStruct, const Var struName);
void Def(pNode root, bool inStruct, const Var struName);
void DecList(pNode root, const Type type, bool inStruct, const  Var struName);
void Dec(pNode root, const Type type, bool inStruct, const Var struName);
Type Exp(pNode root);
bool Args(pNode root, FieldList fl);


Type look4type(char* name);
bool iSameType(const Type a, const Type b);
void printStruct(const FieldList struc);