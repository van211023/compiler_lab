
#include"ir.h"
// #define DEBUG_ARRA
// #define DEBUG_LINE
// #define DEBUG
// #define DEBUG_CODE
// #define DEBUG_PRINT
// #define DEBUG_IR
// #define DEBUG_STUCTURE
// #define DEBUG_PRINT_LIST
// InterCodes assign2addr(Operand left, Operand right){
//     //经过了语义分析，肯定不存在error
//     assert(left->type);
//     assert(right->type);
//     // if(left->)
// }
InterCodes load(Operand* op){
    // if(!(*op)->type)
    //     return NULL;
    if(!(*op)->isAddr)
        return NULL;
    Operand t = newOp(OP_TEMP);
    InterCodes code = newIR(IR_GET_CONTENT, t, *op);
    //要释放掉op原来的内容吗
    *op = t;
    return code;
}
void printVar_list(){
    printf("#####printVar_lsit begin####\n");
    Var_list vl = var_list;
    while(vl){
        printf("INFO: no: %d, name: %s\n", vl->v->u.var_no, vl->name);
        vl = vl->next;
    }
    printf("#####printVar_lsit end####\n");
}
Operand ifID(pNode root){
    assert(root);
    #ifdef DEBUG_STUCTURE
    printf("INFO: childNum: %d\n",root->childNum);
    #endif
    if(!root->childNum && !strcmp(root->token, "ID")){
        // printf("root->token: %s\n", root->token);
        Operand id = findVar(root->val); //函数并没有在这个里面
        // printf("root->val: %s, childNum: %d\n",root->val, root->childNum);   
        // assert(id);
        return id;  
    }
    return NULL;
}

char* getVarName(int var_no){
    Var_list vl = var_list;
    // printVar_list();
    // printf("INFO: varNum: %d, var_no: %d\n", varNum, var_no);
    for(int i = 0; i < varNum - var_no - 1 && vl; i++){
        vl = vl->next;
        // printf("INFO: name:%s#\n",vl->name);
    }
    if(!vl)
        return NULL;
    else {
        // printf("INFO: in getName:name: %s\n", vl->name);
        return vl->name;
    }
    
}
int typeSize(Type t){
    if(t->kind == BASIC){
        return 4;//或者定义为一个宏/常量
    }
    else if(t->kind == ARRAY){
        return t->u.array.size * typeSize(t->u.array.elem);
    }
    else if(t->kind == STRUCTURE){
        int base = 0;
        FieldList fl = t->u.structure;
        while(fl){
            base += typeSize(fl->type);
            fl = fl->tail;
        }
        #ifdef DEBUG
        printf("DEBUG: base: %d\n",base);
        #endif
        return base;
    }
}
int getOffset(FieldList struc, char* name){
    assert(struc);
    assert(name);
    //结构体嵌套怎么办 ？ 
    //递归？
    //dot后面的元素是数组，数组又取下标怎么办？
    //struct是一个链表，这个链表中找name的偏移量 遍历
    int offset = 0;
    if(!strcmp(struc->name, name)){
        return offset;
    }
    else {
        offset += typeSize(struc->type);
        return offset + getOffset(struc->tail, name);
        //递归虽然效率低，但是好写
    }
}
void addIR(InterCodes code){
    // #ifdef DEBUG
    #ifdef DEBUG_CODE
    printf("# enter addIR\nINFO: %0x\n",code);
    #endif
    assert(code);
    if(!ir_list)
        ir_list = code;
    else {
        InterCodes pre = ir_list, cur = ir_list->next;
        // printf(" INFO: %0x\n",cur);
        while(cur){
            pre = cur;
            cur = cur->next;
        }//cur为null
        assert(pre);
        pre->next = code;
        code->pre = pre;
    }
    #ifdef DEBUG_CODE
    printf("     leave addIR\n");
    #endif
}
void fprintIR(FILE* out, InterCodes list){
    #ifdef DEBUG_PRINT
    printf("#enter fprintIR\n");
    #endif
    InterCodes temp_ir = list;
    while(temp_ir){
        InterCode t = temp_ir->code;
        #ifdef DEBUG_PRINT
        printf("IR kind: %d\n", t->kind);
        #endif
        switch(t->kind){
            case IR_ASSIGN:
                assert(t->u.op1.result);
                assert(t->u.op1.right);
                fprintOP(t->u.op1.result, out);
                fprintf(out, " := ");            
                fprintOP(t->u.op1.right, out);
                break;
            case IR_GET_ADDRESS:    
                fprintOP(t->u.op1.result, out);
                fprintf(out, " := &");
                fprintOP(t->u.op1.right, out);
                break;
            case IR_ASSIGN2CONTENT:
                fprintf(out, "*");
                fprintOP(t->u.op1.result, out);
                fprintf(out, " := ");
                fprintOP(t->u.op1.right, out);
                break;
            case IR_GET_CONTENT: IR_ASSIGN_BT_ADDRESS:            
                fprintOP(t->u.op1.result, out);
                fprintf(out, " := ");
                fprintf(out, "*");
                fprintOP(t->u.op1.right, out);
                break;
            case IR_ASSIGN_BT_ADDRESS:  
                fprintf(out, "*");              
                fprintOP(t->u.op1.result, out);
                fprintf(out, " := ");
                fprintf(out, "*");
                fprintOP(t->u.op1.right, out);
                break;
            case IR_ADD:
            case IR_SUB:
            case IR_MUL:
            case IR_DIV:
                fprintOP(t->u.op2.result, out);
                fprintf(out, " := ");
                if(!t->u.op2.result->isAddr && t->u.op2.op1->isAddr && !t->u.op2.op2->isAddr)
                    fprintf(out, "*");
                fprintOP(t->u.op2.op1, out);
                if(t->kind == IR_ADD)
                    fprintf(out, " + ");
                else if (t->kind == IR_SUB)
                    fprintf(out, " - ");       
                else if(t->kind == IR_MUL)
                    fprintf(out, " * ");
                else
                    fprintf(out, " / ");
                if(!t->u.op2.result->isAddr &&  !t->u.op2.op1->isAddr && t->u.op2.op2->isAddr)
                    fprintf(out, "*");
                fprintOP(t->u.op2.op2, out);
                break;
            case IR_GOTO:
                fprintf(out, "GOTO ");
                fprintOP(t->u.single.op, out);
                break;
            case IR_IF_GOTO:
                fprintf(out, "IF ");
                fprintOP(t->u.if_goto.t1, out);
                fprintf(out, " ");
                fprintOP(t->u.if_goto.op, out);
                fprintf(out, " ");
                fprintOP(t->u.if_goto.t2, out);
                fprintf(out, " GOTO ");
                fprintOP(t->u.if_goto.label_true, out);
                break;
            case IR_LABEL:
                fprintf(out, "LABEL ");
                fprintOP(t->u.single.op, out);
                fprintf(out, " :");
                break;
            case IR_RETURN:
                fprintf(out, "RETURN ");
                fprintOP(t->u.single.op, out);
                break;
            case IR_READ:
                fprintf(out, "READ ");
                fprintOP(t->u.single.op, out);
                break;
            case IR_WRITE:
                fprintf(out, "WRITE ");
                fprintOP(t->u.single.op, out);
                break;
            case IR_CALL:
                fprintOP(t->u.op1.result, out);
                fprintf(out, " := CALL ");
                fprintOP(t->u.op1.right, out);
                break;
            case IR_ARG://可能有多个参数？
                fprintf(out, "ARG ");
                // if(t->u.single.op->kind == OP_VARIABLE){
                //     char* name = getVarName(t->u.single.op->u.var_no);
                //     Var v = hash_find(name);
                //     if(v->type->kind==ARRAY || v->type->kind==STRUCTURE)
                //     //二维数组  参数是一维数组的话，就不能这么搞了

                //         fprintf(out, "&");
                // }

                // if(t->u.single.op->isAddr)//数组或者结构体
                    
                fprintOP(t->u.single.op, out);
                break;
            case IR_PARAM:
                fprintf(out, "PARAM ");
                fprintOP(t->u.single.op, out);
                break;
            case IR_FUNCTION:
                fprintf(out, "FUNCTION ");
                fprintOP(t->u.single.op, out);
                fprintf(out, " :");
                break;
            case IR_DEC:
                // printf("&&&&&&\n");
                fprintf(out, "DEC ");
                fprintOP(t->u.op1.result, out);
                fprintf(out, " %d",t->u.op1.right->u.value);
                // fprintOP(t->u.op1.right, out);
            default: break;
        }
        fprintf(out, "\n");
        temp_ir = temp_ir->next;
    }

    #ifdef DEBUG_PRINT
    printf("      leave fprintIR\n");
    #endif
}
void fprintOP(Operand op, FILE* out){
    //打印的时候有一个想法：通过标号，把var tmp label等变量的名称赋成这些
    //这样打印格式一样，降低代码的冗余
    assert(op);
    #ifdef DEBUG_PRINT
    printf("#enter fprintOP, kind: %d\n",op->kind);
    #endif

    switch(op->kind){
        case OP_VARIABLE:
            // if(op->isAddr)
            //     fprintf(out, "*v%d",op->u.var_no);
            // else 
                fprintf(out, "v%d",op->u.var_no);
            break;
        case OP_CONSTANT:
            fprintf(out, "#%d", op->u.value);
            break;
        case OP_LABEL:
            fprintf(out, "label%d",op->u.var_no);
            break;
        case OP_TEMP:
            // if(op->isAddr)
            //     fprintf(out, "*t%d",op->u.var_no);
            // else 
                fprintf(out, "t%d",op->u.var_no);
            break;
        case OP_RELOP:
        case OP_FUNCTION:
            fprintf(out, "%s",op->u.name);
            break;
        default: break;       
    }

    #ifdef DEBUG_PRINT
    printf("      leave fprintOP\n");
    #endif
}
// InterCodes newIR(InterCode code){
//     InterCodes ir = 
// }
InterCodes newIR(int kind, ...){
    #ifdef DEBUG
    printf("#enter newIR, kind:%d\n", kind);
    #endif

    InterCode code = (InterCode)malloc(sizeof(InterCode_));
    code->kind = kind;
    InterCodes ir = (InterCodes)malloc(sizeof(InterCodes_));
    ir->pre = ir->next = NULL;
    ir->code = code;

    va_list valist;
    va_start(valist, kind);
    switch(kind){
        case IR_WRITE:
        case IR_ARG:
        case IR_RETURN:
        case IR_GOTO:
        case IR_LABEL:
        case IR_FUNCTION:
        case IR_READ:
            code->u.single.op = va_arg(valist, Operand);
            break;
        case IR_ASSIGN:
        case IR_CALL:
        case IR_DEC:
        case IR_GET_ADDRESS: 
        case IR_GET_CONTENT:
        case IR_ASSIGN2CONTENT:
        case IR_ASSIGN_BT_ADDRESS:
            code->u.op1.result = va_arg(valist, Operand);
            code->u.op1.right = va_arg(valist, Operand);
            break;
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV:
            code->u.op2.result = va_arg(valist, Operand);
            code->u.op2.op1 = va_arg(valist, Operand);
            code->u.op2.op2 = va_arg(valist, Operand);
            break;
        case IR_IF_GOTO:
            code->u.if_goto.t1 = va_arg(valist, Operand);
            code->u.if_goto.op = va_arg(valist, Operand);
            code->u.if_goto.t2 = va_arg(valist, Operand);
            code->u.if_goto.label_true = va_arg(valist, Operand);
            break;
        case IR_PARAM:
            code->u.single.op = va_arg(valist, Operand);
            break;
        default: break;
        
    }

    #ifdef DEBUG
    printf("     leave newIR\n");
    #endif

    ir->code = code;
    // printf("INFO:create ircode %0x\n",ir);
    return ir;
}
Operand newOp(int kind, ...){
    Operand op = (Operand)malloc(sizeof(Operand_));
    op->isAddr = op->isArra = op->isStruc = false;
    op->type = NULL;
    va_list valist;
    va_start(valist, kind);
    op->kind = kind;
    switch(kind){
        case OP_CONSTANT: 
            op->u.value = va_arg(valist, int); break;
        // case OP_VARIABLE: 
        case OP_RELOP:
        case OP_FUNCTION:{
            char* name = va_arg(valist, char*);
            assert(name);
            op->u.name  = (char*)malloc(sizeof(name)+1);
            strcpy(op->u.name, name);
            break;
        }
        case OP_LABEL:
            op->u.var_no = labelNum;
            labelNum++;
            break;
        case OP_TEMP:
            op->u.var_no = tempNum;
            tempNum++;
            break;
        case OP_VARIABLE:
            op->u.var_no = varNum;
            varNum++;
            break;

        default: assert(0); break;
    }
    va_end(valist);
    return op;
}
// InterCodes newIn
void concatIR(int num, ...){
    #ifdef DEBUG
    printf("#enter concatIR, num:%d\n", num);
    #endif
    InterCodes pre = NULL;
    InterCodes cur = NULL;

    assert(num);

    va_list valist;
    va_start(valist, num);
    
    pre = va_arg(valist, InterCodes);
    num--;
    while(!pre && num > 0){
        num--;
        pre = va_arg(valist, InterCodes);
        #ifdef DEBUG_IR
        printf("HELP\n");
        // printf("INFO: pre:%0x, kind:%d\n", pre,pre->code->kind);
        #endif

    }
    assert(pre);
    // printf("line 264: INFO: %d\n", pre->code->kind);
    // printf("INFO: code1:%0x\n", pre);
    InterCodes pre_nxt = NULL;
    for(int i = 0; i < num ; i++){
        cur = va_arg(valist, InterCodes);

        if(cur == NULL)//当读取所有参数后，为null?
            continue;
        #ifdef DEBUG_IR
        printf("INFO: cur:%0x\n", cur);
        #endif
        assert(pre);
        assert(cur);
        #ifdef DEBUG
        printf("pre:%0x\n",pre);
        #endif

        //把cur加到pre的next为空的后面
        pre_nxt = pre->next;
        while(pre_nxt){
            pre = pre->next;
            pre_nxt = pre->next;
        }

        pre->next  = cur;
        // cur->pre = pre;
        // printf("line 275\n");
        // printf("x\n");
        pre = cur;
        // printf("y\n");/
        cur = NULL;
        // printf("INFO: pre:%0x, cur:%0x\n", pre, cur);
        // printf("line 277: end one loop\n");
    }
    va_end(valist);
    #ifdef DEBUG
    printf("     leave concatIR\n");
    #endif
}

