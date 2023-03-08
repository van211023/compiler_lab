#include"obCode.h"
// #define INFO
// void insertVar(FuncList fl, Operand op){
//     assert(fl);
//     assert(fl->cur);
//     if(!fl->cur->var){
//         fl->cur->var = newVarList();
//         fl->cur->var->cur=op;
//     }
// }
void initFunc(const InterCodes ir){
    InterCodes tmp = ir;
    f_List = NULL;
    FuncList cur_fl = NULL;
    bool con = false;
    while (tmp){
        switch (tmp->code->kind)
        {
        case IR_FUNCTION:
            cur_fl = (FuncList)malloc(sizeof(FuncList_));
            cur_fl->next = NULL;
            Func f = (Func)malloc(sizeof(Func_));
            Operand to = tmp->code->u.single.op;
            assert(to); assert(to->u.name);
            f->name = to->u.name;
            f->var = NULL;
            f->size = 0;
            cur_fl->cur = f;
            //insert cur_fl in funclist
            if(!f_List)
                f_List = cur_fl;
            else{
                FuncList t_fl = f_List;
                assert(t_fl);
                while (t_fl->next){
                    t_fl=t_fl->next;
                }
                t_fl->next = cur_fl;
            }

            break;
        case IR_PARAM:{//存到当前函数的域中，因此，先要找到当前函数
            //如果有参数，则参数是该函数第一个变量
            V_list vl = NULL;
            V_list tmp_vl = NULL;//tail of the list
            int cnt = 0;
            while (tmp->code->kind == IR_PARAM){
                cnt++;
                V_list tl = newVarList();
                Operand op = tmp->code->u.single.op;
                tl->cur = op;
                // printf("info: insert param %0x in %0x, op kind: %d, no: %d\n", op, cur_fl, op->kind, op->u.var_no);
                if(!vl){
                    vl = tl;
                    tmp_vl = vl;
                }
                else{
                    tmp_vl->next = tl;
                    tmp_vl = tmp_vl->next;
                }
                assert(tmp);
                tmp = tmp->next;
                // assert(tmp_vl->next);
                
                // if(cnt<5){//第五个参数的offset也是0
                //     // int base = 4;
                //     // reg[base+cnt].op=op;
                //     // continue;
                // }
                // //第5个参数以后
                // else if(cnt==5)
                //     tl->offset = 8;
                // else
                //     tl->offset = tmp_vl->offset + 4;//参数的offset是正的
            }
            cur_fl->cur->var = vl;
            cur_fl->cur->args = cnt;
            con = true;
            break;
        }
        case IR_ASSIGN:
        case IR_GET_ADDRESS: 
        case IR_GET_CONTENT:
        case IR_ASSIGN2CONTENT:
        case IR_ASSIGN_BT_ADDRESS:{
            // printf(":%0x\n",tmp->code->u.op1.result);
            insertVar(cur_fl, tmp->code->u.op1.result);
            insertVar(cur_fl, tmp->code->u.op1.right);
            break;
        }
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV:{
            insertVar(cur_fl, tmp->code->u.op2.result);
            insertVar(cur_fl, tmp->code->u.op2.op1);
            insertVar(cur_fl, tmp->code->u.op2.op2);
            break;
        }
        case IR_CALL:
        case IR_DEC://数组（没有结构体）
        case IR_READ:{
            insertVar(cur_fl, tmp->code->u.single.op);
            break;            
        }
        //IR_IF_GOTO,IR_GOTO,IR_LABEL,IR_RETURN,IR_ARG,IR_WRITE不会产生新的变量
        default:
            break;
        }
        if(con){
            con = false;
            continue;
        }
        assert(tmp);
        tmp = tmp->next;
    }
    
}
void obCode(FILE* out){
    initFunc(ir_list);
    initReg();
    initOthers(out);
    mapping(out);
}
void initOthers(FILE* out){
    fprintf(out, ".data\n");
    fprintf(out, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(out, "_ret: .asciiz \"\\n\"\n");
    fprintf(out, ".globl main\n");
    fprintf(out, "\n");

    fprintf(out, ".text\n");
    fprintf(out, "read:\n");
    fprintf(out, "  li $v0, 4\n");
    fprintf(out, "  la $a0, _prompt\n");
    fprintf(out, "  syscall\n");
    fprintf(out, "  li $v0, 5\n");
    fprintf(out, "  syscall\n");
    fprintf(out, "  jr $ra\n");
    fprintf(out, "\n");

    fprintf(out, "write:\n");
    fprintf(out, "  li $v0, 1\n");
    fprintf(out, "  syscall\n");
    fprintf(out, "  li $v0, 4\n");
    fprintf(out, "  la $a0, _ret\n");
    fprintf(out, "  syscall\n");
    fprintf(out, "  move $v0, $0\n");
    fprintf(out, "  jr $ra\n");
    fprintf(out, "\n");    
}
void insertVar(FuncList fl, const Operand op){
    //先找到当前函数的链表，即最后一个函数链表
    // FuncList fl = f_List;
    assert(fl);
    if(op->kind==OP_CONSTANT)
        return;
    // while (fl->next){
    //     fl = fl->next;
    // }
    assert(fl->cur);
    V_list vl = fl->cur->var;
    //vl为null
    if(!vl){//肯定不会是param
        vl = newVarList();
        // printf("info: insert %0x in %0x, op kind: %d, no: %d\n", op, fl, op->kind, op->u.var_no);
        vl->cur = op;
        fl->cur->var = vl;
        vl->offset = -4;//栈中第一个局部变量对fp 的偏移量为-4
        //传入参数的偏移量应该是多少？
        return;
    }
    //查找vl中有无op
    while(vl->next){//使得vl不是最后一个
        if(vl->cur==op)
            return;
        vl = vl->next;
    }
    //判断最后一个是不是op
    if(vl->cur==op)
        return;
    //插入到vl后面
    int off = 0;
    if(vl->cur->kind==OP_TEMP)
        off = 4;
    else {
        // printf("debug: kind:%d\n",op->kind);
        off = typeSize(vl->cur->type);
    }
    V_list new_vl = newVarList();
    // printf("info: insert %0x in %0x, op kind: %d, no: %d\n", op, fl, op->kind, op->u.var_no);
    new_vl->cur = op;
    new_vl->offset = vl->offset - off;//offset为负数
    vl->next = new_vl;
}
V_list newVarList(){
    V_list vl = (V_list)malloc(sizeof(V_list_));
    vl->offset = 0;
    vl->cur = NULL;
    vl->next = NULL;
    return vl;
}
void initReg(){
    char* name[32] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
    reg = (Register)malloc(sizeof(Register_)*REGISTER_NUM);
    for(int i = 0; i < REGISTER_NUM; i++){
        reg[i].name = name[i];
        reg[i].op = NULL;
        reg[i].state = R_FREE;
    }
}
int getReg(Operand op){
    //先查询放参数的寄存器
    if(op!=NULL){
        //为NULL时只是单纯找一个空闲的寄存器，尽量不要占用这里的 
        for(int i = 4; i < 8; i++){
            if(reg[i].state == R_BUSY && reg[i].op == op)
                return i;
        }
    }

    for(int i = 8; i < 16; i++){
    //$t0——$t7 每条指令的操作符不会超过8个
        if(reg[i].state == R_FREE){
            reg[i].op = op;
            reg[i].state = R_BUSY;
            return i;
        }
    }
    assert(0);
}
void storeOp(Operand op, FILE* out, int offset, int regIndx, bool wr){
    if(regIndx >= 8 && regIndx < 16){//存在参数对应寄存器中的变量无须操作
        assert(reg[regIndx].state==R_BUSY);
        // printf("#%u, %u\n",reg[regIndx].op, op);
        // assert(reg[regIndx].op==op);
        //如return等这种语句，不需要存储该变量，即虽然寄存器存的不是NULL，但是传入的是NULL
        //把该寄存器的值存到变量：
        if(op&&op->kind!=OP_CONSTANT && wr)//op不是null 也不是常量
            fprintf(out, "  sw %s, %d($fp)\n", reg[regIndx].name, offset);
        reg[regIndx].state = R_FREE;
    }
}
void mapping(FILE* out){
    InterCodes tmp = ir_list;
    FuncList fl = NULL;
    int args_cnt = 0;
    int para_cnt = 0;
    bool rtn = true;
    bool save = false;
    while(tmp){
        
        switch (tmp->code->kind){
        case IR_FUNCTION:{//找到函数的定义，fl置为当前函数对应的FunctList
            //fl = cur func list
            para_cnt = 0;//每次进入para_cnt
            char* name = tmp->code->u.single.op->u.name;

            if(!rtn){
                fprintf(out, "  addi $sp, $sp, %d\n", fl->cur->size);
                fprintf(out, "  lw $fp, 0($sp)\n");
                fprintf(out, "  addi $sp, $sp, 4\n");
                fprintf(out, "  jr $ra\n");
            }
            else    
                rtn = false;//进入一个新的函数
            // printf("debug: wanna find name: %s\n", name);
            fl = f_List;
            bool find = false;
            assert(f_List);
            while(fl){
                // printf("debug: find name: %s\n", fl->cur->name);
                if(!strcmp(name, fl->cur->name)){
                    

                    find = true;
                    break;
                }
                fl=fl->next;
            }
            assert(find);//找到fl
            fprintf(out, "%s:\n",name);
            //push ebp
            fprintf(out, "  addi $sp, $sp, -4\n");
            fprintf(out, "  sw $fp, 0($sp)\n");
            fprintf(out, "  move $fp, $sp\n");
            
            V_list vl = fl->cur->var;
            assert(vl);
            while(vl->next){
                vl = vl->next;
            }
            int fsize = -vl->offset+typeSize(vl->cur->type);
            fl->cur->size = fsize;
            fprintf(out, "  addi $sp, $sp, %d\n", -fsize);
            break;
        }
        case IR_PARAM:{
            para_cnt++;
            int t = fl->cur->args-para_cnt;//总参数-当前的个数=倒数第几个参数（下标从0开始），即arg的正序
            int base = 4;
            printf("arg num: %d, t: %d\n", fl->cur->args, t);
            Operand op = tmp->code->u.single.op;
            if(t<4){//arg 5 4 3 2 1放到$a0-a4
                // printf("xx\n");
                reg[base+t].op=tmp->code->u.single.op;
                reg[base+t].state=R_BUSY;
                // assert(reg[base+t].state==R_BUSY);
                // Operand op = tmp->code->u.single.op;
                // int of = getOpOffset(fl,op);
                // int r=loadOp(tmp->code->u.single.op,out,of);
                // printf("name: %s\n", reg[r].name);
            }
            else{//find it in the varlist
                V_list tmp_vl = fl->cur->var;
                while (tmp_vl){
                    if(tmp_vl->cur==op){
                        // t=fl->cur->args-4
                        tmp_vl->offset=8+(para_cnt-1)*4;//第五个的话，i=4；第六个的话i=5   
                        break;
                    }
                    tmp_vl=tmp_vl->next;
                }
                
            }
            break;
        }
        case IR_ASSIGN:{//load, assign, store
            #ifdef INFO
            printf("info: enter IR_ASSIGN\n");
            #endif
            Operand op1 = tmp->code->u.op1.result;
            assert(op1);
            assert(op1->kind==OP_TEMP||op1->kind==OP_VARIABLE);
            Operand op2 = tmp->code->u.op1.right;
            assert(op2);
            // printf("DEBUG: kind: %d, no: %d, addr: %0x\n",op1->kind,op1->u.var_no, tmp->code->u.op1.result);         

            int offset1 = getOpOffset(fl, op1);

            int r1 = loadOp(op1, out, offset1);  
            assert(reg[r1].state==R_BUSY);
            // printf("DEBUG: kind: %d, no: %d, r1: %d\n",op1->kind,op1->u.var_no ,r1);         
            if(op2->kind==OP_CONSTANT){//可能为整数赋值给一个变量
                fprintf(out, "  li %s, %d\n",reg[r1].name, op2->u.value);
                assert(reg[r1].state==R_BUSY);
                // printf("@@\n");

                // printf("INFO: op: %u\n", op1);
                storeOp(op1,out,offset1,r1, true);
                // printf("store end\n");
            }
            else{
                int offset2 = getOpOffset(fl, op2);
                int r2 = loadOp(op2, out, offset2);
                fprintf(out, "  move %s, %s\n",reg[r1].name, reg[r2].name);
                assert(reg[r1].state==R_BUSY);
                // printf("##\n");
                storeOp(op1,out,offset1,r1, true);
                assert(reg[r2].state==R_BUSY);
                // printf("$$\n");
                storeOp(op2,out,offset2,r2, false);
                // printf("000\n");
            }
            break;
        }
        case IR_ADD:
        case IR_DIV:
        case IR_SUB:
        case IR_MUL:
        {//t1 = t2 + t3;  add reg(t1),reg(t2),reg(t3)
        //不会出现addi
            #ifdef INFO
            printf("info: enter IR_ADD\n");
            #endif

            Operand t1 = tmp->code->u.op2.result;
            Operand t2 = tmp->code->u.op2.op1;//可能为常数
            Operand t3 = tmp->code->u.op2.op2;
            assert(t1->kind==OP_TEMP); 
            // assert(t2->type==OP_TEMP); 
            assert(t3->kind==OP_TEMP);
            int of1 = getOpOffset(fl,t1);
            int r1 = loadOp(t1, out, of1);
            int of2 = 0, r2 = 0;
            if(t2->kind==OP_CONSTANT){
                r2 = getReg(t2);
                fprintf(out, "  li %s, %d\n", reg[r2].name,t2->u.value);
            }
            else{
                of2 = getOpOffset(fl,t2);
                r2 = loadOp(t2, out, of2);
            }
            int of3 = getOpOffset(fl,t3);
            int r3 = loadOp(t3, out, of3);  
            switch (tmp->code->kind){
            case IR_ADD:
                fprintf(out, "  add %s, %s, %s\n",reg[r1].name, reg[r2].name, reg[r3].name); 
                break;  
            case IR_DIV:
                fprintf(out, "  div %s, %s\n", reg[r2].name, reg[r3].name); 
                fprintf(out, "  mflo %s\n", reg[r1].name); 
                break;  
            case IR_SUB:
                fprintf(out, "  sub %s, %s, %s\n",reg[r1].name, reg[r2].name, reg[r3].name); 
                break; 
            case IR_MUL:
                fprintf(out, "  mul %s, %s, %s\n",reg[r1].name, reg[r2].name, reg[r3].name); 
                break;                              
            default:
                break;
            }
            assert(reg[r1].state==R_BUSY);
            storeOp(t1,out,of1,r1, true);
            // if(t2->kind!=OP_CONSTANT)
            assert(reg[r2].state==R_BUSY);
                storeOp(t2,out,of2,r2, false);
            assert(reg[r3].state==R_BUSY);
            storeOp(t3,out,of3,r3, false);
            
            break;
        }
        case IR_GOTO:{
            #ifdef INFO
            printf("info: enter IR_GOTO\n");
            #endif

            Operand op = tmp->code->u.single.op;
            fprintf(out, "  j label%d\n",op->u.var_no);
            break;
        }
        case IR_IF_GOTO:{
            #ifdef INFO
            printf("info: enter IR_IF_GOTO\n");
            #endif
            Operand t1 = tmp->code->u.if_goto.t1;
            Operand t2 = tmp->code->u.if_goto.t2;
            int of1 = getOpOffset(fl,t1);
            int r1 = loadOp(t1, out, of1);
            int of2 = getOpOffset(fl,t2);
            int r2 = loadOp(t2, out, of2);
            Operand op = tmp->code->u.if_goto.op;
            Operand  label = tmp->code->u.if_goto.label_true;
            // printf("INFO: %s\n", op->u.name);
            if(!strcmp(op->u.name, "=="))
                fprintf(out, "  beq %s, %s label%d\n",reg[r1].name, reg[r2].name, label->u.var_no); 
            else if(!strcmp(op->u.name, "!="))
                fprintf(out, "  bne %s, %s label%d\n",reg[r1].name, reg[r2].name, label->u.var_no); 
            else if(!strcmp(op->u.name, ">"))
                fprintf(out, "  bgt %s, %s label%d\n",reg[r1].name, reg[r2].name, label->u.var_no); 
            else if(!strcmp(op->u.name, "<"))
                fprintf(out, "  blt %s, %s label%d\n",reg[r1].name, reg[r2].name, label->u.var_no); 
            else if(!strcmp(op->u.name, ">="))
                fprintf(out, "  bge %s, %s label%d\n",reg[r1].name, reg[r2].name, label->u.var_no); 
            else if(!strcmp(op->u.name, "<="))
                fprintf(out, "  ble %s, %s label%d\n",reg[r1].name, reg[r2].name, label->u.var_no); 
            assert(reg[r1].state==R_BUSY);
            storeOp(t1,out,of1,r1, false);
            assert(reg[r2].state==R_BUSY);
            storeOp(t2,out,of2,r2, false);
            break;
        }
        case IR_GET_CONTENT://x= *y
        case IR_ASSIGN2CONTENT://*x = y
        case IR_GET_ADDRESS://x = &y
        case IR_ASSIGN_BT_ADDRESS://*x=*y
        {
            #ifdef INFO
            printf("info: enter IR_GET_CONTENT\n");
            #endif
            Operand op1 = tmp->code->u.op1.result;
            Operand op2 = tmp->code->u.op1.right;
            int of1 = getOpOffset(fl,op1);
            int r1 = loadOp(op1, out, of1);
            int of2 = getOpOffset(fl,op2);
            int r2 = 0;
            if(tmp->code->kind!=IR_GET_ADDRESS)
                r2 = loadOp(op2, out, of2);
            else{
                r2 = getReg(op2);
                int size = typeSize(op2->type);
                fprintf(out, "  addi %s, $fp, %d\n", reg[r2].name, of2-size);
                //在r2中存储op2的地址
            }
            switch (tmp->code->kind)
            {
            case IR_GET_CONTENT:
                fprintf(out, "  lw %s, 0(%s)\n", reg[r1].name, reg[r2].name); 
                break;
            case IR_ASSIGN2CONTENT:
                fprintf(out, "  sw %s, 0(%s)\n", reg[r2].name, reg[r1].name);
                break; 
            case IR_GET_ADDRESS:
                fprintf(out, "  move %s, %s\n", reg[r1].name, reg[r2].name);
                break;   
            case IR_ASSIGN_BT_ADDRESS:{//*x = *y
                int tr = getReg(NULL);
                fprintf(out, "  lw %s, 0(%s)\n", reg[tr].name, reg[r2].name); //t=*y
                fprintf(out, "  sw %s, 0(%s)\n", reg[tr].name, reg[r1].name); //*x=t
                assert(reg[tr].state==R_BUSY);
                storeOp(NULL,out,0,tr, false);
                break;
            }
            default:
                break;
            }
            assert(reg[r1].state==R_BUSY);
            storeOp(op1,out,of1,r1, true);
            assert(reg[r2].state==R_BUSY);
            storeOp(op2,out,of2,r2, false);
            break;
        }
        // case IR_GET_ADDRESS:{
        //     Operand op1 = tmp->code->u.op1.result;
        //     Operand op2 = tmp->code->u.op1.right;
        //     int of1 = getOpOffset(fl,op1);
        //     int r1 = loadOp(op1, out, of1);
        //     int of2 = getOpOffset(fl,op2);
        //     int r2 = loadOp(op2, out, of2);            
        // }
        case IR_LABEL:{
            Operand op = tmp->code->u.single.op;
            assert(op->kind==OP_LABEL);
            fprintf(out, "label%d:\n", op->u.var_no); 
            break;
        }
        case IR_CALL:
        case IR_READ:{
            #ifdef INFO
            printf("info: enter IR_CALL\n");
            #endif

            args_cnt = 0;//每次进入IR_CALL，cnt置0
            fprintf(out, "  addi $sp, $sp, -4\n");
            fprintf(out, "  sw $ra, 0($sp)\n");
            Operand x;
            if(tmp->code->kind==IR_READ){
                fprintf(out, "  jal read\n");
                x = tmp->code->u.single.op;
            }
            else{
                fprintf(out, "  jal %s\n", tmp->code->u.op1.right->u.name);
                x = tmp->code->u.op1.result;
            }            fprintf(out, "  lw $ra, 0($sp)\n");
            fprintf(out, "  addi $sp, $sp, 4\n");
            x = tmp->code->u.op1.result;
            int of = getOpOffset(fl, x);
            int r = loadOp(x,out,of);
            fprintf(out, "  move %s, $v0\n", reg[r].name);
            storeOp(x,out,of,r, true);
            if(tmp->code->kind==IR_CALL){
                releseParamREg(out);
                save = false;
            }
            break;
        }
        
        
        case IR_WRITE:{
            #ifdef INFO
            printf("info: enter IR_WRITE\n");
            #endif

            if(!save){//在传递参数之前保存
                save = true;
                saveParamReg(out);
            }            

            fprintf(out, "  addi $sp, $sp, -4\n");
            fprintf(out, "  sw $ra, 0($sp)\n");
            Operand x = tmp->code->u.single.op;
            int of = getOpOffset(fl, x);
            int r = loadOp(x,out,of);
            fprintf(out, "  move $a0, %s\n", reg[r].name);
            storeOp(x,out,of,r, false);
            fprintf(out, "  jal write\n");
            fprintf(out, "  lw $ra, 0($sp)\n");
            fprintf(out, "  addi $sp, $sp, 4\n");

            releseParamREg(out);
            save = false;            
            // releseParamREg(out);
            break;
        }
        case IR_ARG:{//要记录参数的个数
            //找空闲参数寄存器，存；否则压栈
            #ifdef INFO
            printf("info: enter IR_ARG\n");
            #endif
            if(!save){//在传递参数之前保存
                save = true;
                saveParamReg(out);
            }
            int base = 4;
            Operand op = tmp->code->u.single.op;
            int offset = getOpOffset(fl, op);
            int r = loadOp(op, out, offset);
            if(args_cnt<4){//第一个实际是最后一个参数，所以说是把后4个参数倒序存入
                //arg存入reg[base+args_cnt]
                fprintf(out, "  move %s, %s\n", reg[base+args_cnt].name, reg[r].name);
                reg[base+args_cnt].state = R_BUSY;
            }
            else{
                fprintf(out, "  addi $sp, $sp, -4\n");
                fprintf(out, "  sw %s, 0($sp)\n", reg[r].name);
            }
            assert(reg[r].state==R_BUSY);
            storeOp(NULL,out,0,r, false);//这里也不用存储该op，直接释放
            args_cnt++;
            break;
        }
        case IR_RETURN:{
            #ifdef INFO
            printf("info: enter IR_RETURN\n");
            #endif
            Operand op = tmp->code->u.single.op;
            int offset = getOpOffset(fl, op);
            int r = loadOp(op, out, offset);
            fprintf(out, "  move $v0, %s\n", reg[r].name);
            fprintf(out, "  addi $sp, $sp, %d\n", fl->cur->size);
            fprintf(out, "  lw $fp, 0($sp)\n");
            fprintf(out, "  addi $sp, $sp, 4\n");
            fprintf(out, "  jr $ra\n");
            assert(reg[r].state==R_BUSY);
            storeOp(NULL, out, 0, r, false);
            rtn = true;
            break;
        }
        default:
            break;
        }
        tmp = tmp->next;
    }
}
void printVl(V_list vl){
    printf("#######print v_list begin#########\n");
    V_list t = vl;
    while(t){
        // printf("op: %0x\n", t->cur);
        t=t->next;
    }
    printf("#########print v_list end#########\n");
}
int getOpOffset(FuncList fl, const Operand op){
    if(op->kind==OP_CONSTANT)//if_goto可能第二个变量为常数
        return 0;
    V_list vl = fl->cur->var;
    // printVl(vl);
    while(vl){
        // printf("find op: %0x\n", vl->cur);
        if(vl->cur == op){
            // printf("INFO: Find\n");
            return vl->offset;
        }
        vl = vl->next;
    }
    // printf("OP: %0x\n", op);
    assert(0);
}
int loadOp(Operand op, FILE* out, int offset){
    //先查询放参数的寄存器
    // int i=0;
    // for(i = 4; i < 8; i++){
    //     if(reg[i].state == R_BUSY && reg[i].op == op){
    //         assert(offset==0);
    //         return i;//在参数对应的寄存器中的变量offset一定为0
    //     }
    // }
    // //否则就需要load   
    // for(i = 8; i < 16; i++){
    // //$t0——$t7 每条指令的操作符不会超过8个
    //     if(reg[i].state == R_FREE){
    //         reg[i].op = op;
    //         reg[i].state = R_BUSY;
    //         break;
    //     }
    // } 
    int i = getReg(op);
    assert(i < 16);
    //要得到op在该函数的offset，
    //就需要知道当前在哪个函数下面，从而才能到对应的链表寻找
    // if(op->kind==OP_CONSTANT)
    //     fprintf(out, "li %s, %d\n", reg[i].name,op->u.value);
    // else

    //本来想把上述的分支语句加进来，这样就不必在上层函数判断了，但是又考虑到offset
    //如果将offset放到本函数内部来求，那么storeOp里面也得再求一次
    //增加了不必要的操作，
    if(i>=8 && op->kind!=OP_CONSTANT)
        fprintf(out, "  lw %s, %d($fp)\n", reg[i].name, offset);
    else if(i>=8)
        fprintf(out, "li %s, %d\n", reg[i].name, op->u.value);
    return i;
}
void saveParamReg(FILE* out){
    fprintf(out, "addi $sp, $sp, -16\n");
    fprintf(out, "sw $a0, 0($sp)\n");
    fprintf(out, "sw $a1, 4($sp)\n");
    fprintf(out, "sw $a2, 8($sp)\n");
    fprintf(out, "sw $a3, 12($sp)\n");
}
void releseParamREg(FILE* out){
    fprintf(out, "lw $a0, 0($sp)\n");
    fprintf(out, "lw $a1, 4($sp)\n");
    fprintf(out, "lw $a2, 8($sp)\n");
    fprintf(out, "lw $a3, 12($sp)\n");
    fprintf(out, "addi $sp, $sp, 16\n");

}