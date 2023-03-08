//
// Created by hby on 22-12-3.
//

#include <available_expressions_analysis.h>

static int IR_val_CMP(IR_val a, IR_val b) {
    if(a.is_const != b.is_const)//一个是常量而另一个不是
        return a.is_const < b.is_const;
    if(a.is_const)//a、b都是常量，
        return a.const_val == b.const_val ? -1 : a.const_val < b.const_val;
    else//都是变量
        return a.var == b.var ? -1 : a.var < b.var;//-1：同一个值， 0：大于，1：小于
}

int Expr_CMP(Expr a, Expr b) {
    if(a.op != b.op)//类型不同，返回0/1
        return a.op < b.op;
    int rs1_cmp = IR_val_CMP(a.rs1, b.rs1);//类型相同，比较操作数
    if(rs1_cmp != -1)//-1是相等
        return rs1_cmp;//不相等，返回0/1
    int rs2_cmp = IR_val_CMP(a.rs2, b.rs2);//第二个操作数
    return rs2_cmp;
}

void Fact_set_var_init(Fact_set_var *fact, bool is_top) {//
    fact->is_top = is_top;
    Set_IR_var_init(&fact->set);
}

void Fact_set_var_teardown(Fact_set_var *fact) {//析构
    Set_IR_var_teardown(&fact->set);
}

//// ============================ Dataflow Analysis ============================

static void AvailableExpressionsAnalysis_teardown(AvailableExpressionsAnalysis *t) {//可用表达式分析的析构函数
    for_map(IR_block_ptr, Fact_set_var_ptr, i, t->mapInFact)
        RDELETE(Fact_set_var, i->val);
    for_map(IR_block_ptr, Fact_set_var_ptr, i, t->mapOutFact)
        RDELETE(Fact_set_var, i->val);
    for_map(IR_var, Vec_ptr_IR_var, i, t->mapExprKill)
        DELETE(i->val);
    Map_Expr_IR_var_teardown(&t->mapExpr);
    Map_IR_var_Vec_ptr_IR_var_teardown(&t->mapExprKill);
    Map_IR_block_ptr_Fact_set_var_ptr_teardown(&t->mapInFact);
    Map_IR_block_ptr_Fact_set_var_ptr_teardown(&t->mapOutFact);
}

static bool
AvailableExpressionsAnalysis_isForward (AvailableExpressionsAnalysis *t) {
    return true; // Forward Analysis
}

static Fact_set_var*
AvailableExpressionsAnalysis_newBoundaryFact (AvailableExpressionsAnalysis *t, IR_function *func) {
    /* TODO:
     * OutFact[Entry] = (Bottom: empty set) / (Top: universal set) / other?
     * return NEW(Fact_set_var, is_top?);
     */
    // TODO();
    // VCALL(t->mapOutFact, set, func->entry, NULL);
    return NEW(Fact_set_var, false);
}

static Fact_set_var*
AvailableExpressionsAnalysis_newInitialFact (AvailableExpressionsAnalysis *t) {
    /* TODO:
     * Must/May Analysis ?
     * InitFact = (Bottom: empty set) / (Top: universal set) / other?
     * return NEW(Fact_set_var, is_top?);
     */
    // TODO();
    // VCALL(t->mapInFact, set, )
    return NEW(Fact_set_var, true);
}

static void
AvailableExpressionsAnalysis_setInFact (AvailableExpressionsAnalysis *t,
                                        IR_block *blk,
                                        Fact_set_var *fact) {
    VCALL(t->mapInFact, set, blk, fact);
}

static void
AvailableExpressionsAnalysis_setOutFact (AvailableExpressionsAnalysis *t,
                                         IR_block *blk,
                                         Fact_set_var *fact) {
    VCALL(t->mapOutFact, set, blk, fact);
}

static Fact_set_var*
AvailableExpressionsAnalysis_getInFact (AvailableExpressionsAnalysis *t, IR_block *blk) {
    return VCALL(t->mapInFact, get, blk);
}

static Fact_set_var*
AvailableExpressionsAnalysis_getOutFact (AvailableExpressionsAnalysis *t, IR_block *blk) {
    return VCALL(t->mapOutFact, get, blk);
}

