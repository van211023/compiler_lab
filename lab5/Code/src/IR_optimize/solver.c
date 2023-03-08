//
// Created by hby on 22-12-2.
//

#include <dataflow_analysis.h>

//// ============================ Forward ============================

static void initializeForward(DataflowAnalysis *t, IR_function *func) {//初始化求解器
    for_list(IR_block_ptr, i, func->blocks) {//对于该函数每一个block
        void *new_in_fact = VCALL(*t, newInitialFact);//新建一个in集合
        VCALL(*t, setInFact, i->val, new_in_fact); //把集合new_in_fact作为i的内容插入到t
        if(i->val == func->entry) { // Entry为Boundary, 需要特殊处理
            void *entry_out_fact = VCALL(*t, newBoundaryFact, func);
            VCALL(*t, setOutFact, i->val, entry_out_fact);//out的处理就和in不一样
        } else {
            void *new_out_fact = VCALL(*t, newInitialFact);
            VCALL(*t, setOutFact, i->val, new_out_fact);
        }
    }
}

static void iterativeDoSolveForward(DataflowAnalysis *t, IR_function *func) {//求解器工作
    while(true) {
        bool updated = false;  //假设没有更新
        // 遍历所有blk
        for_list(IR_block_ptr, i, func->blocks) {
            IR_block *blk = i->val;
            // 获取 IN[blk] 与 OUT[blk]
            Fact *in_fact = VCALL(*t, getInFact, blk), *out_fact = VCALL(*t, getOutFact, blk);
            // IN[blk] = meetAll(OUT[pred] for pred in AllPred[blk])
            for_list(IR_block_ptr, j, *VCALL(func->blk_pred, get, blk)) { //遍历前驱
                IR_block *pred = j->val;  //前驱的blk
                Fact *pred_out_fact = VCALL(*t, getOutFact, pred); //获得前驱的out
                VCALL(*t, meetInto, pred_out_fact, in_fact); //调用meetInto函数计算当前的in
            }
            // 若OUT[blk]发生update, 则进行继续迭代
            if(VCALL(*t, transferBlock, blk, in_fact, out_fact))//通过in来求解out，out有改变，返回值为true
                updated = true;//
        }
        if(!updated) break;
    }
    
}

static void worklistDoSolveForward(DataflowAnalysis *t, IR_function *func) {
    List_IR_block_ptr worklist;
    List_IR_block_ptr_init(&worklist);
    // 初始化将所有blk插入worklist
    for_list(IR_block_ptr, i, func->blocks)
        VCALL(worklist, push_back, i->val);
    while(worklist.tail != NULL) {
        // 从worklist的头部中取出 blk
        IR_block *blk = worklist.head->val;
        VCALL(worklist, pop_front);
        // 获取 IN[blk] 与 OUT[blk]
        Fact *in_fact = VCALL(*t, getInFact, blk), *out_fact = VCALL(*t, getOutFact, blk);
        // IN[blk] = meetAll(OUT[pred] for pred in AllPred[blk])
        for_list(IR_block_ptr, i, *VCALL(func->blk_pred, get, blk)) {//获得该blk的前驱
            IR_block *pred = i->val;
            Fact *pred_out_fact = VCALL(*t, getOutFact, pred); //获得前驱的out
            VCALL(*t, meetInto, pred_out_fact, in_fact); //用前驱的out计算当前的in
        }
        // 若OUT[blk]发生update, 则将其后继全部加入 worklist
        if(VCALL(*t, transferBlock, blk, in_fact, out_fact)) //用 当前的in计算当前的out，若out改变，则返回true
            for_list(IR_block_ptr, i, *VCALL(func->blk_succ, get, blk)) //该out改变了，把这个blk的后继放入工作表，继续循环
                VCALL(worklist, push_back, i->val);
    }
    List_IR_block_ptr_teardown(&worklist);
}

//// ============================ Backward ============================

/* TODO:
 * 根据前向求解器的实现，完成后向求解器的实现
 */

static void initializeBackward(DataflowAnalysis *t, IR_function *func) {
    // TODO();

    // for_list(IR_block_ptr, i, func->blocks) {//对于该函数每一个block
    //     void *new_in_fact = VCALL(*t, newInitialFact);//新建一个in集合
    //     VCALL(*t, setInFact, i->val, new_in_fact); //把集合new_in_fact作为i的内容插入到t
    //     if(i->val == func->exit) { // exit, 需要特殊处理
    //         void *entry_out_fact = VCALL(*t, newBoundaryFact, func);
    //         VCALL(*t, setOutFact, i->val, entry_out_fact);//out的处理就和in不一样
    //     } else {
    //         void *new_out_fact = VCALL(*t, newInitialFact);
    //         VCALL(*t, setOutFact, i->val, new_out_fact);
    //     }
    // }


    for_list(IR_block_ptr, i, func->blocks) {//对于该函数每一个block
        void *new_out_fact = VCALL(*t, newInitialFact);//新建一个集合
        VCALL(*t, setOutFact, i->val, new_out_fact); //把集合new_out_fact作为i的内容插入到t
        if(i->val == func->exit) { // Exit为Boundary, 需要特殊处理；但是对于后向的活跃变量分析，所有的in都是被初始化为空集
            void *exit_in_fact = VCALL(*t, newBoundaryFact, func);  //可能调用的函数不对
            VCALL(*t, setInFact, i->val, exit_in_fact);
        } else {
            void *new_in_fact = VCALL(*t, newInitialFact);
            VCALL(*t, setInFact, i->val, new_in_fact);
        }
    }

}

