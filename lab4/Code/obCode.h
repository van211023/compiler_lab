#pragma once
#include"ir.h"
#define REGISTER_NUM 32
struct Register_{
    char* name;
    Operand op;
    enum {R_FREE, R_BUSY} state;
};
typedef struct Register_ Register_;
typedef struct Register_* Register;
//   InterCode code = (InterCode)malloc(sizeof(InterCode_));
typedef struct V_list_ V_list_;
typedef struct V_list_* V_list;
typedef struct Func_ Func_;
typedef struct Func_* Func;
typedef struct FuncList_ FuncList_;
typedef struct FuncList_* FuncList;

struct Func_{
	char* name;
    V_list var;
    int args;
    int size;
};
struct FuncList_{
	Func cur;
    FuncList next;
};
struct V_list_{
    int offset;//前四个参数在寄存器中，置-1：最先读到的一定是参数
	Operand cur;
    V_list next;
};

FuncList f_List;
void initFunc(const InterCodes ir);

Register reg;
void initReg();
V_list newVarList();
void initOthers(FILE* out);
int getReg(Operand op);
void mapping(FILE* out);
void obCode(FILE* out);
//从变量地址加载到寄存器，可能是参数（在寄存器或者栈里），也可能是栈中的局部变量
int loadOp(Operand op, FILE* out, int offset);
//存储该变量到其对应的地址，若时在寄存器中的参数，不处理；否则将新值存到其对应的地址
void storeOp(Operand op, FILE* out, int offset, int regIndx, bool wr);
int getOpOffset(FuncList fl, const Operand op);
void insertVar(FuncList fl, const Operand op);
void printVl(V_list vl);
void saveParamReg(FILE* out);
void releseParamREg(FILE* out);