static bool
AvailableExpressionsAnalysis_meetInto (AvailableExpressionsAnalysis *t,
                                       Fact_set_var *fact,
                                       Fact_set_var *target) {
    if(fact->is_top) return false;
    if(target->is_top) {
        // TOP => universal set
        // target->set初始为空集, 清除is_top标记, 直接取union表示赋值
        target->is_top = false;
        VCALL(target->set, union_with, &fact->set);
        return true;
    }
    /* TODO:
     * Must/May Analysis ?
     * IN[blk] = union_with / intersect_with (all OUT[pred_blk]) ?  这个应该是一个遍历？
     * return VCALL(target->set, union_with / intersect_with, &fact->set);  这一句可以直接复制
     */
    // TODO();
    // must
    return VCALL(target->set, intersect_with, &fact->set);
}
//转移基本语句？
void AvailableExpressionsAnalysis_transferStmt (AvailableExpressionsAnalysis *t,  
                                                IR_stmt *stmt,
                                                Fact_set_var *fact) {
    IR_var def = VCALL(*stmt, get_def);  //获得该语句的定义？  实际上获得的是该语句的标号
    // e_kill: kill new_def oprand in the expr
    if(def != IR_VAR_NONE && VCALL(t->mapExprKill, exist, def)) {//不是没有变量的语句（？），同时被杀死了
        Vec_IR_var *killed_expr_var = VCALL(t->mapExprKill, get, def); //获得被杀死的这个vector
        for_vec(IR_var, i, *killed_expr_var)  //遍历删除set中被杀死
            VCALL(fact->set, delete, *i); 
    }
    // e_gen
    if(stmt->stmt_type == IR_OP_STMT) { //这个语句的类型是OP
        IR_op_stmt *op_stmt = (IR_op_stmt*)stmt;  
        VCALL(fact->set, insert, op_stmt->rd);  //把这个语句的标号插入fact对应的set
    }
}
//转移基本块
bool AvailableExpressionsAnalysis_transferBlock (AvailableExpressionsAnalysis *t,
                                                IR_block *block,
                                                Fact_set_var *in_fact,
                                                Fact_set_var *out_fact) {
    Fact_set_var *new_out_fact = AvailableExpressionsAnalysis_newInitialFact(t);  //获得一个新的out
    AvailableExpressionsAnalysis_meetInto(t, in_fact, new_out_fact);  //通过各个in，给新的out赋值
    for_list(IR_stmt_ptr, i, block->stmts) { //遍历基本块中的语句
        IR_stmt *stmt = i->val;
        AvailableExpressionsAnalysis_transferStmt(t, stmt, new_out_fact);  //每个语句都进行转移
    }
    bool updated = AvailableExpressionsAnalysis_meetInto(t, new_out_fact, out_fact); //是否更新了
    RDELETE(Fact_set_var, new_out_fact);
    return updated;  //返回是否更新
}

void AvailableExpressionsAnalysis_print_result (AvailableExpressionsAnalysis *t, IR_function *func) {
    printf("Function %s: Available Expressions Analysis Result\n", func->func_name);
    for_list(IR_block_ptr, i, func->blocks) {
        IR_block *blk = i->val;
        printf("=================\n");
        printf("{Block%s %p}\n", blk == func->entry ? "(Entry)" :
                                 blk == func->exit ? "(Exit)" : "",
               blk);
        IR_block_print(blk, stdout);
        Fact_set_var *in_fact = VCALL(*t, getInFact, blk),
                     *out_fact = VCALL(*t, getOutFact, blk);
        printf("[In(top:%d)]:  ", in_fact->is_top);
        for_set(IR_var, var, in_fact->set)
            printf("v%u ", var->key);
        printf("\n");
        printf("[Out(top:%d)]: ", out_fact->is_top);
        for_set(IR_var, var, out_fact->set)
            printf("v%u ", var->key);
        printf("\n");
        printf("=================\n");
    }
}

