%locations
%{

    #include <stdio.h>
    #include "lex.yy.c"
    #include "mylib.h"
    #ifndef YYSTYPE
    #define YYSTYPE pNode
    #endif   
    // 
    pNode root;
    

%}



%token INT
%token  FLOAT
%token  ID
%token PLUS MINUS STAR DIV
%token ASSIGNOP SEMI COMMA RELOP AND OR DOT NOT TYPE LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE 

%nonassoc LOWER_THAN_LB
%nonassoc LOWER_THAN_LP
%nonassoc EXP_ERROR
%nonassoc error

%precedence LOWER_ID
%precedence HIGHER_ID

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV 
%right NOT UMINUS
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%start Program

%%
Program : ExtDefList {
    // printf("ROOT\n");
    $$ = createNode(@$.first_line,grammer_,"Program",NULL,1,$1);root=$$;}
    ;
ExtDefList : {$$ = NULL;}
    |   ExtDef ExtDefList {
        // printf("ExtDefList\n");
        $$ = createNode(@$.first_line,grammer_,"ExtDefList",NULL,2,$1,$2);}
    ;
ExtDef :Specifier ExtDecList SEMI  {$$ = createNode(@$.first_line,grammer_,"ExtDef",NULL,3,$1,$2,$3);}
    | Specifier SEMI  {$$ = createNode(@$.first_line,grammer_,"ExtDef",NULL,2,$1,$2);}
    | Specifier FunDec SEMI {$$ = createNode(@$.first_line,grammer_,"ExtDef",NULL,3,$1,$2,$3);}
    | Specifier FunDec CompSt {
        // printf("match ExtDef\n");
        $$ = createNode(@$.first_line,grammer_,"ExtDef",NULL,3,$1,$2,$3);}
    | Specifier error SEMI {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | Specifier error CompSt {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | Specifier FunDec error {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | error SEMI {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    ;

ExtDecList: VarDec  {
    // printf("match VarDec\n");
    $$ = createNode(@$.first_line,grammer_,"ExtDecList",NULL,1,$1);}
    | VarDec COMMA ExtDecList {$$ = createNode(@$.first_line,grammer_,"ExtDecList",NULL,3,$1,$2,$3);}
    ;
Specifier :TYPE  {
    // printf("match type\n");
    $$ = createNode(@$.first_line,grammer_,"Specifier",NULL,1,$1);}
    | StructSpecifier  {$$ = createNode(@$.first_line,grammer_,"Specifier",NULL,1,$1);}
    ;
StructSpecifier :STRUCT OptTag LC DefList RC  {$$ = createNode(@$.first_line,grammer_,"StructSpecifier",NULL,5,$1,$2,$3,$4,$5);}
    | STRUCT Tag {$$ = createNode(@$.first_line,grammer_,"StructSpecifier",NULL,2,$1,$2);}
    | STRUCT error {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    // | STRUCT OptTag error{findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | STRUCT OptTag LC error RC {findError=1;printError(yylineno,B_,func_,"syntax error!");}
    // | error OptTag LC DefList RC{findError=1; printError(yylineno,B_,func_,"No symbol \"struct\"!");}
    // | error Tag{findError=1; printError(yylineno,B_,func_,"No symbol \"struct\"!");}

    ;
OptTag : {$$ = NULL;}
    | ID %prec LOWER_ID {$$ = createNode(@$.first_line,grammer_,"OptTag",NULL,1,$1);}
    ;
Tag : ID %prec HIGHER_ID{$$ = createNode(@$.first_line,grammer_,"Tag",NULL,1,$1);}
    ;
VarDec :ID {//printf("match ID\n");
    $$ = createNode(@$.first_line,grammer_,"VarDec",NULL,1,$1);}
    | VarDec  LB  INT  RB {$$ = createNode(@$.first_line,grammer_,"VarDec",NULL,4,$1,$2,$3,$4);}
    | VarDec  LB  error RB {findError=1; printError(yylineno,B_,func_,"error");}
    |  error LB INT  RB {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    ;
FunDec :ID  LP  VarList  RP  {$$ = createNode(@$.first_line,grammer_,"FunDec",NULL,4,$1,$2,$3,$4);}
    | ID  LP  RP {$$ = createNode(@$.first_line,grammer_,"FunDec",NULL,3,$1,$2,$3);}
    | ID LP error RP {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | error LP RP {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | ID  LP  VarList  error {findError=1; printError(yylineno,B_,char_,")");}
    | ID  error  VarList  RP {findError=1; printError(yylineno,B_,char_,"(");}
    | ID error RP {findError=1; printError(yylineno,B_,char_,"(");}
    | ID LP error {findError=1; printError(yylineno,B_,char_,")");}
    ;
VarList :ParamDec COMMA VarList {$$ = createNode(@$.first_line,grammer_,"VarList",NULL,3,$1,$2,$3);}
    | ParamDec {$$ = createNode(@$.first_line,grammer_,"VarList",NULL,1,$1);}
    ;
ParamDec :Specifier VarDec %prec LOWER_THAN_LB{$$ = createNode(@$.first_line,grammer_,"ParamDec",NULL,2,$1,$2);}
    ;
CompSt :LC DefList StmtList RC {$$ = createNode(@$.first_line,grammer_,"CompSt",NULL,4,$1,$2,$3,$4);}
    // | LC  error RC  {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    ;
StmtList : {$$=NULL;}
    |Stmt StmtList {$$ = createNode(@$.first_line,grammer_,"StmtList",NULL,2,$1,$2);}
    ;
Stmt :Exp SEMI  {$$ = createNode(@$.first_line,grammer_,"Stmt",NULL,2,$1,$2);}
    | CompSt {$$ = createNode(@$.first_line,grammer_,"Stmt",NULL,1,$1);}
    | RETURN Exp SEMI {
        // printf("find return\n");
        $$ = createNode(@$.first_line,grammer_,"Stmt",NULL,3,$1,$2,$3);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = createNode(@$.first_line,grammer_,"Stmt",NULL,5,$1,$2,$3,$4,$5);}
    | IF LP Exp RP Stmt ELSE Stmt{$$ = createNode(@$.first_line,grammer_,"Stmt",NULL,7,$1,$2,$3,$4,$5,$6,$7);}
    | WHILE LP Exp RP Stmt {$$ = createNode(@$.first_line,grammer_,"Stmt",NULL,5,$1,$2,$3,$4,$5);}
    | IF error ELSE Stmt {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | IF error Stmt {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | WHILE error Stmt {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | RETURN error SEMI  {
        // printf("find error in return\n");
        findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | error SEMI {
        // printf("find error in last\n");
        findError=1; printError(yylineno,B_,func_,"syntax error!");}
    ;
DefList : {$$ = NULL;}
    |Def DefList {$$ = createNode(@$.first_line,grammer_,"DefList",NULL,2,$1,$2);}
    ;
Def :Specifier DecList SEMI  {$$ = createNode(@$.first_line,grammer_,"Def",NULL,3,$1,$2,$3);}
    | Specifier error SEMI  {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    ;
DecList :Dec  {$$ = createNode(@$.first_line,grammer_,"DecList",NULL,1,$1);}
    | Dec COMMA DecList {$$ = createNode(@$.first_line,grammer_,"DecList",NULL,3,$1,$2,$3);}
    | error DecList {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    ;
Dec :VarDec {$$ = createNode(@$.first_line,grammer_,"Dec",NULL,1,$1);}
    | VarDec ASSIGNOP Exp {$$ = createNode(@$.first_line,grammer_,"Dec",NULL,3,$1,$2,$3);}
    | VarDec error Exp {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | VarDec ASSIGNOP error  {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    ;
Exp :Exp ASSIGNOP Exp {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | Exp AND Exp {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | Exp OR Exp {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | Exp RELOP Exp {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | Exp PLUS Exp  {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | Exp MINUS Exp {
        // printf("find minus\n");
        $$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | Exp STAR Exp {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | Exp DIV Exp {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | LP Exp RP {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | MINUS Exp %prec UMINUS{$$ = createNode(@$.first_line,grammer_,"Exp",NULL,2,$1,$2);}
    | NOT Exp {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,2,$1,$2);}
    | ID LP Args RP {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,4,$1,$2,$3,$4);}
    | ID LP RP {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | Exp LB Exp RB {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,4,$1,$2,$3,$4);}
    | Exp DOT ID {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,3,$1,$2,$3);}
    | ID {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,1,$1 );}
    | INT {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,1,$1 );}
    | FLOAT {$$ = createNode(@$.first_line,grammer_,"Exp",NULL,1,$1 );}
    | Exp error Exp {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | Exp error {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    | ID LP  error RP %prec EXP_ERROR {findError=1; printError(yylineno,B_,func_,"syntax error!");}
    // | error %prec EXP_ERROR{findError=1; printError(yylineno,B_,func_,"syntax error!");}
    ;
Args :Exp COMMA Args {$$ = createNode(@$.first_line,grammer_,"Args",NULL,3,$1,$2,$3);}
    | Exp {$$ = createNode(@$.first_line,grammer_,"Args",NULL,1,$1 );}
    ;
%%
// #include "lex.yy.c"
// int main(){
//     if(a)
//     yyparse();
// }
// int main(int argc, char** argv)
// {
    
//     if (argc <= 1) return 1;
//     FILE* f = fopen(argv[1], "r");
//     if (!f)
//     {
//         perror(argv[1]);
//         return 1;
//     }
//     yyrestart(f);
//     // 
//     // yydebug = 1;
//     findError = 0;
//     error_line = 0;
//     // root = NULL;
//     yyparse();
//     printTree(root,0);

//     return 0;
// }
yyerror(char* msg){
    // fprintf(stderr, "error: %s,line:%d,fistline%d lastline%d columu:%d yytext%s.\n", msg,yylineno,yylloc.first_line, yylloc.last_line,yylloc.first_column,yytext);
}
