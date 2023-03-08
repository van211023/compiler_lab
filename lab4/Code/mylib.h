#pragma once
#include<malloc.h>
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<string.h>

enum tokeType{grammer_,int_,float_,symbol_,id_type};
enum errorType{A_,B_,oct_,hex_,comm_,char_,func_};
struct Node{
    int lineno, childNum, type;
    char* token, *val;
    struct Node** child;
};
typedef struct Node Node;
typedef struct Node* pNode;
pNode createNode(int lineno, int type, char* token, char* val, int childNum,  ...);
void printTree(pNode root,int);
void printNode(pNode node, int indent);
void printError(int lineno, int type, int error, char* msg);
int findError;
char* int2char(int n);
int error_line;