void AvailableExpressionsAnalysis_init(AvailableExpressionsAnalysis *t) {
    const static struct AvailableExpressionsAnalysis_virtualTable vTable = {
            .teardown        = AvailableExpressionsAnalysis_teardown,
            .isForward       = AvailableExpressionsAnalysis_isForward,
            .newBoundaryFact = AvailableExpressionsAnalysis_newBoundaryFact,
            .newInitialFact  = AvailableExpressionsAnalysis_newInitialFact,
            .setInFact       = AvailableExpressionsAnalysis_setInFact,
            .setOutFact      = AvailableExpressionsAnalysis_setOutFact,
            .getInFact       = AvailableExpressionsAnalysis_getInFact,
            .getOutFact      = AvailableExpressionsAnalysis_getOutFact,
            .meetInto        = AvailableExpressionsAnalysis_meetInto,
            .transferBlock   = AvailableExpressionsAnalysis_transferBlock,
            .printResult     = AvailableExpressionsAnalysis_print_result
    };
    t->vTable = &vTable;
    Map_Expr_IR_var_init(&t->mapExpr); //表达式映射到IR_var
    Map_IR_var_Vec_ptr_IR_var_init(&t->mapExprKill);  //IR_var映射到其vector指针，存储杀死的表达式
    Map_IR_block_ptr_Fact_set_var_ptr_init(&t->mapInFact);  //把IR_block映射到集合指针，存储In表达式
    Map_IR_block_ptr_Fact_set_var_ptr_init(&t->mapOutFact);
}

//// ============================ Optimize ============================

static void map_expr_kill_insert(AvailableExpressionsAnalysis *t,
                              IR_var use_var,
                              IR_var expr_var) {
    Vec_IR_var *vec_e_kill;
    if(!VCALL(t->mapExprKill, exist, use_var)) {//usr_var没有被杀死
        vec_e_kill = NEW(Vec_IR_var); 
        VCALL(t->mapExprKill, insert, use_var, vec_e_kill); //插入map，即杀死该变量
    } else vec_e_kill = VCALL(t->mapExprKill, get, use_var);  //否则查找map，获得对应的值
    VCALL(*vec_e_kill, push_back, expr_var);//为什么要push啊
}

static IR_var create_new_expr_var_map(AvailableExpressionsAnalysis *t, Expr expr) {
    IR_var expr_var = ir_var_generator();  //获得一个新的变量标号
    VCALL(t->mapExpr, insert, expr, expr_var);//把expr和其标号expr_var插入map
    if(!expr.rs1.is_const) map_expr_kill_insert(t, expr.rs1.var, expr_var);//第一个操作数不是常量，杀死第一个操作数？
    if(!expr.rs2.is_const) map_expr_kill_insert(t, expr.rs2.var, expr_var);//第二个操作数不是常数
    return expr_var;
}

// 简单等价表达式替换
bool simple_expr_optimize(IR_stmt **stmt_ptr_ptr) {
    IR_stmt *stmt = *stmt_ptr_ptr;
    IR_op_stmt *op_stmt = (IR_op_stmt*)stmt;
    switch (op_stmt->op) {
        case IR_OP_ADD: {
            if(op_stmt->rs1.is_const && op_stmt->rs1.const_val == 0) {//第一个操作数是0
                *stmt_ptr_ptr = (IR_stmt*)NEW(IR_assign_stmt, op_stmt->rd, op_stmt->rs2);//替换为赋值语句
                RDELETE(IR_stmt, stmt);
                return true;
            }
            if(op_stmt->rs2.is_const && op_stmt->rs2.const_val == 0) {//第二个操作数为0
                *stmt_ptr_ptr = (IR_stmt*)NEW(IR_assign_stmt, op_stmt->rd, op_stmt->rs1);//替换为赋值
                RDELETE(IR_stmt, stmt);
                return true;
            }
            break;
        }
        case IR_OP_SUB: {
            if(op_stmt->rs2.is_const && op_stmt->rs2.const_val == 0) {//第二个操作数为0
                *stmt_ptr_ptr = (IR_stmt*)NEW(IR_assign_stmt, op_stmt->rd, op_stmt->rs1);
                RDELETE(IR_stmt, stmt);
                return true;
            }
            break;
        }
        case IR_OP_MUL: {
            if(op_stmt->rs1.is_const && op_stmt->rs1.const_val == 1) {//第一个乘数为1
                *stmt_ptr_ptr = (IR_stmt*)NEW(IR_assign_stmt, op_stmt->rd, op_stmt->rs2);
                RDELETE(IR_stmt, stmt);
                return true;
            }
            if(op_stmt->rs2.is_const && op_stmt->rs2.const_val == 1) {//第二个乘数为1
                *stmt_ptr_ptr = (IR_stmt*)NEW(IR_assign_stmt, op_stmt->rd, op_stmt->rs1);
                RDELETE(IR_stmt, stmt);
                return true;
            }
            break;
        }
        case IR_OP_DIV: {
            if(op_stmt->rs2.is_const && op_stmt->rs2.const_val == 1) {//除数为1
                *stmt_ptr_ptr = (IR_stmt*)NEW(IR_assign_stmt, op_stmt->rd, op_stmt->rs1);
                RDELETE(IR_stmt, stmt);
                return true;
            }
            break;
        }
    }
    return false;//没有优化
}