// void concatIR(InterCodes code1, ...){
//     #ifdef DEBUG
//     printf("#enter concatIR\n");
//     #endif
//     InterCodes pre = NULL;
//     InterCodes cur = NULL;
//     va_list valist;
//     va_start(valist, code1);
    
//     pre = code1;
//     assert(code1);
//     assert(pre);
//     // printf("INFO:%xlld\n",pre);
//     // printf("INFO:END\n");
//     // assert(0);
//     assert(pre->code);
    
//     // assert(0);
//     // printf("line 264: INFO: %d\n", pre->code->kind);
//     printf("INFO: code1:%0x\n", code1);
//     while(1){
//         printf("line 266: INFO: pre kind:%d\n", pre->code->kind);
//         cur = va_arg(valist, InterCodes);
//         printf("INFO: cur:%0x\n", cur);
    
//         if(cur == NULL)//当读取所有参数后，为null?
//             break;
//         assert(pre);
//         assert(cur);
//         pre->next  = cur;
//         // cur->pre = pre;
//         // printf("line 275\n");
//         pre = cur;
//         cur = NULL;
//         // printf("INFO: pre:%0x, cur:%0x\n", pre, cur);
//         // printf("line 277: end one loop\n");
//     }
//     va_end(valist);
//     #ifdef DEBUG
//     printf("     leave concatIR\n");
//     #endif
// }
Operand findVar(char* name){
    Var_list vl = var_list;
    while(vl){
        if(!strcmp(vl->name, name))
            return vl->v;
        vl = vl->next;
    }
    return NULL;
}
void translate(pNode root, FILE* out){
    labelNum = tempNum = varNum = 0;
    var_list = NULL;
    ir_list = NULL;
    f  = fopen("output.txt", "w");
    // printHashTable();
    trans_Program(root);
    fprintIR(out, ir_list);
    #ifdef DEBUG_PRINT_LIST
    printVar_list();
    #endif
    // printf("\nname:%s\n",var_list->name);
    fclose(f);
}
// Program → ExtDefList

