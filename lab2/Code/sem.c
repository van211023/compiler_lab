#include "sem.h"
// #define DEBUG_global
// #define DEBUG_type
// #define HELP
#define DEBUG_hash2
// #define DEBUG_s
// #define DEBUG_struct
#ifdef HELP
    #define DEBUG_type
    #define DEBUG 
    #define DEBUG_hash2
    #define DEBUG_struct
    #define DEBUG_func
    #define DEBUG2
    #define DEBUG_hash
#endif
void initStack(){
    newScope = true;
    s = NULL;
}
void printStack(){
    pStackNode ps = top();
    printf("*************print stack begin*************\n");
    if(!ps){
        printf("the stack is empty\n");

    }
    else {
        pTableNode pt = ps->node;
        while(pt){
            printf("name: %s. type: %d\n",pt->node->name, pt->node->type->kind);
            pt = pt->next;
        }
    }
    printf("*************print stack end*************\n");
}
void push(pStackNode t){
    t->next = s;//next point to the ptn
    // printf("stack top address:%lld\n",s);
    s = t;
    // printf("new stack top address:%lld\n",s);

}
bool pop(pStackNode* t){
    // printf("stack top address:%lld\n",s);
    if(!s)
        return false;
    else {
        *t = s;
        s = s->next;
        // printf("new stack top address:%lld\n",s);
        return true;
    }
}
bool isEmpty(){
    return !s;
}
pStackNode top(){
    return s;
}

bool findInScope(char* name){
    #ifdef DEBUG
    printStack();
    printf("info: enter findInScope, print all the symbol in this scope\n");
    #endif
    if(!name){
        assert(0);
        return false;
    }
    // assert(0);
    pStackNode ps = top();
    // assert(0);
    if(!ps){
        #ifdef DEBUG
        printf("info: the stack is empty!\n");
        #endif
        return false;
    }
    pTableNode pt = ps->node;
    // assert(0);
    while(pt){
        if(!pt->node->name){
            assert(0);
        }
        #ifdef DEBUG
        printf("info: symbol %s\n",pt->node->name);
        #endif       
        if(strcmp(pt->node->name,name)==0){//the same

            #ifdef DEBUG
            printf("info: find symbol %s redef\n",pt->node->name);
            #endif            
            return true;
        }
        pt=pt->next;
    }
    #ifdef DEBUG
    printf("info: print all the symbol end\n");
    #endif    
    return false;
}
void setNewScope(){
    #ifdef DEBUG
    printf("***********set new scope and init stack head \n");
    #endif
    pStackNode psn = (pStackNode)malloc(sizeof(StackNode));
    pTableNode ptn = (pTableNode)malloc(sizeof(TableNode));
    Var v = (Var)malloc(sizeof(Var_));
    v->name = (char*)malloc(sizeof("head"));
    strcpy(v->name, "head");
    v->type = (Type)malloc(sizeof(Type_));
    v->type->kind  = 0;
    ptn->node = v;
    ptn->next = NULL;
    psn->node = ptn;
    psn->next = NULL;
    push(psn);
    newScope = false;
}
void initHashTalbe(){
    hashTable = (TableNode**)malloc(sizeof(pTableNode)*(TABLE_SIZE+1));
    for(int i = 0; i < TABLE_SIZE + 1; i++)
        hashTable[i] = NULL;
}
void printHashTable(){
    printf("**********hash table begin***********\n");
    for(int i = 0; i <= TABLE_SIZE; i++){
        if(hashTable[i]){
            pTableNode pt = hashTable[i];
            while(pt){
                printf("name: %s, type: %d\n",pt->node->name,pt->node->type->kind);
                pt=pt->next;
            }
        }
    }
    printf("**********hash table end***********\n");
}
unsigned int hash_pjw(char* name)
{
    unsigned int val = 0, i;
    for (; *name; ++name){
        val = (val << 2) + *name;
        if (i = val & ~TABLE_SIZE) 
            val = (val ^ (i >> 12)) & TABLE_SIZE;
    }
    return val;
}
//???????????????,???????????????????????????????????????????????????????????????????????????????????????
//????????????????????????????????????????????????????????????
bool hash_insert(const Var v){
#ifdef DEBUG
    printf("debug: enter hash_insert\n");
#endif
    // if(newScope){//?????????????????????????????????????????????????????????????????????????????????
    // // printf(" ");
    // #ifdef DEBUG_hash
    //     printf("    debug: new scope\n");
    // #endif    
    //     newScope = false;
    //     pStackNode psn = (pStackNode)malloc(sizeof(StackNode));
    //     pTableNode ptn = (pTableNode)malloc(sizeof(TableNode));
    //     ptn->node = v;
    //     ptn->next = NULL;
    //     psn->node = ptn;
    //     psn->next = NULL;
    //     push(psn);
    //     //insert in the hash
    //     int val = hash_pjw(v->name);
    //     pTableNode p = (pTableNode)malloc(sizeof(TableNode));
    //     p->node = v;
    //     p->next = NULL;
    //     printHashTable();
    //     if(!hashTable[val])
    //         hashTable[val] = p;
    //     else {
    //         p->next = hashTable[val];
    //         hashTable[val] = p;
    //     }      
    //     #ifdef DEBUG_hash2
    //     printHashTable();
    //     printf("    info: insert new symbol %s in the table, set newScope = false\n",v->name);
    //     #endif  
    //     return true;
    // }
    //find, insert in the head
    // else{
        #ifdef DEBUG_hash
            printf("    debug: no new scope\n");
        #endif  
        pStackNode ps = top();
        if(!ps){
            #ifdef DEBUG_s
            printf(" info: the stack is empty!\n");
            #endif
            assert(0);
        }
        pTableNode pt = ps->node->next;
        // bool fe = false;//find error
        // while(pt){
        //     if(strcmp(pt->node->name,v->name)==0){//the same
        //         fe = true;
        //         //raise error handle
        //         //debug:
        //         //?????????????????????
        //         printf("name %s already existed!\n",v->name);
        //         return false;
        //     }
        //     if(pt->next)
        //         pt=pt->next;
        //     else 
        //         break;
        // }
        //insert:
        int val = hash_pjw(v->name);
        pTableNode p = (pTableNode)malloc(sizeof(TableNode));
        p->node = v;
        p->next = NULL;

        if(!hashTable[val])
            hashTable[val] = p;
        else {
            p->next = hashTable[val];
            hashTable[val] = p;
        }
        #ifdef DEBUG_hash2
        printf("    info: insert %s in the table\n",v->name);  
        #endif
        //insert in the list
        pTableNode p2 = (pTableNode)malloc(sizeof(TableNode));
        p2->node = v;
        p2->next = pt;//????????????

        // pt = p2;//????????????????????????pt???ps->node?????????????????????
        ps->node->next = p2;
        return true;
    // }

}
Var hash_find(char* name){
    //??????????????????????????????????????????
    int val = hash_pjw(name);
    if(!hashTable[val])
        return NULL;
    else{
        pTableNode pt = hashTable[val];
        while(pt){
            if(!strcmp(pt->node->name,name))
                return pt->node;
            pt = pt->next;
        }
        return NULL;
    }//?????????????????????????????????
    //stack????????????
    //???????????????????????????node?????????????????????????????????
}
bool hash_del(){
    //??????????????????????????????????????????
    // pStackNode ps = top(s);
    #ifdef DEBUG_hash
    printf(" enter hash del\n");
    printStack();
    #endif
    pStackNode ps;
    bool suc = pop(&ps);
    if(!suc){
        #ifdef DEBUG_hash
        printf("info: pop error!\n");
        #endif
        return false;
    }

    // printf("ps address:%lld\n",ps);
    pTableNode pt = ps->node->next;
    pTableNode t;
    char n[100];
    // strcpy(n)
    while(pt){
        //debug:
        // printf("pt is not null\n");
        // printf("info:debug: in loop\n");
        // printf("pt address:%lld\n",pt);
        t = pt;
        strcpy(n,t->node->name);
        pt = pt->next;
        
        // if(!pt)
        //     printf("pt is null\n");
        // else {
        //     printf("pt is not null\n");
            // printf("pt->next address:%lld\n",pt);
        // }
        
        free(t);//free table node
        //travel hash table
        // printf("pt is not null\n");
        //?????????name?????????
        //???????????????????????????????????????????????????????????????
        #ifdef DEBUG_hash
        printf("info : hash and del:%s\n",n);
        #endif
        int val = hash_pjw(n);
        // printf("del %s\n",n);
        if(!hashTable[val]){
            //deubg:
            #ifdef DEBUG_hash
            printf("no find %s in hash\n",n);     
            #endif      
            ;//raise error
        }
        else{
            #ifdef DEBUG_hash
            printf("find %s in hash\n",n);   
            #endif
            //????????????????????????
            
            if(!strcmp(hashTable[val]->node->name,n)){
                t = hashTable[val];
                hashTable[val]=hashTable[val]->next;
            }
            else{//??????????????????????????????????????????
                pTableNode pre = hashTable[val], cur = pre;
                while(cur){
                    if(!strcmp(cur->node->name,n)){

                        #ifdef DEBUG_hash

                        printf("         hash del name: %s\n",n);
                        #endif

                        t = cur;
                        pre->next = cur->next;
                        break;
                    }         
                    pre = cur;
                    cur = cur->next;           
                }
            }           
            del_ptn(t);
        }
    }
    free(ps);
    return true;

}