// 将公共表达式标记为相同变量
void AvailableExpressionsAnalysis_merge_common_expr(AvailableExpressionsAnalysis *t,
                                                    IR_function *func) {
    for_list(IR_block_ptr, i, func->blocks) { //遍历block
        IR_block *blk = i->val;//val是list中i对应block的值
        for_list(IR_stmt_ptr, j, i->val->stmts) {//遍历基本块中的每个语句
            IR_stmt *stmt = j->val;
            if(stmt->stmt_type == IR_OP_STMT) {//类型为OP
                IR_op_stmt *op_stmt = (IR_op_stmt*)stmt;
                if(simple_expr_optimize(&j->val)) continue;//对该表达式能够进行简单的优化，则跳过下列语句（类型已经不是OP）
                Expr expr = {.op = op_stmt->op, .rs1 = op_stmt->rs1, .rs2 = op_stmt->rs2};//复制到expr
                if(expr.op == IR_OP_ADD || expr.op == IR_OP_MUL) {//乘法或者加法，可以交换操作数的位置
                    if(IR_val_CMP(expr.rs1, expr.rs2) == 0) {//a > b
                        IR_val tmp = expr.rs1;
                        expr.rs1 = expr.rs2;
                        expr.rs2 = tmp;
                    }
                }//expr是与op_stmt操作数位置相对称的语句
                IR_var expr_var;//表达式的标号
                if(!VCALL(t->mapExpr, exist, expr)) {//不存在expr
                    expr_var = create_new_expr_var_map(t, expr);//把expr加入map
                } else expr_var = VCALL(t->mapExpr, get, expr);//否则查map获得expr_var
                VCALL(blk->stmts, insert_back, j,  //新建一个stmt插在j后面？
                      (IR_stmt*)NEW(IR_assign_stmt,
                                    op_stmt->rd,
                                    (IR_val){.is_const = false, .var = expr_var}));
                op_stmt->rd = expr_var;  //这里，将标号赋值为相同的
            }
        }
    }
}

// 消除公共表达式
static void block_remove_available_expr_def (AvailableExpressionsAnalysis *t, IR_block *blk) {
    Fact_set_var *blk_in_fact = VCALL(*t, getInFact, blk);//获得blk_in_fact
    Fact_set_var *new_in_fact = AvailableExpressionsAnalysis_newInitialFact(t);//新创建一个集合
    AvailableExpressionsAnalysis_meetInto(t, blk_in_fact, new_in_fact); //分析它的in和out
    for_list(IR_stmt_ptr, i, blk->stmts) {
        IR_stmt *stmt = i->val;//对每个基本块语句
        if(stmt->stmt_type == IR_OP_STMT) {//类型为OP
            IR_op_stmt *op_stmt = (IR_op_stmt*)stmt;  //把这个地址转换一下，话说这样不会使得地址空间不匹配吗
            IR_var expr_var = op_stmt->rd;
            if(VCALL(new_in_fact->set, exist, expr_var)) // available  在新创建的这个集合中可用，
                stmt->dead = true; //这个语句就dead了
        }
        AvailableExpressionsAnalysis_transferStmt(t, stmt, new_in_fact);
    }
    RDELETE(Fact_set_var, new_in_fact);//删除刚刚创建的这个
    remove_dead_stmt(blk); // 删除标记为 dead 的可用表达式赋值
}

void AvailableExpressionsAnalysis_remove_available_expr_def (AvailableExpressionsAnalysis *t, IR_function *func) {
    for_list(IR_block_ptr, j, func->blocks) { //遍历该函数的基本块
        IR_block *blk = j->val;
        block_remove_available_expr_def(t, blk);
    }
}