// ExtDefList → ExtDef ExtDefList
// | null
void trans_Program(pNode root){
    #ifdef DEBUG
    printf("#enter Program\n");
    #endif
    if(!root)
        return;
    trans_ExtDefList(root->child[0]);
    #ifdef DEBUG
    printf("     leave Program\n");
    #endif
}
void trans_ExtDefList(pNode root){
    #ifdef DEBUG
    printf("#enter ExtDefList\n");
    #endif
// ExtDefList → ExtDef ExtDefList
// | null
    if(!root)   
        return;
    assert(root->child[0]);
    InterCodes code = trans_ExtDef(root->child[0]);
    if(code)
        addIR(code);
    // assert(root->child[1]);
    trans_ExtDefList(root->child[1]);
    // assert(0);
    #ifdef DEBUG
    printf("     leave ExtDefList\n");
    #endif
}
// ExtDef → Specifier ExtDecList SEMI  没有
// | Specifier SEMI  不用生成中间代码
// | Specifier FunDec CompSt
// | Specifier FunDec SEMI 没有函数的声明，只会出现定义
InterCodes trans_ExtDef(pNode root){
    #ifdef DEBUG
    printf("#enter ExtDef\n");
    #endif
    //碰到函数的时候，把函数生成中间代码
    if(!strcmp(root->child[1]->token, "FunDec")){
        InterCodes code1 = trans_FunDec(root->child[1]);
        InterCodes code2 = trans_Compst(root->child[2]);//为了获得参数的
        // assert(code1); assert(code2);

        #ifdef DEBUG_CODE
        printf("INFO: in ExtDef code1:%0x,code2:%0x\n",code1, code2);
        #endif
        concatIR(2, code1, code2);

        #ifdef DEBUG
        printf("     leave ExtDef\n");
        #endif
        assert(code1);
        return code1;;
    }
    return NULL;
    // else if(!strcmp(root->child[1]->token, "ExtDecList")){
    //     #ifdef DEBUG
    //     printf("     leave ExtDef\n");
    //     #endif
    //     return trans_ExtDecList(root->child[1]);
    // }

}
InterCodes trans_FunDec(pNode root){
    #ifdef DEBUG
    printf("#enter FunDec\n");
    #endif
    // FunDec → ID LP VarList RP
    // | ID LP RP
    //为什么不直接查符号表？
    //此时已经有了符号表，并且该函数对应的参数也已经在符号表里面
    //哦不，符号表在进入/退出嵌套的作用域时，会逐层退出并删除表中元素，但是最后一层，并没有删除
    //but应该在语义分析后，删除符号表中的内容，因此应该逐层遍历，得到VarList
    assert(root->child[0]->val);
    Operand fun = newOp(OP_FUNCTION, root->child[0]->val);
    InterCodes code1 = newIR(IR_FUNCTION, fun);
    if(root->childNum == 4){//函数有参数
        //get ir of VarList
        InterCodes code2 = trans_VarList(root->child[2]);
        #ifdef DEBUG_CODE
        printf("INFO: in FunDec code1:%0x,code2:%0x\n",code1, code2);
        #endif
        concatIR(2, code1, code2);
    }
    #ifdef DEBUG
    printf("     leave FunDec\n");
    #endif

     assert(code1);  return code1;;

}
InterCodes trans_VarList(pNode root){
    #ifdef DEBUG
    printf("#enter VarList\n");
    #endif
    // VarList → ParamDec COMMA VarList
    // | ParamDec
    //要正序地将各个参数加入IR， 因此我们先求ParamDc
    InterCodes code1 = trans_ParamDec(root->child[0]);
    if(root->childNum == 3){
        InterCodes code2 = trans_VarList(root->child[2]);
        #ifdef DEBUG_CODE
        printf("INFO: in VarList code1:%0x,code2:%0x\n",code1, code2);
        #endif
        concatIR(2, code1, code2);
    }
    #ifdef DEBUG
    printf("     leave VarList\n");
    #endif

     assert(code1);  return code1;;

}
InterCodes trans_ParamDec(pNode root){
    #ifdef DEBUG
    printf("#enter ParamDec\n");
    #endif
    // ParamDec → Specifier VarDec   
    //选做：当变量的类型是结构体或者数组时，引用传值，中间代码的生成是不一样的
    //需要获得偏移量 并取该地址的元素

    //不需要知道参数的类型，只要按顺序给个名字就好了
    Operand para = trans_VarDec(root->child[1], true);
    // if(para->)
    InterCodes para_ir = newIR(IR_PARAM, para);
    #ifdef DEBUG
    printf("     leave ParamDec\n");
    #endif
    assert(para_ir);
    return para_ir;    

}
Operand trans_VarDec(pNode root, bool isPara){
    #ifdef DEBUG
    printf("#enter VarDec\n");
    #endif
    // VarDec → ID
    // | VarDec LB INT RB  数组的变量定义不做额外的处理
    if(root->childNum == 1){
        //函数参数：先查找var_list 存在则报错
        //不存在则建立一个，创建op并插入链表
        assert(root->child[0]);
        
        // printf("$$root:%s,root->child:%s\n", root->token,root->child[0]->token);
        assert(root->child[0]->val);
        char* name = root->child[0]->val;
        Operand id = findVar(name);
        assert(!id);//找到就报错，变量声明不能重复

        // if(!id && ifPara)//没有找到参数
            // assert(0);
        if(!id){//没有找到
            //怎么获取它的编号
            id = newOp(OP_VARIABLE);  
            Var v = hash_find(name);//查找变量，看他的类型，数组或者结构体则为地址
            id->type = v->type;//得到type  这样就不用查找了
            #ifdef DEBUG
            printf("Name: %s\n", v->name);
            #endif
            assert(v->type);
            assert(id->type);
            if((v->type->kind == ARRAY || v->type->kind == STRUCTURE)&& isPara)
                id->isAddr = true;
            else if(!isPara){
                if(v->type->kind == ARRAY)
                    id->isArra = true;
                else if(v->type->kind == STRUCTURE)
                    id->isStruc = true;
            }
            Var_list var = (Var_list)malloc(sizeof(Var_list_));
            var->v = id;
            var->name = (char*)malloc(sizeof(name)+1);
            strcpy(var->name, name);
            var->next = var_list;
            var_list = var;//插入链表
        }
        // InterCodes para = newIR(IR_PARAM, id);
        // return para;
        #ifdef DEBUG
        printf("     leave VarDec\n");
        #endif

        return id;
    }
    else {
        #ifdef DEBUG
        printf("     leave VarDec\n");
        #endif
        return trans_VarDec(root->child[0], isPara);
    }

}
// CompSt → LC DefList StmtList RC
InterCodes trans_Compst(pNode root){//stmtList不可能也NULL
    #ifdef DEBUG
    printf("#enter Compst\n");
    #endif
    InterCodes code1 = trans_DefList(root->child[1]);
    InterCodes code2 = trans_StmtList(root->child[2]);
    #ifdef DEBUG_CODE
    printf("INFO: in Compst code1:%0x,code2:%0x\n",code1, code2);
    #endif
    if(code1){
        concatIR(2, code1, code2);
    }
    else 
        return code2;//如果code2为NULL，也可能返回null
    #ifdef DEBUG
    printf("     leave Compst\n");
    #endif
    assert(code1);
    return code1;
}
InterCodes trans_DefList(pNode root){//有可能返回NULL
    #ifdef DEBUG
    printf("#enter DefList\n");
    #endif
    // DefList → Def DefList
    // | null
    if(!root)
        return NULL;
    InterCodes code1 = trans_Def(root->child[0]);
    InterCodes code2 = trans_DefList(root->child[1]);
    // assert(code1);
    if(code2){
        #ifdef DEBUG_CODE
        printf("INFO: in DefList code1:%0x,code2:%0x\n",code1, code2);
        #endif
        if(code1)
            concatIR(2, code1, code2);
        else 
            return code2;
    }
    #ifdef DEBUG
    printf("     leave DefList\n");
    #endif
    return code1;;
}
InterCodes trans_Def(pNode root){    //有可能返回NULL
    #ifdef DEBUG
    printf("#enter Def\n");
    #endif
    // Def → Specifier DecList SEMI
    InterCodes code = trans_DecList(root->child[1]);

    //这里的类型应该也不用管？
    #ifdef DEBUG
    printf("     leave Def\n");
    #endif
    return code;
}
InterCodes trans_DecList(pNode root){//有可能返回NULL
    #ifdef DEBUG
    printf("#enter DecList\n");
    #endif
    // DecList → Dec
    // | Dec COMMA DecList
        // printf("$$root:%s,root->child:%s\n", root->token,root->child[0]->token);
    InterCodes code1 = trans_Dec(root->child[0]);
    // fprintIR(f, code1);
    if(root->childNum == 3){
        InterCodes code2 = trans_DecList(root->child[2]);
        #ifdef DEBUG_CODE
        printf("INFO: in DecList: code1:%0x, code2:%0x\n", code1, code2);
        #endif
        if(code1)
            concatIR(2, code1, code2);
        else if(code2)
            return code2;
        else{
            return NULL;
        }
    }
    #ifdef DEBUG
    printf("     leave DecList\n");
    #endif
    #ifdef DEBUG_CODE
    printf("INFO: in DecList: code1:%0x\n", code1);
    #endif
    return code1;;
}
InterCodes trans_Dec(pNode root){//有可能返回NULL
    #ifdef DEBUG
    printf("#enter Dec\n");
    #endif
    // Dec → VarDec
    // | VarDec ASSIGNOP Exp  变量定义时的赋值  
        // printf("$$root:%s,root->child:%s\n", root->token,root->child[0]->token);
    Operand var = trans_VarDec(root->child[0], false);
    assert(var);
    assert(var->kind==OP_VARIABLE);//stmt中，也会有TEMP的type是数组/结构体
    // assert(var->u.name);
    #ifdef DEBUG
    printf("INFO: in dec var_no: %d\n", var->u.var_no);
    #endif
    // char* name = getVarName(var->u.var_no);
    // assert(name);
    // Var v = hash_find(name);//在哈希表中查找
    // printf("INFO: %s#\n", name);
    InterCodes code=NULL;
    //如果是定义了一个数组或者结构体类型变量，生成DEC的中间代码
    if((var->type->kind == ARRAY || var->type->kind == STRUCTURE)){
        //按place的话，应该是把它当做参数传入VarDec
        int x = typeSize(var->type);
        Operand sz = newOp(OP_CONSTANT, x);
        #ifdef DEBUG
        printf("kind: %d\n",var->kind);
        #endif
        code = newIR(IR_DEC, var, sz);
        // printf("@@@@@@%0x@@@@\n",code);
        #ifdef DEBUG
        printf("INFO: DEC\n");
        #endif
        assert(code);
        // {
        //     if(code->code->kind==IR_DEC)
        //         printf("##DEC##\n");
        // }
        // fprintIR(f, code);
    }
    if(root->childNum == 3){//VarDec ASSIGNOP Exp
        Operand t = newOp(OP_TEMP);
        InterCodes code1 = trans_Exp(root->child[2], t);//Exp
        //理论上来讲，t应该在trans_Exp中被赋予值
        assert(code1);
        //有数组的赋值怎么办  理论上来讲，不会有数组在定义的时候赋值

        InterCodes code2 = NULL;
        if(t-> isArra){//结构体定义时不会赋值
            assert(0);
        }
        InterCodes tmp = load(&t);
        if(tmp){
            concatIR(2, code1, tmp);
        }
        code2 = newIR(IR_ASSIGN, var, t);
        concatIR(2, code1, code2);
        if(code)//前面有Dec
            concatIR(2, code, code1);
        else //code为空
            code = code1;
        #ifdef DEBUG
        printf("     leave Dec\n");
        #endif
    }
    // else{
    //     //没有赋值，也不是特色类型，不生成中间代码
    //     return code
    //     assert(0);
    // }
    // fprintIR(f, code);   
    return code;//may null

}
InterCodes trans_StmtList(pNode root){
    #ifdef DEBUG
    printf("#enter StmtList\n");
    #endif
    // StmtList → Stmt StmtList
    // | null
    if(!root)   
        return NULL;
    InterCodes code1 = trans_Stmt(root->child[0]);
    InterCodes code2 = trans_StmtList(root->child[1]);
    assert(code1);
    // fprintIR(f, code1);
    // fprintIR(f, code2);
    #ifdef DEBUG_CODE
    printf("INFO: code1: %0x, code2: %0x\n", code1, code2);
    #endif
    // printf("code1->kind: %d, code->result->kind: %d\n",code1->code->kind, code1->code->u.op1.right->kind);
    // fclose(f);
    // assert(0);
    concatIR(2, code1, code2);
    #ifdef DEBUG
    printf("     leave StmtList\n");
    #endif
     assert(code1);  return code1;;

}