void del_ptn(pTableNode ptn){
    // free(ptn->node->name);
    // free(ptn->node->type);
    // free(ptn->node);//??????????????????fieldlist
    // free(ptn);
}
void initFuncList(){//???????????????????????????????????????
    funcList = (pTableNode)malloc(sizeof(TableNode));
    funcList->node = NULL;
    funcList->next = NULL;
}
void insertFunc(Var v){
    pTableNode pt = (pTableNode)malloc(sizeof(TableNode));
    pt->node = v;
    pt->next=funcList->next;
    funcList->next = pt;
}
bool removeFunc(char* name){

    pTableNode pre = funcList, cur = funcList->next;
    while(cur){
        if(!strcmp(cur->node->name,name)){
            pre->next = cur->next;
            del_ptn(cur);
            #ifdef DEBUG_func
            printf("  remove func : %s\n",name);
            #endif
            return true;
        }
        pre = cur;
        cur = cur->next;
    }
    return false;
}
Var findFunc(char* name){
    pTableNode pt = funcList->next;
    while(pt){
        if(!strcmp(pt->node->name, name))
            return pt->node;
        pt= pt->next;
    }
    return false;
}
void printUNfunc(){
    pTableNode pt = funcList->next;
    while(pt){
        semError(funcNotDef,pt->node->lineno,"Undefined function");
        pt = pt->next;
    }
}
void semError(int type, int lineno, char* info){
    printf("Error type %d at Line %d: %s.\n",type, lineno, info);
}
void semAnaly(pNode root){
    if(!root){
        //tree is null
        return;
    }
    initFuncList();
    initHashTalbe();
    initStack();
    setNewScope();

    //???read ??? write??????

    char r[10] = "read";
    char w[10] = "write";
    Type rt = (Type)malloc(sizeof(Type_));//???????????????
    Type wt = (Type)malloc(sizeof(Type_));
    rt->kind = wt->kind = FUNCTION;
    rt->u.func.nArgs = 0;//????????????
    wt->u.func.nArgs = 1;
    Type r_rt = (Type)malloc(sizeof(Type_));//???????????????
    Type w_rt = (Type)malloc(sizeof(Type_));
    r_rt->kind  = w_rt->kind = BASIC;
    r_rt->u.basic = w_rt->u.basic = int_;//????????????int
    rt->u.func.rType = r_rt;
    wt->u.func.rType = w_rt;
    rt->u.func.args = NULL;
    FieldList fl = (FieldList)malloc(sizeof(FieldList_));
    char n[10] = "a";//write???????????????
    Type nt = (Type)malloc(sizeof(Type_));
    nt->kind = BASIC;
    nt->u.basic = int_;
    fl->name = n;
    fl->type = nt;
    fl->tail = NULL;
    wt->u.func.args = fl;
    Var read = (Var)malloc(sizeof(Var_));
    Var write = (Var)malloc(sizeof(Var_));
    read->name = r;
    write->name = w;
    read->type = rt;
    write->type = wt;
    assert(hash_insert(read));
    assert(hash_insert(write));
    Program(root);

    printUNfunc();
    printHashTable();
    //?????????????????????????????????
}
// Program ??? ExtDefList
void Program(pNode root){
#ifdef DEBUG
    printf("debug: enter Program\n");
#endif    
    if(!root)
        return;
    ExtDefList(root->child[0]);
}
// ExtDefList ??? ExtDef ExtDefList
// | null
void ExtDefList(pNode root){
#ifdef DEBUG
    printf("debug: enter ExtDefList\n");
#endif 
    if(!root){
        return;
    }
    ExtDef(root->child[0]);
    ExtDefList(root->child[1]);   
}
// ExtDef ??? Specifier ExtDecList SEMI
// | Specifier SEMI
// | Specifier FunDec CompSt
// | Specifier FunDec SEMI
void ExtDef(pNode root){
#ifdef DEBUG
    printf("debug: enter ExtDef\n");
#endif 

    if(!root)
        return ;
    //get type from specifier
    Type type = Specifier(root->child[0]);

    #ifdef DEBUG
        printf("debug: leave Specifier\n");
        if(!type){
            printf("   ERROR!!!\n");
            assert(0);
        }
        else{
            printf("  INFO: %d\n",type->kind);
        }
    #endif 
    // printf("\n\n");
    // assert(root->child[1]->token!=NULL);
    // assert(root->child[2]->token);
    #ifdef DEBUG
        printf("                debug: before if-else\n");
    #endif     
    if(root->childNum <= 2){
        #ifdef DEBUG
            printf("  debug: match Specifier SEMI\n");
        #endif 
    }
    else if(!strcmp(root->child[1]->token,"ExtDecList")){
        #ifdef DEBUG
            printf("  debug: match Specifier ExtDecList SEMI\n");
        #endif 
        // if(isEmpty()){
        //     #ifdef DEBUG_f
        //     printf("set newScope\n");
        //     #endif
        //     newScope = true;
        // }
        // else{
        //     #ifdef DEBUG_f
        //     printf(" not set newScope\n");
        //     #endif            
        // }
        ExtDecList(root->child[1], type);
    }
    else if(!strcmp(root->child[2]->token,"SEMI")){
        #ifdef DEBUG
            printf("  debug: match Specifier FunDec SEMI\n");
        #endif 
        //???????????????????????????
        Var v = FunDec(root->child[1], false);//????????????id???
        hash_del();//????????????
        // printf("out fundec\n");
        if(!v){
            #ifdef DEBUG_func
            printf("func no declar\n");
            #endif
            return;
        }
        v->type->u.func.rType = type;
        if(!findFunc(v->name)){
            // printf("line: %d doesn't find fun : %s, insert to the list\n",root->lineno,v->name);
            Var find_func = hash_find(v->name);
            if(find_func){
                //??????????????????????????????
                if(iSameType(v->type,find_func->type)){
                    #ifdef DEBUG_func
                    printf(" func %s declar is after def\n",v->name);
                    #endif
                }
                else {
                    semError(funDefCom,v->lineno,"Inconsistent declaration of function");
                }
            }
            else 
                insertFunc(v);
        }
        else {//????????????????????????????????????copy??????
            semError(funDefCom,v->lineno,"Inconsistent declaration of function");
            return;
        }
        // if(!newScope){
        //     //????????????????????????
        //     #ifdef DEBUG
        //     printf("  line: %d hash del of func\n",root->lineno);
        //     #endif
        //     assert(hash_del());
        // }
        #ifdef DEBUG
        printf("info:return type1:%d\n",type->kind);
        #endif        
    }
    else if(!strcmp(root->child[1]->token,"FunDec")){
        #ifdef DEBUG
            printf("  debug: match Specifier FunDec CompSt\n");
        #endif 
            //????????????????????????
        Var v = FunDec(root->child[1], true);//????????????id??????
        //???????????????????????????}??????del

        // printf("out fundec\n");
        // if(!v)
        //     return;
        // else 
        //     printf("name: %s\n", v->name);
        v->type->u.func.rType = type;
        Var annonce = findFunc(v->name);
        // printf("out findFunc\n");
        if(annonce){
            #ifdef DEBUG
            printf(" in extDef func def\n");
            #endif   
            if(!iSameType(annonce->type,v->type)){
                semError(funDefCom,v->lineno,"Inconsistent declaration of function");
            }
            else {
                removeFunc(v->name);
            }
        }
        // printf("another\n");
        // if(!iSameType(type,v->type->u.func.rType)){
        //     printf(" type: %d %d\n",type->kind, v->type->u.func.rType->kind);
        //     semError(funDefCom,v->lineno,"Inconsistent declaration of function");
        //     return;
        // }
        
        #ifdef DEBUG
        printf("    info:return type1:%d\n",type->kind);
        #endif
        // Type rtype = CompSt(root->child[2]);//?????????????????????
        CompSt(root->child[2],type);
        // printf("out compst\n");
        v->type->u.func.rType = type;
        //?????????????????????
        // if(findInScope(v->name)){
        //     #ifdef DEBUG
        //     printf("  info: find %s redef\n",v->name);
        //     #endif
        //     semError(funReDef,root->lineno,"Function Redefinition");
        // }
        // // if(iSameType(type,rtype)){
        //     //??????FunDec???id??????
        // else {
            // #ifdef DEBUG
            // printf("info:insert %s into the hash table\n",v->name);
            // #endif
            // bool succ = hash_insert(v);//???????????????????????????????????????????????????????????????
            // //???????????????
            // if(!succ)
            //     semError(funReDef,root->lineno,"Function Redefinition ");    
        // }   

        // }
        // else {
            // semError(returnNotMatch,root->lineno,"Funtion return type not match");
        // }
    }
    else{
        //???????????????????????????  ?????????????????????int ;??????????????????
        //?????????????????????assert???

    }
    #ifdef DEBUG
    printf("debug: leave ExtDef\n");
    #endif 
}
// ExtDecList ??? VarDec
// | VarDec COMMA ExtDecList
void ExtDecList(pNode root, const Type type){//finish
#ifdef DEBUG
    printf("debug: enter ExtDecList\n");
#endif
    //?????????VarDec???????????????type

    Var v = VarDec(root->child[0],type);
    char* name = v->name;
    #ifdef DEBUG_global
    printf("type->kind: %d name: %s\n",v->type->kind,name);
    #endif
    if(findInScope(name)){
        //error
        semError(varReDef,root->lineno,"Variable Redefition");
    }
    else {
        assert(hash_insert(v));//????????????
    }
    if(root->childNum==3)
        ExtDecList(root->child[2],type);
}
// Specifier ??? TYPE
// | StructSpecifier
Type Specifier(pNode root){//finish
#ifdef DEBUG
    printf("debug: enter Specifier\n");
#endif
    if(!strcmp(root->child[0]->token,"TYPE")){
        Type t = (Type)malloc(sizeof(Type_));
        t->val_kind=rval;
        t->kind = BASIC;
        if(!strcmp(root->child[0]->val,"int")){
            t->u.basic=int_;
        }
        else {
            #ifdef DEBUG
            printf("debug: root->child[0]->val: %s\n",root->child[0]->val);
            #endif
            t->u.basic=float_;
        }
        #ifdef DEBUG
        printf("get type : %d\n",t->kind);
        #endif
        return t;
    }
    else{
        Type t = StructSpecifier(root->child[0]);

        
        #ifdef DEBUG
        printf("get type structure\n");
        #endif
        return t;
    }    
}
// StructSpecifier ??? STRUCT OptTag LC DefList RC
// | STRUCT Tag
Type StructSpecifier(pNode root){//finish
#ifdef DEBUG
    printf("debug: enter StructSpecifier\n");
#endif
    // printHashTable();
    if(root->childNum==2){
        char* name = Tag(root->child[1]);
        //look up table
        // printHashTable();
        Var v = hash_find(name);
        if(!v){
            #ifdef DEBUG
            printf("    Info: not find %s in the hashTable\n",name);
            #endif
            semError(struUk, root->lineno, "Use unknown structure");
            return NULL;
        }
        return v->type;
    }
    else {//def
        char* name = OptTag(root->child[1]);
        #ifdef DEBUG
        printf("    debug: set new scope\n");
        #endif  
        if(name){
            Var v = hash_find(name);
            if(v){
                // printf("name: %s\n",name);
                semError(struReDef,root->child[1]->lineno,"Structure Redefinition");
                return NULL;
            }
        }
        
        #ifdef DEBUG
        if(name)
            printf("  opTag: %s ",name);
        else
            printf("  opTag = NULL ");
        printf("  set newScope True\n");
        #endif
        //????????????????????????DefList????????????????????????????????????
        //???????????????????????????????????????
        //??????????????????????????????????????????????????????????????????????????????
        if(name){
            //????????????????????????
            if(findInScope(name)){
                semError(struReDef,root->lineno,"Structure Redefinition");
                #ifdef DEBUG
                printf("   info: struReDef error, set newScope = false\n");
                #endif
                // newScope = false;
                return NULL;
            }
        }
        Var v = (Var)malloc(sizeof(Var_));
        v->name = name;
        v->isVar = false;
        v->type = (Type)malloc(sizeof(Type_));
        v->type->kind = STRUCTURE;
        v->type->u.func.args = NULL;
        // ???????????????  ????????????????????????????????????????????????????????????????????????????????????????????????????????????
        if(name){
            if(findInScope(v->name)){
                assert(0);
            }
            else{
                // if(isEmpty())
                //     newScope = true;
                bool succ = hash_insert(v);//???????????????
                
                assert(succ);
            }
        }
        // newScope = true;
        setNewScope();
        DefList(root->child[3], true,v);//????????????????????????????????????
        hash_del();
        if(newScope){
            #ifdef DEBUG 
            printf("  info: deflist of struct %s  is empty, set newscope false\n",v->name);
            #endif
            // newScope = false;
        }
        else{
            #ifdef DEBUG
            printf("  lined: %d hash del after sturct def\n",root->lineno);
            #endif
            // hash_del();
        }
        //???????????????
        // if(name){
        //     if(findInScope(v->name)){
        //         assert(0);
        //     }
        //     else{
        //         if(isEmpty())
        //             newScope = true;
        //         bool succ = hash_insert(v);
        //         assert(succ);
        //     }
        // }
        #ifdef DEBUG_struct
        printf(" helpful info: \n");
        if(name)
        printf("   struct name: %s\n", v->name);
        #endif
        FieldList fl = v->type->u.structure;
        while(fl){
            #ifdef DEBUG_struct
            printf("    var name: %s\n",fl->name);
            #endif
            fl = fl->tail;
        }

        return v->type; 
    }
}
// OptTag ??? ID
// | null
char* OptTag(pNode root){//finish
#ifdef DEBUG
    printf("debug: enter OptTag\n");
#endif
    if(root)
        return root->child[0]->val;
    else
        return NULL;
}
// Tag ??? ID
char* Tag(pNode root){//finsh
#ifdef DEBUG
    printf("debug: enter Tag\n");
#endif
    if(!root)
        assert(0);
    else
        return root->child[0]->val;
}
// VarDec ??? ID
// | VarDec LB INT RB
Var VarDec(pNode root,Type type){//finish
#ifdef DEBUG
    printf("debug: enter VarDec\n");
#endif
    //???????????????????????????
    //?????????????????????????????????type???
    //??????????????????????????????????????????
    if(root->childNum==1){
        // Type t = (Type)malloc(sizeof(Type_));
        char* name = root->child[0]->val;
        if(findInScope(name)){//????????????

        //?????????????????????????????????????????????
        
            if(type->kind==STRUCTURE){
                semError(struReDef,root->lineno,"Variable Redefinition");
                return NULL;
            }
        }
        Var v = (Var)malloc(sizeof(Var_));
        v->name = name;
        v->isVar = true;
        //??????type?????????????????????????????????free??????????????????????????????????????????
        v->type = type;
        return v;
    }
    else {//????????????????????????????????????
        Var v = VarDec(root->child[0], type);
        v->isVar = true;
        int x = atoi(root->child[2]->val);
        Type t = (Type)malloc(sizeof(Type_));
        t->kind = ARRAY;
        t->u.array.elem = v->type;
        t->u.array.size = x;
        //????????????type???
        v->type = t;
        return v;
    }
}
// FunDec ??? ID LP VarList RP
// | ID LP RP
Var FunDec(pNode root, bool def){//???????????????????????????
#ifdef DEBUG
    printf("debug: enter FunDec\n");
#endif
    //type????????????????????????????????????????????????????????????????????????
    //?????????????????????ID????????????????????????????????????????????????
    //??????????????????????????????????????????
    Var v = (Var)malloc(sizeof(Var_));
    v->lineno = root->lineno;
    v->isVar  = false;
    v->type = (Type)malloc(sizeof(Type_));
    v->name = root->child[0]->val;
    v->type->kind = FUNCTION;
    // printf("name: %s\n",v->name);
    v->type->u.func.rType = NULL;
    v->type->u.func.nArgs=0;
    v->type->u.func.args=NULL;
    //debug:
    // if(!def){
    //     //?????????????????????
    //     return v;
    // }
    if(!v->name){
        assert(0);
    }
    else {
        #ifdef DEBUG
        printf("find name:%s\n",v->name);
        #endif
    }
    if(findInScope(v->name)){
        semError(funReDef,root->lineno," Function Redefinition");
        
        // return v;//NULL
    }
    if(def&&!findInScope(v->name)){
        #ifdef DEBUG_hash
        printf("    info: insert %s\n",v->name);
        #endif
        assert(hash_insert(v));//????????????????????????
    }
    // newScope = true;
    setNewScope();//?????????????????????????????????
    //????????????????????????????????????????????????CompSt??????????????????????????????????????????
    if(root->childNum==4){
        #ifdef DEBUG
        printf("function has args!\n");
        #endif
        // newScope = true;
        VarList(root->child[2],v);
        
    }

    // printf("end\n");
    return v;
} 
// VarList ??? ParamDec COMMA VarList
// | ParamDec
void VarList(pNode root, const Var  fun){//finish
#ifdef DEBUG
    printf("debug: enter VarList\n");
#endif
    //??????????????????????????????????????????????????? ????????????????????????????????????????????????????????????
    // if(childNum==1){
    //     type->u.func.nArgs++;
    //     FieldList fl = (FieldList)malloc(sizeof(FieldList_));
    //     Var v = ParamDec(root->child[0]);
    //     fl->type = v->type;
    //     fl->name = v->name;
    //     fl->tail = fun->type->u.func.args;
    //     fun->type.u.func.args = fl;
    // }
    // else {
    //     VarList(root->child[2],fun);
    // }
    if(root->childNum==3)
        VarList(root->child[2],fun);
    fun->type->u.func.nArgs++;
    FieldList fl = (FieldList)malloc(sizeof(FieldList_));
    Var v = ParamDec(root->child[0]);
    
    //???????????????????????????
    //?????????????????????????????????????????????
    //
    assert(!findInScope(v->name));
    assert(hash_insert(v));
    
    fl->type = v->type;
    fl->name = v->name;
    fl->tail = fun->type->u.func.args;
    fun->type->u.func.args = fl;
}
// ParamDec ??? Specifier VarDec
Var ParamDec(pNode root){//finish
#ifdef DEBUG
    printf("debug: enter PramDec\n");  
#endif
    Type type = Specifier(root->child[0]);
    Var v = VarDec(root->child[1], type);
    //
    return v;
}
// CompSt ??? LC DefList StmtList RC
void CompSt(pNode root, const Type rType){
#ifdef DEBUG
    printf("debug: enter CompSt\n");
#endif
    //printHashTable();
    //???????????????????????????????????????
    #ifdef DEBUG
        printf("    debug: set new scope000\n");
    #endif  
    // newScope  = true;//??????LC????????????????????????
    if(!root || root->childNum == 0){
        return;
    }
    // printf("5555555555\n");
    // printf(" childNUm: %d\n",root->childNum);
    // assert(root->child[1]);
    // printf("Lineno: %d\n",root->child[1]->lineno);
    DefList(root->child[1],false,NULL);
    // ????????????????????????
    // printf("111\n");
    StmtList(root->child[2], rType);
    //????????????????????????????????????????????????????????????????????????????????????
    // printf("222\n");
    hash_del();
    // if(newScope){
    //     printf("  out stmtlist not insert value, no hash del, set scope false\n");
    //     // newScope = false;
    // }
    // else{
    //     #ifdef DEBUG
    //     printf("  line: %d hash del\n",root->lineno);
    //     #endif
    //     assert(hash_del());
    // }
    // printf("000\n");
    // return t;
}
// StmtList ??? Stmt StmtList
// | null
void StmtList(pNode root, const Type rType){//??????????????????
#ifdef DEBUG
    printf("debug: enter StmtList\n");
#endif
    if(!root)
        return ;
    Stmt(root->child[0], rType);
    // printf("9999\n");
    StmtList(root->child[1], rType);
    // printf("10010101\n");
    // if(t1&&!t2)
    //     return t1;
    // else if(!t1&&t2)
    //     return t2;
    // else{
    //     // semError(returnNotMatch,root->lineno,"Function has no return");
    //     // return NULL;
    // }
}
// Stmt ??? Exp SEMI 
// | CompSt
// | RETURN Exp SEMI
// | IF LP Exp RP Stmt +
// | IF LP Exp RP Stmt ELSE Stmt
// | WHILE LP Exp RP Stmt   +
void Stmt(pNode root, const Type rType){
#ifdef DEBUG
    printf("debug: enter Stmt\n");
#endif
    //printHashTable();
    if(root->childNum > 3){
        // printf("333333333333\n");
        if(!strcmp(root->child[0]->token,"IF")||!strcmp(root->child[0]->token,"WHILE")){//if & while
            Type t1 = Exp(root->child[2]);
            // printf("333\n");
            if(!t1)
                return ;
            if(t1->kind!=BASIC||t1->kind==BASIC&&t1->u.basic!=int_){
                semError(opNotMatch,root->lineno,"if/while exp statement needs int");
                return;
            }
            
            if(!strcmp(root->child[4]->token,"Stmt")){
                // printf("5555\n");
                Stmt(root->child[4], rType);
            }
            // else if(!strcmp(root->child[5]->token,"ELSE")){
            if(root->childNum == 7){
                #ifdef DEBUG
                    printf("debug: enter ELSE\n");
                #endif
                // printf("4444\n");
                Stmt(root->child[6], rType);
            }
        }
        else{
            #ifdef DEBUG
            printf("  node name %s\n",root->child[0]->token);
            #endif            
            assert(0);
        }
    }
    if(root->childNum==3){//RETURN Exp SEMI
        #ifdef DEBUG
        printf(" in stmt return before run into chlid[1]\n");
        #endif   
        // return Exp(root->child[1]);

        Type t = Exp(root->child[1]);
        #ifdef DEBUG
        printf(" in stmt return\n");
        if(t)
            printf("%d\n",t->kind);
        printf("2\n");
        #endif   
        if(!iSameType(t,rType)){
            semError(returnNotMatch,root->lineno,"Funtion return type not match");
        }
    }
    else if(root->childNum==2){
        Exp(root->child[0]);
    }
    else{//CompSt
        // newScope = true;
        setNewScope();
        // printf("666\n");
        CompSt(root->child[0], rType);
        // printf("77777\n");
    }
}

