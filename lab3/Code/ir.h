#include "mylib.h"
#include "sem.h"
// #include<malloc.h>
// #include<stdio.h>
// #include<stdarg.h>
// #include<stdlib.h>
// #include<string.h>
// #include<stdbool.h>
// #include<assert.h>
// #include
typedef struct Operand_* Operand;
// enum IR{IR_VARIABLE, IR_CONSTANT, IR_ADDRESS, 
//     IR_LABEL, IR_TEMP, IR_ASSIGN, IR_ADD, IR_SUB, IR_MUL, 
//     IR_IF_GOTO, OR_RELOP};
struct Operand_ {
    // enum { VARIABLE, CONSTANT, ADDRESS, LABEL, TEMP } kind;
    enum {OP_VARIABLE, OP_CONSTANT, OP_ADDRESS, OP_LABEL, OP_TEMP, 
    OP_RELOP, OP_FUNCTION} kind;
    union {
        int var_no;//变量的个数  ID?
        int value;
        char* name;
        //...
    } u;
    Type type;
    bool isAddr, isArra, isStruc;//嵌套数组怎么整啊
    //空间or时间? 空间换时间的话就是在operand里面标记当前数组元素的大小,或者直接把它的Type放进来
    //时间换空间即 每次都要遍历来寻找
    // union{
    //     Type a;
    //     FieldList fl;
    // }
};
typedef struct Operand_ Operand_;
struct InterCode_
{
    // enum { ASSIGN, ADD, SUB, MUL, … } kind;
    enum { IR_ASSIGN, IR_ADD, IR_SUB, IR_DIV ,IR_MUL, 
    IR_IF_GOTO, IR_GOTO, IR_DEC, IR_FUNCTION,  IR_LABEL, 
    IR_RETURN, IR_READ, IR_CALL, IR_ARG, IR_WRITE, 
    IR_PARAM, 
    IR_GET_ADDRESS, //x = &y
    IR_ASSIGN2CONTENT, //*x = y
    IR_GET_CONTENT,  //x = *y
    IR_ASSIGN_BT_ADDRESS //*x = *y
    } kind;
    union {
        struct {Operand op;} single;
        struct { Operand result, right; } op1;
        struct { Operand result, op1, op2; } op2;
        struct {Operand t1, op, t2, label_true;} if_goto;
        //...
    } u;
};
typedef struct InterCode_ InterCode_;
typedef struct InterCode_* InterCode;

typedef struct InterCodes_ InterCodes_;
typedef struct InterCodes_ * InterCodes;//每个中间代码的节点
struct InterCodes_ { 
    InterCode code; 
    struct InterCodes_ *pre, *next; 
};
typedef struct Arg_* Arg;
typedef struct Arg_ Arg_;
struct Arg_{
    Operand arg;
    Arg next;
    Arg pre;
};


struct Arg_list_{//每次发现一个参数时，就插入一个Arg_到tail
    Arg head, tail;//每次插入时，总是插入到tail
};
//怎么感觉头尾指针没必要
typedef struct Arg_list_ Arg_list_;
typedef struct Arg_list_* Arg_list; 
typedef struct Var_list_* Var_list;

struct Var_list_{
    Operand v;
    char* name;
    Var_list next;
};
typedef struct Var_list_ Var_list_;
int labelNum, tempNum, varNum;//label和t也是需要标号的
Var_list var_list;
InterCodes ir_list;

void printVar_list();
// InterCodes assign2addr(Operand left, Operand right);
InterCodes load(Operand* op);
char* getVarName(int var_no);
Operand ifID(pNode root);//是id则返回op，不是返回NULL
int typeSize(Type t);//也可以附带着求高维数组的低维大小
int getOffset(FieldList struc, char* name);//求结构体的
void addIR(InterCodes code);//把中间代码加入到中间代码链表里
void fprintIR(FILE* out, InterCodes list);
void fprintOP(Operand op, FILE* out);
void translate(pNode root, FILE* out);
InterCodes newIR(int kind, ...);
Operand newOp(int kind, ...);
void concatIR(int num, ...);
Operand findVar(char* name);
void trans_Program(pNode root);
void trans_ExtDefList(pNode root);
InterCodes trans_ExtDef(pNode root);
InterCodes trans_FunDec(pNode root);
InterCodes trans_VarList(pNode root);
InterCodes trans_ParamDec(pNode root);
Operand trans_VarDec(pNode root, bool isPara);

InterCodes trans_Compst(pNode root);
InterCodes trans_DefList(pNode root);
InterCodes trans_Def(pNode root);
InterCodes trans_DecList(pNode root);
InterCodes trans_Dec(pNode root);
InterCodes trans_StmtList(pNode root);
InterCodes trans_Exp(pNode root, Operand place);
InterCodes trans_Args(pNode root, Arg_list arg_list);

InterCodes trans_Stmt(pNode root);
InterCodes trans_Cond(pNode root, Operand label_true, Operand label_false);
FILE* f;