InterCodes trans_Exp(pNode root, Operand place){
    #ifdef DEBUG
    printf("#enter Exp\n");
    #endif
    if(!root)
        return NULL;
    if(root->childNum == 1){
        // | ID
        // | INT
        // assert(root->child[0]->val);
        if(!strcmp(root->child[0]->token,"INT")){//emm这个应该是可以被优化掉？不需要一个临时变量来存储常量
            // #ifdef DEBUG
            // printf(" Exp->INT\n");
            // #endif            
            int val = atoi(root->child[0]->val);
            #ifdef DEBUG
            printf("Val: %d\n",val);
            #endif
            // Operand op = (Operand)malloc(sizeof(Operand_));
            // op->kind = OP_CONSTANT;
            // op->u.value = val
            Operand op = newOp(OP_CONSTANT, val);
            assert(place);
            place->type = NULL;
            place->isAddr = place->isArra = place->isStruc = false;
            #ifdef DEBUG
            printf(" leave Exp->INT\n");
            #endif    
            return newIR(IR_ASSIGN, place, op);
            //赋值给place 
            //create intercode
            //return
        }
        else if(!strcmp(root->child[0]->token,"ID")){//ID
        #ifdef DEBUG
        printf(" Exp->ID\n");
        #endif            
            // Var v = hash_find(root->child[0]->val);
            Operand v = findVar(root->child[0]->val);

            #ifdef DEBUG_ARRA
            printf("INFO: ID name: %s, type: %d, typeSize: %d\n",root->child[0]->val, v->type->kind, typeSize(v->type));
            Var tmp_v = hash_find(root->child[0]->val);
            printArra(tmp_v);
            #endif
            assert(v);
            // Operand op = (Operand_)malloc(sizeof(Operand_));
            // op->kind = IR_VARIABLE;
            // op->u.name = (char*)malloc(sizeof(v->name+1));
            // Operand op = newOp(OP_VARIABLE);
            // strcpy(op->u.name, v->name);
            assert(place);
            // if()
            place->isAddr = v->isAddr;//没毛病 如果是地址，那么因为赋值，所以会变成地址；如果不是，则要取地址
            place->isArra = v->isArra;
            place->isStruc = v->isStruc;

            // Var var = hash_find(root->child[0]->val);
            // v->type = var->type;
            InterCodes code = NULL;
            if(v->type->kind == BASIC){
                // place->type;
                code = newIR(IR_ASSIGN, place, v);
            }
            else if(v->type->kind == ARRAY || v->type->kind == STRUCTURE){
                //数组和结构体的处理是一样的
                place->isAddr = true;
                // assert(place->isAddr);//一般这个时候place肯定是
                if(v->isAddr){//作为函数参数的数组变量
                    code =  newIR(IR_ASSIGN, place, v);
                }
                else {
                    // place->isAddr = true;
                    code = newIR(IR_GET_ADDRESS, place, v);
                }
                if(v->type->kind == ARRAY)//a[2][3] -> a[1]
                    place->type = v->type;
                else if(v->type->kind == STRUCTURE)
                    place->type = v->type;
            }
            #ifdef DEBUG
            printf("     leave Exp\n");
            #endif
            assert(code);  
            return code;
            // else of(v->type->kind == STRUCTURE)
            // else if((var->type->kind == ARRAY || var->type->kind == STRUCTURE)&&!v->isAddr){
            //     // printf("aaaa\n");
            //     place->isAddr = true;
            //     code = newIR(IR_GET_ADDRESS, place, v); 
            //     }
            //     else{
            //         if(place->isAddr && !v->isAddr)
            //             code =  newIR(IR_ASSIGN2CONTENT, place, v);
            //         else 
            //             code =  newIR(IR_ASSIGN, place, v);
            //     }
                // code =  newIR(IR_ASSIGN, place, v);
            // printf("\nassign:v%d\n",v->u.var_no);

        }
    }
    if(!strcmp(root->child[0]->token,"MINUS")){
        // | MINUS Exp
        #ifdef DEBUG
        printf(" Exp->MINUS Exp\n");
        #endif            
        Operand t1 = newOp(OP_TEMP);
        InterCodes code1 = trans_Exp(root->child[1], t1);
        Operand zero = newOp(OP_CONSTANT,0);//下一个参数是常量的大小
        assert(place);
        InterCodes code2 = newIR(IR_SUB, place, zero, t1);
        concatIR(2, code1, code2);

        #ifdef DEBUG
        printf("     leave Exp\n");
        #endif
         assert(code1);  return code1;;
    }
    // if(root->childNum == 3){
        if(!strcmp(root->child[1]->token,"ASSIGNOP")){
        #ifdef DEBUG
        printf(" Exp->Exp ASSIGNOP Exp\n");
        #endif            


            //Exp ASSIGNOP Exp
            // | Exp MINUS Exp
            // | Exp STAR Exp
            // | Exp DIV Exp
            // Var v = hash_find(root->child[0]->val);//
            // assert(v);
            // Operand t1 = (Operand_)malloc(sizeof(Operand_));
            // Operand op = newOp(TEMP);
            // if(!strcmp(root->child[0]->child[0]->token, "ID") && root->child[0]->childNum == 1){
        
            //         #ifdef DEBUG
            //         printf(" first exp of assign is ID\n");
            //         #endif
            //     Operand v = findVar(root->child[0]->child[0]->val);
            //     // assert(0);
            //     assert(v);
            //     Operand t1 = newOp(OP_TEMP);
            //     InterCodes code1 = trans_Exp(root->child[2], t1);
            //     assert(code1);
            //     // fprintIR(f, code1);
            //     #ifdef DEBUG
            //     printf(" after trans Exp2 int exp assign\n");
            //     #endif

            //     InterCodes code2_t1 = NULL;
            //     if(v->isAddr && !t1->isAddr)
            //         code2_t1 = newIR(IR_ASSIGN2CONTENT, v, t1);
            //     else
            //         code2_t1 = newIR(IR_ASSIGN, v, t1);
            //     // assert(place);
            //     // fprintIR(f, code2_t1);
            //     InterCodes code2_t2 = NULL;
            //     if(place){
            //         if(place->isAddr && !v->isAddr)
            //             code2_t2 = newIR(IR_ASSIGN2CONTENT, place, v);
            //         else 
            //             code2_t2 = newIR(IR_ASSIGN, place, v);
            //         // fprintIR(f, code2_t2);
            //     }
            //     // assert(code1); assert(code2_t1); assert(code2_t2);
            //     // concatIR(3, code2_t1, code2_t2);
            //         #ifdef DEBUG
            //         printf(" concat three part of assign\n");
            //         #endif
            //     //  printf("INFO: code1:%0x, code2:%0x, code3:%0x\n", code1, code2_t1, code2_t2);

            //     concatIR(3, code1, code2_t1, code2_t2);
            //          #ifdef DEBUG
            //         printf(" exit exp assign\n");
            //         #endif
            //     assert(code1);  return code1;;
            // }
            // else {//数组或者结构体  Exp = Exp
                Operand t1 =  ifID(root->child[0]->child[0]);
                
                Operand t2 = newOp(OP_TEMP);
                //有可能会碰到
                InterCodes code1 = NULL;
                if(!t1){
                    t1 = newOp(OP_TEMP);
                    code1 = trans_Exp(root->child[0], t1);
                }
                
                InterCodes code2 = trans_Exp(root->child[2], t2);
                InterCodes code3 = NULL;
                //先不考虑一维以上数组的赋值（需要拷贝一段内存空间）
                //所以下面处理的都是只要拷贝一个值的
                if((t1->isAddr||t1->isArra||t1->isStruc) ){
                    if (t2->isAddr||t2->isArra||t2->isStruc)
                        // code3 = assign2addr(t1, t2);
                        code3 = newIR(IR_ASSIGN_BT_ADDRESS, t1, t2);
                    else //值赋给地址指向的内存
                        code3 = newIR(IR_ASSIGN2CONTENT, t1, t2);
                }
                else{//不是地址或者结构体、数组
                    code3 = load(&t2);//不管t2是什么，取它的内容
                    //如果这种指针不行的话，就要另想办法
                    InterCodes code4 = newIR(IR_ASSIGN, t1, t2);
                    if(code3)
                        concatIR(2, code3, code4);
                    else 
                        code3 = code4;
                }
                if(!code1){
                    concatIR(2, code2, code3);
                    code1 = code2;
                }
                else {
                    concatIR(3, code1, code2, code3);
                }
                assert(code2); assert(code3);
                // assert(0);
                
                if(place){
                    place->type = t1->type;
                    place->isAddr = t1->isAddr; place->isStruc=t1->isStruc; place->isArra=t1->isStruc;
                }
                #ifdef DEBUG
                printf("     leave Exp from array or structure\n");
                #endif
                 assert(code1);  return code1;;
            // }

        }
        else if(!strcmp(root->child[1]->token,"PLUS") || !strcmp(root->child[1]->token,"MINUS")
           || !strcmp(root->child[1]->token,"STAR") || !strcmp(root->child[1]->token,"DIV")){
        #ifdef DEBUG
        printf(" Exp->Exp PLUS Exp blabla\n");
        #endif  
            // | Exp PLUS Exp
            Operand t1 = newOp(OP_TEMP);
            Operand t2 = newOp(OP_TEMP);
            InterCodes code1 = trans_Exp(root->child[0], t1);
            InterCodes code2 = trans_Exp(root->child[2], t2);
            
            if(t1->isAddr && !place->isAddr){
                Operand t3 = newOp(OP_TEMP);
                InterCodes tmp1 = newIR(IR_GET_CONTENT, t3, t1);
                concatIR(2, code2, tmp1);
                t1 = t3;
            }
            if(t2->isAddr && !place->isAddr){
                Operand t4 = newOp(OP_TEMP);
                InterCodes tmp2 = newIR(IR_GET_CONTENT, t4, t2);
                concatIR(2, code2, tmp2);
                t2 = t4;
            }

            InterCodes code3 = NULL;
            assert(place);
            if(!place)
                place = newOp(OP_TEMP);
            if(!strcmp(root->child[1]->token,"PLUS"))
                code3 = newIR(IR_ADD, place, t1, t2);
            else if(!strcmp(root->child[1]->token,"MINUS"))
                code3 = newIR(IR_SUB, place, t1, t2);
            else if(!strcmp(root->child[1]->token,"STAR"))
                code3 = newIR(IR_MUL, place, t1, t2);
            else 
                code3 = newIR(IR_DIV, place, t1, t2);
            concatIR(3, code1, code2, code3);
            // concatIR(3, code2, code3);
            #ifdef DEBUG
            printf("     leave Exp from plus sub mul div\n");
            #endif
             assert(code1);  return code1;;
        }
        else if(!strcmp(root->child[1]->token, "DOT")){
            #ifdef DEBUG
            printf(" Exp->Exp DOT ID\n");
             #endif  
            //结构体  | Exp DOT ID
            char* name = root->child[2]->val;
            //Exp有三种可能：
            //  1，ID
            //  2，Exp DOT ID 不会出现
            //  3，Exp(ID) [INT] 
            // place->isAddr = true;
                #ifdef DEBUG_LINE
                printf("INFO: line: %d\n", root->lineno);
                #endif 
            Operand t1 = newOp(OP_TEMP);//这种情况下，t1一定会被写为isAddr
            #ifdef DEBUG
            printf("child[0]->token: %s, chilNum: %d\n", root->child[0]->token, root->child[0]->childNum);
            #endif
            InterCodes code1 = trans_Exp(root->child[0], t1); 
            assert(code1);
            //t1会在里面被赋值   同时生成处理t1的IR
            assert(t1->isAddr);
            
            // assert(root->child[0]->childNum==1);//先不处理结构体数组
            //把可能需要取地址的内容递归调用，这里就不需要再取地址了
            // char* struc = getVarName(id->u.var_no);
            // char* struc = root->child[0]->child[0]->val;//非结构体数组
            // Var v = hash_find(struc);//v就是结构体
            // assert(v);
            #ifdef DEBUG
            printf("INFO: t1->kind: %d, type->kind: %d, t1->isAddr: %d, isStruc: %d\n",t1->kind, t1->type->kind ,t1->isAddr,t1->isStruc);
            #endif
            assert(t1->type);

            assert(t1->type->kind == STRUCTURE);

            #ifdef DEBUG_STUCTURE
            printStruct(t1->type->u.structure);
            #endif
            int tmp_offset = getOffset(t1->type->u.structure, name);       
            Var tmp_var = hash_find(name);//域名
            #ifdef DEBUG_STUCTURE
            printf("INFO: fl name: %s\n", name);
            #endif
            
            assert(tmp_var);
            int offset = typeSize(t1->type) - typeSize(tmp_var->type) - tmp_offset;
            //由于结构体是把各个域名插在头部，因此求偏移量要倒着求
            Operand cons = newOp(OP_CONSTANT, offset);

            place->isAddr = true;
            place->type = tmp_var->type;
            #ifdef DEBUG
            printf("tmp_var name: %s, type: %d\n", tmp_var->name, tmp_var->type->kind);
            printf("place->type: %d\n",place->type->kind);
            #endif
            InterCodes code2 = newIR(IR_ADD, place, t1, cons);

            if(code1)
                concatIR(2, code1, code2);
            else 
                code1 = code2;
            return code1;

            // Operand id = ifID(root->child[0]->child[0]);
            // if(id){//第一种情况
            //     #ifdef DEBUG
            //     printf("INFO: in exp.id var_no: %d\n", id->u.var_no);
            //     #endif
            //     char* struc = getVarName(id->u.var_no);
            //     Var v = hash_find(struc);
            //     assert(v);
            //     assert(v->type->kind == STRUCTURE);
            //     int offset = getOffset(v->type->u.structure, name);
            //     Operand cons = newOp(OP_CONSTANT, offset);
            //     InterCodes code = newIR(IR_ADD, place, id, cons);
            //     assert(code);
            //     return code;
            // }
            // else{//结构体名还是ID，不过要先求偏移量
            //     //那我去递归地翻译它，但是ID怎么传上来呢
            //     assert(0);
            // }

            // //经过语义分析，可以确定Exp中肯定有id
            // //但如果是结构体嵌套这种类型，则需要递归地调用，
            // //现行的结构不适合递归
            // //要么在这个分支语句里处理完，要么修改架构
            // assert(0);//先不处理
        }
        else if (!strcmp(root->child[1]->token, "LB")){
            #ifdef DEBUG
            printf(" Exp->Exp LB Exp RB\n");
            #endif  
            //整体上, 第一个exp可能包含多维，第二个exp是一个数字/变量
            //假如对第一个exp采用递归式地求解  可是我们难以获得它的id
                #ifdef DEBUG_LINE
                printf("INFO: line: %d\n", root->lineno);
                #endif
            Operand t1 = newOp(OP_TEMP);
            InterCodes code1 = trans_Exp(root->child[0], t1);
            assert(t1->type->kind ==ARRAY);
            assert(t1->isAddr);
            assert(code1);
            Operand t2 = newOp(OP_TEMP);
            InterCodes code2 = trans_Exp(root->child[2], t2);
            // printf("t1 type: %d\n",t1->type->kind);
            // assert(t1->type->kind==ARRAY);
            int n1;
            if(t1->type->kind==ARRAY){
                t1->type = t1->type->u.array.elem;
                n1 = typeSize(t1->type);//数组arr的大小
            }
            else if(t1->type->kind==STRUCTURE){
                assert(0);
                n1 = typeSize(t1->type);
            }
            else 
                n1 = 4;//BASIC
            #ifdef DEBUG_ARRA
            printf("arra type: %d, type size: %d\n",t1->type->kind, n1);
            #endif
            Operand ts = newOp(OP_CONSTANT, n1);
            // Operand t3 = newOp(TEMP);
            InterCodes code3 = newIR(IR_MUL, t2, ts, t2);
            // assert(place);
            if(!place){
                place = newOp(OP_TEMP);
            }
            place->isAddr = true;
            InterCodes code4 = newIR(IR_ADD, place, t1, t2);
            
            if(t1->type->kind == ARRAY){
                if(t1->type->u.array.elem->kind == STRUCTURE)   //加上[]是结构体，不会再递归为数组
                    place->type = t1->type->u.array.elem;
                else
                    place->type = t1->type;
            }
                
            else if(t1->type->kind == STRUCTURE){
                place->type = t1->type;
                // printf("####INFO:\n");
            }
            else
                place->type = NULL;

            concatIR(4, code1, code2, code3, code4);
            #ifdef DEBUG
            printf("   leave exp[exp]\n");
            #endif
            return code1;



            // if(!strcmp(root->child[0]->child[0]->token, "ID") && root->child[0]->childNum==1){
            //     //先做最简单的一位数组
            //     //第一个子节点是Exp->ID，即ID [Exp] 的形式
            //     char* name = root->child[0]->child[0]->val;
            //     assert(name);
            //     Operand t1 = newOp(OP_TEMP);
            //     t1->isAddr = true;
            //     InterCodes code1 = trans_Exp(root->child[0], t1);//base
            //     // t1->isAddr = true;
            //     Var arra = hash_find(name);
            //     assert(arra);
            //     int baseSize = typeSize(arra->type->u.array.elem);
            //     Operand bs = newOp(OP_CONSTANT, baseSize);
            //     Operand t2 = newOp(OP_TEMP);
            //     printf("NO: %d\n", t2->u.var_no);
            //     t2->isAddr = true;
            //     InterCodes code2 = trans_Exp(root->child[2], t2);
            //     // t2->isAddr = true;
            //     Operand t3 = newOp(OP_TEMP);//offset
            //     t3->isAddr = true;
            //     InterCodes code3 = newIR(IR_MUL, t3, t2, bs);
            //     //把值存在t1里面
            //     // assert(place);
                
                
            //     InterCodes code4 = NULL;
            //     if(place){
            //         // if(place->isAddr)
            //             code4 = newIR(IR_ADD, place, t1, t3);
            //         // else  /  

            //     }
            //     concatIR(4, code1, code2, code3, code4);
            //     #ifdef DEBUG_LINE
            //     printf("INFO: lind: %d\n", root->lineno);
            //     #endif
            //     assert(code1);
            //     return code1;
            // }


            //数组  肯定要把place赋值
            assert(0);
        }
        else if(!strcmp(root->child[1]->token,"RELOP") || !strcmp(root->child[1]->token,"AND")
           || !strcmp(root->child[1]->token,"OR") || !strcmp(root->child[0]->token,"NOT")){

            #ifdef DEBUG
            printf(" Exp->Exp RELOP Exp, AND \n");
            #endif  

            // | Exp AND Exp
            // | Exp OR Exp
            // | Exp RELOP Exp
            // | NOT Exp
            Operand label1 = newOp(OP_LABEL);
            Operand label2 = newOp(OP_LABEL);
            Operand zero = newOp(OP_CONSTANT, 0);
            // assert(place);
            
            InterCodes code0 = NULL;
            if(place)
                code0 = newIR(IR_ASSIGN, place, zero);
            // else 
            //     assert(0);
            InterCodes code1 = trans_Cond(root, label1, label2);
            InterCodes code2_t1 = newIR(IR_LABEL, label1);
            Operand one  = newOp(OP_CONSTANT, 1);
            // assert(place);
            InterCodes code2_t2 = NULL;
            if(place)
                code2_t2 =  newIR(IR_ASSIGN, place, one);
            InterCodes code3 = newIR(IR_LABEL, label2);
            concatIR(5, code0, code1, code2_t1, code2_t2, code3);
            if(!code0)
                code0 = code1;
            #ifdef DEBUG
            printf("     leave Exp from relop\n");
            #endif
            assert(code0);
            return code0;
        }
        // | LP Exp RP
        // | ID LP Args RP
        // | ID LP RP
        else if(!strcmp(root->child[0]->token, "LP")){

            #ifdef DEBUG
            printf(" Exp->LP Exp RP\n");
            #endif  
            // assert(place);
            InterCodes code = trans_Exp(root->child[1], place);

            #ifdef DEBUG
            printf("     leave Exp from (Exp)\n");
            #endif
            assert(code);
            return code;
        }
        else if(!strcmp(root->child[0]->token, "ID")){

            #ifdef DEBUG
            printf(" Exp->ID LP (Args) RP\n");
            #endif  
            // function = lookup(sym_table, ID)
            // if (function.name == “read”) return [READ place]
            // return [place := CALL function.name]

            // function = lookup(sym_table, ID)
            // arg_list = NULL
            // code1 = translate_Args(Args, sym_table, arg_list)
            // if (function.name == “write”) return code1 + [WRITE arg_list[1]]
            // + [place := #0]
            Var v = hash_find(root->child[0]->val);
            assert(v);
            if(root->childNum ==3){//没有参数
                if(!strcmp(v->name, "read")){

                    #ifdef DEBUG
                    printf(" newIR for func read\n");
                    #endif
                    InterCodes c = newIR(IR_READ, place);
                    #ifdef DEBUG
                    printf(" return func read\n");
                    #endif    

                    return c;
                }
                assert(v->name);
                #ifdef DEBUG
                printf(" call func %s\n",v->name);
                #endif

                Operand fun = newOp(OP_FUNCTION, v->name);
                // assert(place);
                if(!place)//没有把函数调用的返回值赋给其他变量
                    place = newOp(OP_TEMP);
                InterCodes code =  newIR(IR_CALL, place, fun);

                #ifdef DEBUG
                printf("     leave Exp from fun call(no args)\n");
                #endif
                 assert(code);  return code;
            }
            else{//函数有参数               
            
                #ifdef DEBUG
                printf("  func %s has args\n",root->child[0]->val);
                #endif

                Arg_list arg_list = (Arg_list)malloc(sizeof(Arg_list_));
                arg_list->head = arg_list->tail = NULL;
                InterCodes code1 = trans_Args(root->child[2], arg_list);
                if(!strcmp(v->name, "write")){
                    // printf("AAA\n");
                    //get arg_list[1], 下标应该是从1开始的
                    // Operand arg1 = newOp(OP_VARIABLE);
                    Operand arg1 = NULL;
                    if(arg_list->head )//写的话只有一个变量
                        arg1 = arg_list->head->arg;
                    else 
                        assert(0);
                    #ifdef DEBUG_args
                    printf("INFO: arg1: %d\n",arg1->isAddr);
                    #endif
                    Operand zero = newOp(OP_CONSTANT, 0);
                    InterCodes tmp_load = load(&arg1);
                    // assert(tmp_load);
                    InterCodes code2 = newIR(IR_WRITE, arg1);
                    assert(code2);
                    // assert(place);s
                    InterCodes code3 = NULL;
                    if(place)
                        code3 =  newIR(IR_ASSIGN, place, zero);
                    #ifdef DEBUG
                    printf("INFO: tmp_load: %0x\n",tmp_load);
                    #endif
                    if(!code1 && !tmp_load){
                        concatIR(2, code2, code3);
                        code1 = code2;
                    }
                    else if(!code1){
                        concatIR(3, tmp_load, code2, code3);
                        code1 = tmp_load;                        
                    }
                    else 
                        concatIR(4, code1, tmp_load,  code2, code3);

                    #ifdef DEBUG
                    printf("     leave Exp from func write\n");
                    #endif
                     assert(code1);  return code1;;
                }
                //其他有参数的函数
                Arg tmp = NULL;

                InterCodes tmp_code = NULL;
                // tmp_code = arg_list->tail;
                // while(tmp){
                //     if(tmp->arg->isAddr && )

                // }

                InterCodes pre = code1;
                InterCodes cur = NULL;
                
                // for i = 1 to length(arg_list) code2 = code2 + [ARG arg_list[i]]
                // return code1 + code2 + [place := CALL function.name]
                tmp = arg_list->head;
                while(tmp){
                    if((tmp->arg->kind==ARRAY || tmp->arg->kind==STRUCTURE) && !tmp->arg->isAddr){
                        Operand t = newOp(OP_TEMP);
                        t->isAddr = true;
                        tmp_code = newIR(IR_GET_ADDRESS, t, tmp->arg);
                        if(pre){
                            concatIR(2, pre, tmp_code);
                        }
                        pre = tmp_code;
                    }//数组和结构体需要获得地址
                    else if(tmp->arg->isAddr && !tmp->arg->type){
                        tmp_code = load(&tmp->arg);
                        if(pre)
                            concatIR(2, pre, tmp_code);
                        pre = tmp_code;
                    }
                        
                    cur = newIR(IR_ARG, tmp->arg);//每次为参数 新建一个中间代码
                    assert(cur);
                    if(!pre)
                        code1 = cur;
                    else
                        concatIR(2, pre, cur);//添加在已有链表的后面
                    pre = cur;//但是由于Arg的参数print需要与实际参数的位置相反
                    tmp = tmp->next;//因此在获得arg_list的时候需要反着来
                }
                assert(v->name);
                Operand fun = newOp(OP_FUNCTION, v->name);
                #ifdef DEBUG_LINE
                printf("INFO: line: %d\n", root->lineno);
                #endif
                // assert(place);
                if(!place)
                    place = newOp(OP_TEMP);                
                cur = newIR(IR_CALL, place, fun);
                concatIR(2, pre, cur);

                #ifdef DEBUG
                printf("     leave Exp from fun call with args\n");
                #endif
                // assert(code1);  
                return code1;;

            }
        }
    // }
}
InterCodes trans_Args(pNode root, Arg_list arg_list){
// Args → Exp COMMA Args
// | Exp  
    #ifdef DEBUG
    printf("#enter Args\n");
    #endif
    // Operand t1 = ifID(root->child[0]->child[0]);//Args->Exp->ID
    // InterCodes code1 = NULL;

    // if(!t1){
    Operand t1  = newOp(OP_TEMP);
    InterCodes code1 = trans_Exp(root->child[0], t1);//如果ID对应的是数组/结构体，则会生成取地址&操作
        //如果Exp -> Exp [Exp] 那么t1也是它的地址✔
    // }
    //t1是args前面的参数，因此要插入到头部，因为在对函数的参数解析的时候是按顺序生成中间代码的

    //↑这个不对，传入的arg_list是前面已经获得好了的参数列表，我们再加入参数时，需要加入到其尾部
    //如果是插入到头部则应该先处理Args再处理Exp
    //但是想一想，要这个有head和tail似乎没什么用？只是给了一个包装让空的参数列表也可以传到下一层


    //11.16 21:11应该插入头部以保证arg_list中参数的    ####逆序#####
    Arg t1_arg = (Arg)malloc(sizeof(Arg_));//从Exp1获得的参数
    t1_arg->arg = t1;
    t1_arg->pre = NULL;
    t1_arg->next = NULL;
    if(!arg_list->head){
        //此时链表为空
        arg_list->head = arg_list->tail = t1_arg;

    }
    else {//把这个加到头部
        t1_arg->next = arg_list->head;
        arg_list->head->pre =  t1_arg;
        arg_list->head = t1_arg;
        // arg_list->tail->next = t1_arg;
        // arg_list->tail = t1_arg;
    }
    if(root->childNum == 1){
        #ifdef DEBUG
        printf("     leave Args\n");
        #endif

           return code1;;
    }
    else {

        InterCodes code2 = trans_Args(root->child[2], arg_list);
        if(!code1)
            code1 = code2;
        else 
            concatIR(2, code1, code2);
        #ifdef DEBUG
        printf("     leave Args");
        #endif
    
        //  assert(code1);  
        return code1;;
    }
}
InterCodes trans_Stmt(pNode root){
    // Stmt → 
    // | 
    #ifdef DEBUG
    printf("#enter Stmt\n");
    #endif
    if(root->childNum == 1){//CompSt
        return trans_Compst(root->child[0]);
    }
    else if(root->childNum == 2){//Exp SEMI 
        #ifdef DEBUG
        printf(" Stmt->Exp\n");
        printf("     lineno: %d\n",root->lineno);
        #endif
        
        return trans_Exp(root->child[0], NULL);//这个为空怎么办，会不会影响到后面exp中的对place赋值什么的
    }
    else if(root->childNum == 3){// | RETURN Exp SEMI
        // t1 = new_temp()
        // code1 = translate_Exp(Exp, sym_table, t1)
        // code2 = [RETURN t1]
        // return code1 + code2
        Operand t1 = newOp(OP_TEMP);
        // t1->isAddr = false;
        InterCodes code1 = trans_Exp(root->child[1], t1);
        if(t1->isAddr){
            // Operand tmp = newOp(OP_TEMP);
            InterCodes tmp_code = load(&t1);
            concatIR(2, code1, tmp_code);
        }
        InterCodes code2 = newIR(IR_RETURN, t1);
        concatIR(2, code1, code2);
        #ifdef DEBUG
        printf("     leave Stmt\n");
        #endif
         assert(code1);  return code1;;
    }
    else if(!strcmp(root->child[0]->token, "IF")){
    // | IF LP Exp RP Stmt +
    // | IF LP Exp RP Stmt ELSE Stmt
        Operand label1 = newOp(OP_LABEL);
        Operand label2 = newOp(OP_LABEL);
        InterCodes code1 = trans_Cond(root->child[2], label1, label2);
        InterCodes code2 = trans_Stmt(root->child[4]);
        InterCodes mid1 = newIR(IR_LABEL, label1);
        InterCodes mid2 = newIR(IR_LABEL, label2);
        if(root->childNum == 5){
            concatIR(4, code1, mid1, code2, mid2);

    #ifdef DEBUG
    printf("     leave Stmt\n");
    #endif
             assert(code1);  return code1;;
        }
        else {
            Operand label3 = newOp(OP_LABEL);
            InterCodes code3 = trans_Stmt(root->child[6]);
            InterCodes gt = newIR(IR_GOTO, label3);
            InterCodes mid3 = newIR(IR_LABEL, label3);
            concatIR(7, code1, mid1, code2, gt, mid2, code3, mid3);
    #ifdef DEBUG
    printf("     leave Stmt\n");
    #endif
             assert(code1);  return code1;;
        }    
    }
    else if(!strcmp(root->child[0]->token, "WHILE")){
        // WHILE LP Exp RP Stmt

        // label1 = new_label()
        // label2 = new_label()
        // label3 = new_label()
        // code1 = translate_Cond(Exp, label2, label3, sym_table)
        // code2 = translate_Stmt(Stmt1, sym_table)
        // return [LABEL label1] + code1 + [LABEL label2] + code2
        // + [GOTO label1] + [LABEL label3]
        Operand label1 = newOp(OP_LABEL);
        Operand label2 = newOp(OP_LABEL);
        Operand label3 = newOp(OP_LABEL);
        InterCodes code1 = trans_Cond(root->child[2], label2, label3);
        InterCodes code2 = trans_Stmt(root->child[4]);
        InterCodes mid1 = newIR(IR_LABEL, label1);
        InterCodes mid2 = newIR(IR_LABEL, label2);
        InterCodes gt = newIR(IR_GOTO, label1);
        InterCodes mid3 = newIR(IR_LABEL, label3);
        concatIR(6, mid1, code1, mid2, code2, gt, mid3);
    #ifdef DEBUG
    printf("     leave Stmt\n");
    #endif
        return mid1;
    }
}