// DefList ??? Def DefList
// | null
void DefList(pNode root, bool inStruct, const Var  struName){
#ifdef DEBUG
    printf("debug: enter DefList\n");
#endif
    //intruct==true?????????????????????????????????????????????????????????????????????
    //?????????????????????
    if(root){
        Def(root->child[0],inStruct,struName);
        DefList(root->child[1],inStruct,struName);
    }
    else {
        #ifdef DEBUG
        printf("4444313131\n");
        #endif
    }
}
// Def ??? Specifier DecList SEMI
void Def(pNode root, bool inStruct, const Var  struName){
#ifdef DEBUG
    printf("debug: enter Def\n");
    printf("info: whether in the struct: %d\n",inStruct);
#endif
    //????????????
    Type t = Specifier(root->child[0]);
    if(!t)//get error
        return;
    // #ifdef DEBUG
    DecList(root->child[1],t,inStruct,struName);
}
// DecList ??? Dec
// | Dec COMMA DecList
void DecList(pNode root, const Type type, bool inStruct, const Var  struName){
#ifdef DEBUG
    printf("debug: enter DecList\n");
    printf("info: whether in the struct: %d\n",inStruct);
#endif
    //?????????????????????????????????????????????????????????????????????????????????????????????????????????
    if(root->childNum==3){
        DecList(root->child[2],type,inStruct,struName);
        // Dec(root->child[0],type,inStruct,struName);
    }
    Dec(root->child[0],type,inStruct,struName); 
}
// Dec ??? VarDec
// | VarDec ASSIGNOP Exp
void Dec(pNode root, const Type type, bool inStruct, const Var  struName){
    //?????????????????????inStruct???true???struName???????????????
    //???????????????????????????????????????????????????????????????,???????????????????????????
#ifdef DEBUG
    printf("debug: enter Dec\n");
    printf("info: whether in the struct: %d\n",inStruct);
#endif
    Var v = VarDec(root->child[0],type);
    if(!v){
        #ifdef DEBUG
        printf(" info: get from VarDec is NULL\n");
        #endif
        return;
    }

    Var sameName = hash_find(v->name);//?????????????????????????????????
    if(sameName && !sameName->isVar){
        // if(sameName->type->kind==BASIC){
        //     semError
        // }
        semError(varReDef,root->lineno,"The variable name is conficted with the struct or func");

    }
    if(root->childNum==1){
        if(inStruct){//??????????????????
            bool find = findInScope(v->name);
            #ifdef DEBUG
            printf("  info: find in the struct: %s\n",v->name);
            #endif
            if(find){
                semError(struAreaReDef,root->lineno,"The variable already existed in the struct");
                return ;
            }
            assert(hash_insert(v));
            //???v?????????struName ?????????
            FieldList fl = (FieldList)malloc(sizeof(FieldList_));
            fl->name = v->name;
            fl->type = v->type;
            fl->tail = struName->type->u.structure;
            struName->type->u.structure = fl;
        }
        else{
            //???????????????
            bool find = findInScope(v->name);
            #ifdef DEBUG
            printf("  info: find %s\n",v->name);
            #endif
            if(find){
                semError(varReDef,root->lineno,"The variable already existed");
            }
            else {
                #ifdef DEBUG
                printf("debug: insert: name %s, type %d\n",v->name,v->type->kind);
                #endif
                assert(hash_insert(v));
            }
        }
    }
    else{
        if(inStruct){//????????????????????????
            semError(struAreaReDef,root->lineno,"initialize in the structure");
            return;
        }
        else{
            assert(struName==NULL);
            if(findInScope(v->name)){
                semError(varReDef,root->lineno,"The variable already existed");
                return;
            }
            Type t = Exp(root->child[2]);
            #ifdef DEBUG
            printf(" in dec = op\n");
            #endif   
            if(!iSameType(t,v->type)){//?????????????????????
                #ifdef DEBUG
                printf("  type arount = :t1 %d t2 %d\n",t->u.basic,v->type->u.basic);
                #endif
                semError(asNotMatch,root->lineno,"the value doesn't match around the assignop");
            }
            else {
                #ifdef DEBUG2
                printf("    info: insert symbol %s \n",v->name);
                #endif
                // assert(hash_insert(v));
                bool succ = (hash_insert(v));
                assert(succ);
            }
        }
        

    }
}
// Exp ??? Exp ASSIGNOP Exp =
// | Exp AND Exp  &&
// | Exp OR Exp   ||
// | Exp RELOP Exp  <|<=
// | Exp PLUS Exp   +
// | Exp MINUS Exp  -
// | Exp STAR Exp   *
// | Exp DIV Exp    /
// | LP Exp RP  (E)
// | MINUS Exp  -
// | NOT Exp   ~
// | ID LP Args RP
// | ID LP RP   fun()
// | Exp LB Exp RB    
// | Exp DOT ID  struct.x
// | ID   x   
// | INT   1
// | FLOAT  1.1
Type Exp(pNode root){
#ifdef DEBUG
    printf("debug: enter Exp\n");
#endif
    //exp?????????exp???????????????????????????????????????????????????????????????
    //??????exp?????????????????????????????????????????????????????????????????????
    //printHashTable();
    if(root->childNum==1){
        #ifdef DEBUG
        printf("enter childNum 1\n");
        #endif
        Type type = NULL;
        if(strcmp(root->child[0]->token,"INT")==0){//
        //?????????root->child[0]->type?????????
            type = (Type)malloc(sizeof(Type_));
            type->kind=BASIC;
            type->u.basic=int_;        
            type->val_kind = rval;
        }
        else if(strcmp(root->child[0]->token,"FLOAT")==0){
            type = (Type)malloc(sizeof(Type_));
            type->kind=BASIC;
            type->u.basic=float_;
            type->val_kind = rval;
        }
        else if(strcmp(root->child[0]->token,"ID")==0){
            #ifdef DEBUG
            printf(" info: name: %s\n",root->child[0]->val);
            #endif
            // type = look4type(root->child[0]->val);
            Var v = hash_find(root->child[0]->val);
             
            if(v){
                type = v->type;
                // printf("name: %s2\n",v->name);
            }
            // printf(" id: %s: type : %d\n",v->name,v->type->kind);
            // printHashTable();
            if(!v){
                //?????????????????????,
                semError(varUk,root->lineno,"Variable Undefined");
                return NULL;
            }
            type->val_kind = lval;//??????????????????????????????????????????
            
            // return type;
        }
        else{
            //??????????????????
            assert(0);
        }
        #ifdef DEBUG
        printf(" leave childNum 1\n");
        #endif        
        return type;
    }
    else if(root->childNum==2){
        #ifdef DEBUG
        printf("enter childNum 2\n");
        #endif
        //?????????????????????  not???minus????????????????????????????????????exp???
        if(!strcmp(root->child[0]->token,"NOT")){//???????????????int
            Type t = Exp(root->child[1]);
            if(!t)
                return NULL;
            if(t->kind!=BASIC || t->kind==BASIC&&t->u.basic!=int_){
                semError(opNotMatch,root->lineno,"Type mismatched for operator not, need int");
                return NULL;
            }
            else 
                return t;
        }
        else if(!strcmp(root->child[0]->token,"MINUS")){//-?????????int/float
            Type t = Exp(root->child[1]);
            if(!t)
                return NULL;
            if(t->kind!=BASIC){
                semError(opNotMatch,root->lineno,"Type mismatched for operator minus, need int/float");
                return NULL;
            }
            else 
                return t;
        }
        else
            assert(0);
        // return Exp(root->child[1]);
    }
    else if(root->childNum==3){
        #ifdef DEBUG
        printf("enter childNum 3\n");
        #endif
        /*        if(!strcmp(root->child[1],"ASSIGNOP")){
            Type t1 = Exp(root->child[0]);
            Type t2 = Exp(root->child[2]);
            if(t1->val_kind==rval){
                semError(asRval,root->lineno,"The left-hand side of an assignment must be a variable");
                return NULL;
            }
            if(!)
        }*/
        //printHashTable();
        if(!strcmp(root->child[1]->token,"ASSIGNOP")||!strcmp(root->child[1]->token,"AND")||
         !strcmp(root->child[1]->token,"OR")||!strcmp(root->child[1]->token,"RELOP")||
         !strcmp(root->child[1]->token,"PLUS")||!strcmp(root->child[1]->token,"MINUS")||
         !strcmp(root->child[1]->token,"STAR")||!strcmp(root->child[1]->token,"DIV")){
                //?????????????????????????????????????????????????????????else??????????????????????????????
            Type t1 = Exp(root->child[0]);
            Type t2 = Exp(root->child[2]);
            //printHashTable();
            // printf("t1 and t2 type: %d %d basic: %0x %d\n",t1->kind,t2->kind,t1->u.basic,t2->u.basic);
            //printHashTable();
            // if(t1->kind==FUNCTION){
            //     if(t1->u.func.rType)
            //         printf("    rtype: %d ",t1->u.func.rType->kind);
            //     // if(t1->u.func.nArgs
            //     printf("   nArgs: %d  ",t1->u.func.nArgs);
            //     printf("\n");
            // }
            if(!t1 || !t2){
                #ifdef  DEBUG
                printf("    info: one of the exp is NULL\n");
                #endif
                return NULL;
            }

            //??????t1???t2????????? ???assert??????Error
            if(!strcmp(root->child[1]->token,"ASSIGNOP") && t1->val_kind==rval){
                //semError:????????????
                semError(asRval,root->lineno,"The left-hand side of an assignment must be a variable");
                return NULL;
            }
            #ifdef DEBUG
            printf(" in exp \n");
            #endif   
            if(!iSameType(t1,t2)){
                // semError:???????????????
                if(!strcmp(root->child[1]->token,"ASSIGNOP")){
                    
                    semError(asNotMatch,root->lineno,"Type mismatched for assignment");
                }
                else
                    semError(opNotMatch,root->lineno,"Type mismatched for operator");
                return NULL;
            }
            if((!strcmp(root->child[1]->token,"PLUS")||!strcmp(root->child[1]->token,"MINUS")||
            !strcmp(root->child[1]->token,"STAR")||!strcmp(root->child[1]->token,"DIV")||
            !strcmp(root->child[1]->token,"RELOP"))){
                if(t1->kind!=BASIC){
                    //semError: ????????????????????????+-*/??????????????????????????????????????????
                    semError(opNotMatch,root->lineno,"Type mismatched for operator, need int/float");
                    return NULL;
                }
                else if(!strcmp(root->child[1]->token,"RELOP")){//????????????????????????0/1
                    Type tmp = (Type)malloc(sizeof(Type_));
                    tmp->kind=BASIC;
                    tmp->val_kind=rval;
                    tmp->u.basic=int_;
                    return tmp;
                }
                else
                    return t1;
                
            }
            else if((!strcmp(root->child[1]->token,"AND")||!strcmp(root->child[1]->token,"OR"))){
                if(t1->kind!=BASIC||t1->kind==BASIC&&t1->u.basic!=int_){
                    //semError:?????????????????????int
                    semError(opNotMatch,root->lineno,"Type mismatched for operator, need int");
                    return NULL;
                }
                else{
                    Type tmp = (Type)malloc(sizeof(Type_));
                    tmp->kind=BASIC;
                    tmp->val_kind=rval;
                    tmp->u.basic=int_;
                    return tmp;
                }
            }
            return t1;//??????????????????????????????????????????
            
        }
        else if(!strcmp(root->child[0]->token,"LP")){//LP Exp RP
            #ifdef DEBUG
            printf("    in LP Exp RP\n");
            #endif   
            Type t = Exp(root->child[1]);
            return t;
        }
        else if(!strcmp(root->child[0]->token,"ID")){//ID LP RP,func
            #ifdef DEBUG
            printf("   in ID LP RP\n");
            #endif   
            char* name = root->child[0]->val;
            Var v = hash_find(name);
            if(!v){
                semError(funUk,root->lineno,"Function doesn't exist");
                return NULL;
            }
            if(v->type->kind!=FUNCTION){
                semError(funCallErr,root->lineno,"Variable is not a function");
                return NULL;
            }
                
            if(v->type->u.func.nArgs!=0){
                semError(argsNotMatch,root->lineno,"Function needs arguments");
                return NULL;
            }
            else {
                Type t=(Type)malloc(sizeof(Type_));
                t->kind = v->type->u.func.rType->kind;
                t->val_kind=rval;
                t->u = v->type->u.func.rType->u;
                return t;
            } 
        }
        else if(!strcmp(root->child[1]->token,"DOT")){//
            #ifdef DEBUG
            printf("    in Exp Do id\n");
            #endif   
            Type t = Exp(root->child[0]);
            char* name = root->child[2]->val;
            #ifdef DEBUG_struct
            printf("    info: find struct field %s \n",name);
            #endif
            if(!t){
                #ifdef DEBUG_struct
                printf("     info: line %d id %s doesn't exist!\n",root->lineno,root->child[0]->val);
                #endif
                assert(0);
            }
            //????????????????????????????????????????????????????????????free????????????????????????????????????????????????
            //find name in t;
            if(t->kind!=STRUCTURE){
                semError(struVisErr,root->lineno,"visit not structure variable");
                return NULL;
            }  
            #ifdef DEBUG_struct
            printf(" find name: %s\n",name);
            printf("  first var name: %s\n",t->u.structure->name);
            #endif
            FieldList tmp_fl = t->u.structure;
            while(tmp_fl){
                #ifdef DEBUG_struct
                printf("  struct var name: %s\n",tmp_fl->name);
                #endif
                if(!strcmp(tmp_fl->name,name)){
                    #ifdef DEBUG
                    printf(" struct var name %s, kind: %d\n",tmp_fl->name, tmp_fl->type->kind);
                    #endif   
                    tmp_fl->type->val_kind = lval;
                    return tmp_fl->type;
                }
                tmp_fl=tmp_fl->tail;
            }
            
            semError(struUkArea,root->lineno,"visited variable not int structure");
            return NULL;
            
        }
        else
            assert(0);
    }
    else if(root->childNum==4){
        if(!strcmp(root->child[0]->token,"Exp")){
            #ifdef DEBUG
            printf("    in exp lb exp rb\n");
            #endif   
            Type t1 = Exp(root->child[0]);
            Type t2 = Exp(root->child[2]);
            #ifdef DEBUG
            printf("    get arra and idx\n");
            #endif 
            if(!t1 || !t2){
                return NULL;
                assert(0);  
            }
            if(t1->kind!=ARRAY){
                semError(arrVisErr,root->lineno,"visit not array variable");
                return NULL;
            }
            if(!(t2->kind==BASIC&&t2->u.basic==int_)){
                semError(arrVisFlt,root->lineno,"visit index is not int");
                return NULL;
            }
            // printf("1\n");
            t1->u.array.elem->val_kind = lval;
            return t1->u.array.elem;
        }
        else if(!strcmp(root->child[0]->token,"ID")){
            //????????????????????????????????????????????????????????????
            char* name = root->child[0]->val;
            #ifdef DEBUG
            printf("   in id lp args rp\n");
            
            printf("call func:%s %d\n",name,root->lineno);
            #endif   

            Var v = hash_find(name);
            if(!v){
                semError(funUk,root->lineno,"Undefined function");
                return NULL;
            }
            if(v->type->kind!=FUNCTION){
                semError(funCallErr,root->lineno,"call variable as a function");
                return NULL;
            }
                
            if(v->type->u.func.nArgs==0){
                semError(argsNotMatch,root->lineno,"Function doesn't need arguments");
                return NULL;
            }
            else {
                bool isMatch = Args(root->child[2],v->type->u.func.args);
                if(!isMatch){
                    semError(argsNotMatch,root->lineno,"Arguments are not matched for function");
                    // return NULL;
                }
                else {
                    #ifdef DEBUG
                    printf("call fun,match name: %s\n",v->name);
                    #endif
                }
                Type t=(Type)malloc(sizeof(Type_));//???????????????????????????????????????
                t->kind = v->type->u.func.rType->kind;
                t->val_kind=rval;
                t->u = v->type->u.func.rType->u;
                return t;
            } 

        }
        else{
            #ifdef DEBUG
            printf("info: root->child[0]->token : %s\n", root->child[0]->token);
            #endif
            assert(0);
        }
    }
}
// Args ??? Exp COMMA Args
// | Exp
//?????????????????????????????????????????????????????????
//???id??????????????????????????????????????????
//????????????fl?????????Type
//?????????var????????????fl???????????????????????????child???????????????????????????????????????????????????
//var???fl???????????????????????????
//??????????????????????????????????????????????????????
bool Args(pNode root, FieldList fl){
#ifdef DEBUG
    printf("debug: enter Args\n");
#endif    
    if(!root&&fl || root&&!fl){
        //???????????????????????????
        semError(argsNotMatch,root->lineno,"Function is not applicable for arguments");
        return false;
    }

    //print fl
    #ifdef DEBUG_args
    FieldList tmp_ = fl;
    while(tmp_){
        if(tmp_->name)
            printf("Name: %s ",tmp_->name);
        else 
            printf(" no name ");
        if(tmp_->type)
            printf("Type: %d \n",tmp_->type->kind);
        else    
            printf(" no type\n");
        tmp_ = tmp_->tail;
    }
    #endif

    if(root->childNum==1){
        //??????fl???child
        #ifdef DEBUG
        printf(" in args judges func arg type\n");
        #endif   
        bool equal = iSameType(fl->type,Exp(root->child[0]));
        if(!equal || fl->tail){
            //not match
            return false;
        }
        else
            return true;
    }
    else if (root->childNum==3){
        #ifdef DEBUG
        printf(" in args judges func args type\n");
        #endif   
        bool e1 = iSameType(fl->type,Exp(root->child[0]));
        bool e2 = Args(root->child[2], fl->tail);
        if(!e1||!e2){
            printf("false\n");
            return false;
        }
        else 
            return true;
    }
    else //childNum error
        assert(0);
    

}
Type look4type(char* name){
#ifdef DEBUG
    printf("debug: enter look4type\n");
#endif
    //????????????????????????
    //????????????????????????????????????????????????NULL
    //?????????????????????????????????????????????
    Var v = hash_find(name);
    // printf("%s\n",name);
    if(!v)  
        return NULL;
    else
        return v->type;
}

