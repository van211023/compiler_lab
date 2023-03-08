//
// Created by hby on 22-12-4.
//

#include <copy_propagation.h>


void Fact_def_use_init(Fact_def_use *fact, bool is_top) {
    fact->is_top = is_top;
    Map_IR_var_IR_var_init(&fact->def_to_use);
    Map_IR_var_IR_var_init(&fact->use_to_def);
}

void Fact_def_use_teardown(Fact_def_use *fact) {
    Map_IR_var_IR_var_teardown(&fact->def_to_use);
    Map_IR_var_IR_var_teardown(&fact->use_to_def);
}

//// ============================ Dataflow Analysis ============================

static void CopyPropagation_teardown(CopyPropagation *t) {
    for_map(IR_block_ptr, Fact_def_use_ptr, i, t->mapInFact)
        RDELETE(Fact_def_use, i->val);
    for_map(IR_block_ptr, Fact_def_use_ptr, i, t->mapOutFact)
        RDELETE(Fact_def_use, i->val);
    Map_IR_block_ptr_Fact_def_use_ptr_teardown(&t->mapInFact);
    Map_IR_block_ptr_Fact_def_use_ptr_teardown(&t->mapOutFact);
}

static bool
CopyPropagation_isForward (CopyPropagation *t) {
    // TODO: isForward?
    // TODO();
    return true;
}

static Fact_def_use*
CopyPropagation_newBoundaryFact (CopyPropagation *t, IR_function *func) {
    // TODO: return NEW(Fact_def_use, is_top?);
    // TODO();
    return NEW(Fact_def_use, false);//像可用表达式分析，
}

static Fact_def_use*
CopyPropagation_newInitialFact (CopyPropagation *t) {
    // TODO: return NEW(Fact_def_use, is_top?);
    // TODO();
    return NEW(Fact_def_use, true);//
}

static void
CopyPropagation_setInFact (CopyPropagation *t,
                                        IR_block *blk,
                                        Fact_def_use *fact) {
    VCALL(t->mapInFact, set, blk, fact);
}

static void
CopyPropagation_setOutFact (CopyPropagation *t,
                                         IR_block *blk,
                                         Fact_def_use *fact) {
    VCALL(t->mapOutFact, set, blk, fact);
}

static Fact_def_use*
CopyPropagation_getInFact (CopyPropagation *t, IR_block *blk) {
    return VCALL(t->mapInFact, get, blk);
}

static Fact_def_use*
CopyPropagation_getOutFact (CopyPropagation *t, IR_block *blk) {
    return VCALL(t->mapOutFact, get, blk);
}

static bool
CopyPropagation_meetInto (CopyPropagation *t,
                          Fact_def_use *fact,
                          Fact_def_use *target) {
    if(fact->is_top) return false;  //第一个参数为全集，没有更新；和全集交，得到本身，所以不处理
    if(target->is_top) { //第二个参数为全集，交了后得到的是fact
        target->is_top = false;  
        for_map(IR_var, IR_var, it, fact->def_to_use) {  //遍历它的每个def_to_use
            VCALL(target->def_to_use, insert, it->key, it->val);//key是def
            VCALL(target->use_to_def, insert, it->val, it->key);//key是use
        }
        return true; //有更新
    }
    // Map intersect
    bool updated = false; //假设没有更新
    Map_IR_var_IR_var not_exist;
    Map_IR_var_IR_var_init(&not_exist);
    for_map(IR_var, IR_var, it, target->def_to_use) //遍历target的每个def_to_use
        if(!VCALL(fact->def_to_use, exist, it->key) || VCALL(fact->def_to_use, get, it->key) != it->val) {
            //fact的def_to_use中没有该键值，或者有但是对应的值不相等，
            VCALL(not_exist, insert, it->key, it->val); //这个map就不应该存在结果中
            updated = true;
        }
    for_map(IR_var, IR_var, it, not_exist) { //对于不应该存在的
        VCALL(target->def_to_use, delete, it->key); //删除
        VCALL(target->use_to_def, delete, it->val);
    }
    Map_IR_var_IR_var_teardown(&not_exist); //删除这个临时的map
    return updated;
}

void CopyPropagation_transferStmt (CopyPropagation *t,
                                   IR_stmt *stmt,
                                   Fact_def_use *fact) {
    IR_var new_def = VCALL(*stmt, get_def); //本语句定义的
    //// copy_kill
    if(new_def != IR_VAR_NONE) {  //def
        if(VCALL(fact->def_to_use, exist, new_def)) {  //def_to_use里面有new_def
            IR_var use = VCALL(fact->def_to_use, get, new_def);
            /* TODO:
             * use is killed by new_def
             * VCALL(fact->def_to_use/use_to_def?, delete, use/new_def?); 盲猜def_to_use
             */ 
            // TODO();
            VCALL(fact->use_to_def, delete, use);
            VCALL(fact->def_to_use, delete, new_def);
        }
        if(VCALL(fact->use_to_def, exist, new_def)) { //为什么use_to_def里面会有def啊
            IR_var def = VCALL(fact->use_to_def, get, new_def);
            /* TODO:
             * def is killed by new_def
             * VCALL(fact->def_to_use/use_to_def?, delete, def/new_def?);  盲猜use_to_def
             */ 
            // TODO();
            VCALL(fact->def_to_use, delete, def); //也不知道这个写的对不对
            VCALL(fact->use_to_def, delete, new_def); 
        }
    }
    //// copy_gen
    if(stmt->stmt_type == IR_ASSIGN_STMT) {
        IR_assign_stmt *assign_stmt = (IR_assign_stmt*)stmt;
        if(!assign_stmt->rs.is_const) {
            IR_var def = assign_stmt->rd, use = assign_stmt->rs.var;
            /* TODO:
             * def is killed by new_def
             * VCALL(fact->def_to_use/use_to_def?, set, def/use?);
             */ 
            // TODO();
            VCALL(fact->def_to_use, set, def, use);
            VCALL(fact->use_to_def, set, use, def);
        }
    }
}