static void iterativeDoSolveBackward(DataflowAnalysis *t, IR_function *func) {
    // TODO();

    // while(true) {
    //     bool updated = false;  //假设没有更新
    //     // 遍历所有blk
    //     for_list(IR_block_ptr, i, func->blocks) {
    //         IR_block *blk = i->val;
    //         // 获取 IN[blk] 与 OUT[blk]
    //         Fact *in_fact = VCALL(*t, getInFact, blk), *out_fact = VCALL(*t, getOutFact, blk);
    //         // IN[blk] = meetAll(OUT[pred] for pred in AllPred[blk])
    //         for_list(IR_block_ptr, j, *VCALL(func->blk_pred, get, blk)) { //遍历前驱
    //             IR_block *pred = j->val;  //前驱的blk
    //             Fact *pred_out_fact = VCALL(*t, getOutFact, pred); //获得前驱的out
    //             VCALL(*t, meetInto, pred_out_fact, in_fact); //调用meetInto函数计算当前的in
    //         }
    //         // 若OUT[blk]发生update, 则进行继续迭代
    //         if(VCALL(*t, transferBlock, blk, in_fact, out_fact))//通过in来求解out，out有改变，返回值为true
    //             updated = true;//
    //     }
    //     if(!updated) break;
    // }
    


    while(true) {
        bool updated = false;  //假设没有更新
        // 遍历所有blk
        for_list(IR_block_ptr, i, func->blocks) {
            IR_block *blk = i->val;
            // 获取 IN[blk] 与 OUT[blk]
            Fact *in_fact = VCALL(*t, getInFact, blk), *out_fact = VCALL(*t, getOutFact, blk);
            // IN[blk] = meetAll(OUT[pred] for pred in AllPred[blk])
            for_list(IR_block_ptr, j, *VCALL(func->blk_succ, get, blk)) { //遍历后继
                IR_block *succ = j->val;  //后继的blk
                Fact *succ_in_fact = VCALL(*t, getInFact, succ); //获得后继的in
                VCALL(*t, meetInto, succ_in_fact, out_fact); //调用meetInto函数计算当前的out
            }
            // 若OUT[blk]发生update, 则进行继续迭代
            if(VCALL(*t, transferBlock, blk, in_fact, out_fact))//通过out来更新in，out有改变，返回值为true
                updated = true;//
        }
        if(!updated) break;
    }
}

static void worklistDoSolveBackward(DataflowAnalysis *t, IR_function *func) {
    // TODO();

    List_IR_block_ptr worklist;
    List_IR_block_ptr_init(&worklist);
    // 初始化将所有blk插入worklist
    for_list(IR_block_ptr, i, func->blocks)
        VCALL(worklist, push_back, i->val);
    while(worklist.tail != NULL) {
        // 从worklist的头部中取出 blk
        IR_block *blk = worklist.head->val;
        VCALL(worklist, pop_front);
        // 获取 IN[blk] 与 OUT[blk]
        Fact *in_fact = VCALL(*t, getInFact, blk), *out_fact = VCALL(*t, getOutFact, blk);
        // OUT[blk] = meetAll(OUT[pred] for pred in AllPred[blk])
        for_list(IR_block_ptr, i, *VCALL(func->blk_succ, get, blk)) { //取出前驱
            IR_block *succ = i->val;
            Fact *succ_in_fact = VCALL(*t, getInFact, succ); //获得后继的in
            VCALL(*t, meetInto, succ_in_fact, out_fact); //计算后赋值给当前的out
        }
        // 若In[blk]发生update, 则将其前驱全部加入 worklist
        if(VCALL(*t, transferBlock, blk,in_fact, out_fact))
            for_list(IR_block_ptr, i, *VCALL(func->blk_pred, get, blk))
                VCALL(worklist, push_back, i->val);
    }
    List_IR_block_ptr_teardown(&worklist);

}

//// ============================ Solver ============================

void worklist_solver(DataflowAnalysis *t, IR_function *func) {//工作表算法
    if(VCALL(*t, isForward)) {//向前
        initializeForward(t, func);
        worklistDoSolveForward(t, func);
    } else {//向后
        initializeBackward(t, func);
        worklistDoSolveBackward(t, func);
    }
}

void iterative_solver(DataflowAnalysis *t, IR_function *func) {//迭代算法
    if(VCALL(*t, isForward)) {
        initializeForward(t, func);
        iterativeDoSolveForward(t, func);
    } else {
        initializeBackward(t, func);
        iterativeDoSolveBackward(t, func);
    }
}