bool iSameType(const Type a, const Type b){//
#ifdef DEBUG
    printf("debug: enter iSameType\n");
#endif
    // if(!a)
    //     printf("1\n");
    // else if(!b){
    //     printf("2\n");
    // }
    // else{ 
    //     // printf("33\n");
    //     printf("%d \n",a->u.basic);
    // }
    // printf("4\n");
    if(!a&&b || !b&&a){
        // //printf("11\n");
        #ifdef DEBUG_type
        printf("  info: one of is null\n");
        #endif
        return false;
    }
    else if(!a && !b){
         //printf("11\n");
       #ifdef DEBUG_type
        printf("  info: all of are null\n");
        #endif        
        return true;
    }
    else if(a->kind != b->kind){
        //printf("11\n");
        #ifdef DEBUG_type
        printf("  kind: %d %d \n",a->kind,b->kind);
        #endif
        return false;
    }
    else if(a->kind == BASIC){
        //printf("11\n");
        #ifdef DEBUG_type
        printf("  BASIC kind:  %d %d \n", a->u.basic, b->u.basic);
        #endif        
        if(a->u.basic != b->u.basic)
            return false;
        else 
            return true;
    }
    else if(a->kind == ARRAY){
    //printf("11\n");
        #ifdef DEBUG_type
        printf("  ARRAY\n");
        #endif         
        return iSameType(a->u.array.elem,b->u.array.elem);
    }
    else if(a->kind == STRUCTURE){
    //printf("11\n");
        //??????????????????????????????????????????
        #ifdef DEBUG_type
        printf("  structure\n");
        #endif         
        FieldList fa = a->u.structure;
        FieldList fb = b->u.structure;
        if(fa && fb){
            //??????
            #ifdef DEBUG
                printf("    info: name: %s, %s\n",fa->name,fb->name);
            #endif
            if(!strcmp(fa->name, fb->name))
                return true;
        }
        #ifdef DEBUG_type
        printf("  STRUCTURE\n");
        // printf("STRUCTURE: %s, %s\n",a->u.structure->name, a->u.structure->name);
        if(!fa && !fb){
            printf("   all the ptrs are NULL\n");
        }
        else if (fa && fb){
            printf("   every ptr is not NULL\n");
        }
        else {
            printf("   every ptr is not NULL\n");
        }
        #endif          
        while(fa&&fb){
        #ifdef DEBUG
        printf(" in isametype\n");
        #endif   

            if(iSameType(fa->type,fb->type)){
                fa = fa->tail;
                fb = fb->tail;
            }
            else
                return false;
        }
        if(!fa&&!fb)
            return true;
        else 
            return false;
    }
    else if(a->kind == FUNCTION){
    //printf("11\n");
        #ifdef DEBUG_type
        printf("kind: FUNCTION \n");
        #endif
        if(a->u.func.nArgs != b->u.func.nArgs){
            #ifdef DEBUG_type
            printf("  num of args is not equal: %d, %d\n",a->u.func.nArgs,b->u.func.nArgs);
            #endif            
            return false;
        }
        else{
            #ifdef DEBUG_type
            printf(" in isametype\n");
            #endif   
            if(iSameType(a->u.func.rType,b->u.func.rType)){
                //args
                FieldList fa = a->u.func.args;
                FieldList fb = b->u.func.args;
                while(fa&&fb){
                    #ifdef DEBUG_type
                    printf(" in isametype\n");
                    #endif   
                    if(iSameType(fa->type,fb->type)){
                        fa = fa->tail;
                        fb = fb->tail;
                    }
                    else
                        return false;
                }
                if(!fa&&!fb)
                    return true;
                else 
                    return false;
            }
            else 
                return false;
        }
    }
    else 
        assert(0);
}