bool CopyPropagation_transferBlock (CopyPropagation *t,
                                                 IR_block *block,
                                                 Fact_def_use *in_fact,
                                                 Fact_def_use *out_fact) {
    Fact_def_use *new_out_fact = CopyPropagation_newInitialFact(t); //为该blk新建一个out
    CopyPropagation_meetInto(t, in_fact, new_out_fact);  //用in更新它
    for_list(IR_stmt_ptr, i, block->stmts) {  //遍历该blk中的每一个语句
        IR_stmt *stmt = i->val;
        CopyPropagation_transferStmt(t, stmt, new_out_fact); //传递函数
    }
    bool updated = CopyPropagation_meetInto(t, new_out_fact, out_fact);//控制流方程
    RDELETE(Fact_def_use, new_out_fact);
    return updated;
}

void CopyPropagation_print_result (CopyPropagation *t, IR_function *func) {
    printf("Function %s: Copy Propagation Result\n", func->func_name);
    for_list(IR_block_ptr, i, func->blocks) {
        IR_block *blk = i->val;
        printf("=================\n");
        printf("{Block%s %p}\n", blk == func->entry ? "(Entry)" :
                                 blk == func->exit ? "(Exit)" : "",
               blk);
        IR_block_print(blk, stdout);
        Fact_def_use *in_fact = VCALL(*t, getInFact, blk),
                *out_fact = VCALL(*t, getOutFact, blk);
        printf("[In(top:%d)]:  ", in_fact->is_top);
        for_map(IR_var, IR_var, j, in_fact->def_to_use)
            printf("{v%u := v%u} ", j->key, j->val);
        printf("\n");
        printf("[Out(top:%d)]: ", out_fact->is_top);
        for_map(IR_var, IR_var, j, out_fact->def_to_use)
            printf("{v%u := v%u} ", j->key, j->val);
        printf("\n");
        printf("=================\n");
    }
}

void CopyPropagation_init(CopyPropagation *t) {//复制传播后面是死代码消除
    const static struct CopyPropagation_virtualTable vTable = {
            .teardown        = CopyPropagation_teardown,
            .isForward       = CopyPropagation_isForward,
            .newBoundaryFact = CopyPropagation_newBoundaryFact,
            .newInitialFact  = CopyPropagation_newInitialFact,
            .setInFact       = CopyPropagation_setInFact,
            .setOutFact      = CopyPropagation_setOutFact,
            .getInFact       = CopyPropagation_getInFact,
            .getOutFact      = CopyPropagation_getOutFact,
            .meetInto        = CopyPropagation_meetInto,
            .transferBlock   = CopyPropagation_transferBlock,
            .printResult     = CopyPropagation_print_result
    };
    t->vTable = &vTable;
    Map_IR_block_ptr_Fact_def_use_ptr_init(&t->mapInFact);
    Map_IR_block_ptr_Fact_def_use_ptr_init(&t->mapOutFact);
}

//// ============================ Optimize ============================

// 将所有use变为copy的def变量
static void block_replace_available_use_copy (CopyPropagation *t, IR_block *blk) {
    Fact_def_use *blk_in_fact = VCALL(*t, getInFact, blk);  //获得in
    Fact_def_use *new_in_fact = CopyPropagation_newInitialFact(t); //新建一个in
    CopyPropagation_meetInto(t, blk_in_fact, new_in_fact);  //把旧的整到新的上
    for_list(IR_stmt_ptr, i, blk->stmts) {  //遍历每个语句块
        IR_stmt *stmt = i->val;
        IR_use use = VCALL(*stmt, get_use_vec); //该语句使用的use
        for(int j = 0; j < use.use_cnt; j++)
            if(!use.use_vec[j].is_const) { //不是常量
                IR_var use_var = use.use_vec[j].var; 
                if(VCALL(new_in_fact->def_to_use, exist, use_var))  //这个use可以作为键值被索引？？？
                    use.use_vec[j].var = VCALL(new_in_fact->def_to_use, get, use_var);  //把它的
            }
        CopyPropagation_transferStmt(t, stmt, new_in_fact);
    }
    RDELETE(Fact_def_use, new_in_fact);
}

void CopyPropagation_replace_available_use_copy (CopyPropagation *t, IR_function *func) {
    for_list(IR_block_ptr, j, func->blocks) { //遍历每个blk
        IR_block *blk = j->val;
        block_replace_available_use_copy(t, blk);
    }
}