InterCodes trans_Cond(pNode root, Operand label_true, Operand label_false){
    // | Exp AND Exp
    // | Exp OR Exp
    // | Exp RELOP Exp
    // | NOT Exp
    if(root->childNum == 3){//呃 会不会把other case 也包含进来 应该不会
        if(!strcmp(root->child[1]->token, "RELOP")){
            // Operand t1 = newOp(OP_TEMP);
            // Operand t2 = newOp(OP_TEMP);
            // InterCodes code1 = trans_Exp(root->child[0], t1);
            // InterCodes code2 = trans_Exp(root->child[2], t2);
            Operand t1 = ifID(root->child[0]->child[0]), t2 = ifID(root->child[2]->child[0]);
            InterCodes code1 = NULL, code2 = NULL;
            if(!t1){
                t1 = newOp(OP_TEMP);
                code1 = trans_Exp(root->child[0], t1);
            }
            if(!t2){
                t2 = newOp(OP_TEMP);
                code2 = trans_Exp(root->child[2], t2);
            }
            if(t1->isAddr && !t2->isAddr){
                Operand t3 = newOp(OP_TEMP);
                InterCodes tmp1 = newIR(IR_GET_CONTENT, t3, t1);
                if(code2)
                    concatIR(2, code2, tmp1);
                else    
                    code2 = tmp1;
                t1 = t3;
            }
            else if(!t1->isAddr && t2->isAddr){
                Operand t4 = newOp(OP_TEMP);
                InterCodes tmp2 = newIR(IR_GET_CONTENT, t4, t2);
                if(code2)
                    concatIR(2, code2, tmp2);
                else 
                    code2 = tmp2;
                t2 = t4;
            }
            else if(t1->isAddr && t2->isAddr && !t1->type && !t2->type){
                Operand t3 = newOp(OP_TEMP);
                InterCodes tmp1 = newIR(IR_GET_CONTENT, t3, t1);
                if(code2)
                    concatIR(2, code2, tmp1);
                else    
                    code2 = tmp1;
                t1 = t3;   
                
                Operand t4 = newOp(OP_TEMP);
                InterCodes tmp2 = newIR(IR_GET_CONTENT, t4, t2);
                if(code2)
                    concatIR(2, code2, tmp2);
                else 
                    code2 = tmp2;
                t2 = t4;             
            }
            Operand op  = newOp(OP_RELOP, root->child[1]->val);
            InterCodes code3 = newIR(IR_IF_GOTO, t1, op, t2, label_true);
            InterCodes code4 = newIR(IR_GOTO, label_false);
            if(code1)
                concatIR(4, code1, code2, code3, code4);
            else if(code2){
                concatIR(3, code2, code3, code4);
                code1 = code2;
            }
            else {
                concatIR(2, code3, code4);
                code1 = code3;
            }
             assert(code1);  return code1;;
        }
        else if(!strcmp(root->child[1]->token, "OR")){
            Operand label1 = newOp(OP_LABEL);
            InterCodes code1 = trans_Cond(root->child[0], label_true, label1);
            InterCodes code2 = trans_Cond(root->child[2], label_true, label_false);
            InterCodes mid  = newIR(IR_LABEL, label1);
            concatIR(3, code1, mid, code2);
            return code1;
        }
        else if(!strcmp(root->child[1]->token, "AND")){   
            Operand label1 = newOp(OP_LABEL);
            InterCodes code1 = trans_Cond(root->child[0], label1, label_false);
            InterCodes code2 = trans_Cond(root->child[2], label_true, label_false);
            InterCodes mid  = newIR(IR_LABEL, label1);
            concatIR(3, code1, mid, code2);
             assert(code1);  return code1;;                  
        }
    }
    else if (root->childNum == 2 && !strcmp(root->child[0]->token, "NOT")){
        return trans_Cond(root->child[1], label_false, label_true);
    }
    // t1 = new_temp()
    // code1 = translate_Exp(Exp, sym_table, t1)
    // code2 = [IF t1 != #0 GOTO label_true]
    //  assert(code1);  return code1; + code2 + [GOTO label_false]    
    Operand t1 = newOp(OP_TEMP);
    InterCodes code1 = trans_Exp(root, t1);
    Operand op = newOp(OP_RELOP, "!=");
    Operand zero = newOp(OP_CONSTANT, 0);
    InterCodes code2 = newIR(IR_IF_GOTO, t1, op, zero, label_true);
    InterCodes code3 = newIR(IR_GOTO, label_false);
    concatIR(3, code1, code2, code3);
    return code1;
}