// int main()
// {
//     initStack();
//     initHashTalbe();
//     printf("1:%d\n",isEmpty());
//     pStackNode psn = (pStackNode)malloc(sizeof(StackNode));
//     pTableNode ptn = (pTableNode)malloc(sizeof(TableNode));
//     FieldList fl = (FieldList)malloc(sizeof(FieldList_));
//     char* name = (char*)malloc(sizeof(char)*10);
//     Type type = (Type)malloc(sizeof(Type_));
//     strcpy(name, "van");
//     fl->type = type;
//     fl->name = name;
//     fl->tail = NULL;
//     ptn->node = fl;
//     ptn->next = NULL;
//     psn->node = ptn;
//     psn->next = NULL;
//     // push(psn);
//     newScope = true;
//     hash_insert(fl);
//     // printf("pt1 address:%lld\n",ptn);
//     // printf("ps1 address:%lld\n",psn);

//     pStackNode psn2 = (pStackNode)malloc(sizeof(StackNode));
//     pTableNode ptn2 = (pTableNode)malloc(sizeof(TableNode));
//     FieldList f2 = (FieldList)malloc(sizeof(FieldList_));
//     char* n2 = (char*)malloc(sizeof(char)*10);
//     Type t2 = (Type)malloc(sizeof(Type_));
//     strcpy(n2, "v2");
//     f2->type = t2;
//     f2->name = n2;
//     f2->tail = NULL;
//     ptn2->node = f2;
//     ptn2->next = NULL;
//     psn2->node = ptn2;
//     psn2->next = NULL;

//     FieldList fl2 = (FieldList)malloc(sizeof(FieldList_));
//     char* name2 = (char*)malloc(sizeof(char)*10);
//     Type type2 = (Type)malloc(sizeof(Type_));
//     strcpy(name2, "van2");
//     fl2->type = type2;
//     fl2->name = name2;
//     fl2->tail = NULL;

//     FieldList fl3 = (FieldList)malloc(sizeof(FieldList_));
//     char* name3 = (char*)malloc(sizeof(char)*10);
//     Type type3 = (Type)malloc(sizeof(Type_));
//     strcpy(name3, "van3");
//     fl3->type = type3;
//     fl3->name = name3;
//     fl3->tail = NULL;    
//     // printf("pt2 address:%lld\n",ptn2);
//     // printf("ps2 address:%lld\n",psn2);

//     pTableNode pt = psn2->node;
//     if(pt){
//         // printf("pt address:%lld\n",pt);
//         if(pt->next){
//         // printf("pt->next address:%lld\n",pt->next);
//         }
//         else
//             printf("null next\n");
//     }
//     else 
//         printf("null\n");




//     printf("stack top:%s\n",top()->node->node->name);

//     printf("2:stack empty:%d\n",isEmpty());
//     bool x = hash_insert(fl2);
//     // push(psn2);
//     newScope = true;
//     hash_insert(f2);
//     bool z = hash_insert(fl3);

//     pTableNode find = hash_find(name2);
//     printf("3:insert %s %d, insert %s %d\n",fl2->name, x,fl3->name,z);
//     if(find)
//         printf("4:find %s\n",name2);
//     else 
//         printf("4:not find %s!\n",name2);

//     printf("hash_del:\n\n");
//     bool y = hash_del();
//     printf("5:stack is empty:%d\n",isEmpty());
//     find = hash_find(name);
//     printf("6:del:%d\n",y);
//     if(!find)
//         printf("7:not find\n");
//     else 
//         printf("%s\n",find->node->name);
//     // printf("5:%s\n",fl->name);
//     // printf
// }