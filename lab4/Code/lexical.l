%{
    #include "mylib.h"
    #ifndef YYSTYPE
    #define YYSTYPE pNode
    #endif
    #include "syntax.tab.h"
    // extern int findError;
    // yylineno = 1;
    int yycolumn = 1;
        #define YY_USER_ACTION \
     yylloc.first_line = yylloc.last_line = yylineno; \
     yylloc.first_column = yycolumn; \
     yylloc.last_column = yycolumn + yyleng - 1; \
     yycolumn += yyleng;
    //  FLOAT [+-]?({digit}*\.?{digit}+|{digit}+\.)
%}
%option yylineno
SIGN [-+]
digit [0-9]
delim [ \t\n]
INT (0|[1-9]+{digit}*)
INT_oct 0{digit}+
INT_hex 0x[0-9a-zA-Z]+
FLOAT ({digit}*\.{digit}+|{digit}+\.)
ID [_a-zA-Z]+[_0-9a-zA-Z]*
RELOP >|<|>=|<=|==|!=
TYPE int|float
e_sign [e|E]
%%
"//" {
    // printf("%s",yytext);
    char c = input();
    while(c!='\n')
        c = input();
}

"*/" {
    // printf("%s",yytext);
    findError=1;
    printError(yylineno,B_,func_,"Wrong Comments!");
}
"/*" {
    // printf("%s",yytext);
    char a = input(), b = input();
    int end = 0;
    while(!end){
        if(a=='*' && b=='/'){
            end = 1;
        }
        // else if(a=='/' && b=='*'){
        //     findError=1;
        //     printError(yylineno, B_,func_,"Comments Nested!");
        //     break;
        // }
        else{
            a = b;
            b = input();
        }
    }//attention: error process
}
"+" { yylval = createNode(yylineno,symbol_,"PLUS",yytext,0); return PLUS;}
"-" {yylval = createNode(yylineno,symbol_,"MINUS",yytext,0);return MINUS;}
"*" {yylval = createNode(yylineno,symbol_,"STAR",yytext,0);return STAR;}
"/" {yylval = createNode(yylineno,symbol_,"DIV",yytext,0);return DIV;}
";" {yylval = createNode(yylineno,symbol_,"SEMI",yytext,0);return SEMI;}
"," {yylval = createNode(yylineno,symbol_,"COMMA",yytext,0);return COMMA;}
"=" {yylval = createNode(yylineno,symbol_,"ASSIGNOP",yytext,0);return ASSIGNOP;}
{RELOP}  {yylval = createNode(yylineno,symbol_,"RELOP",yytext,0);return RELOP;}
"&&" {yylval = createNode(yylineno,symbol_,"AND",yytext,0);return AND;}
"||" {yylval = createNode(yylineno,symbol_,"OR",yytext,0);return OR;}
"." {yylval = createNode(yylineno,symbol_,"DOT",yytext,0);return DOT;}
"!" {yylval = createNode(yylineno,symbol_,"NOT",yytext,0);return NOT;}
{TYPE} {yylval = createNode(yylineno,id_type,"TYPE",yytext,0);return TYPE;}
"(" {yylval = createNode(yylineno,symbol_,"LP",yytext,0);return LP;}
")" {yylval = createNode(yylineno,symbol_,"RP",yytext,0);return RP;}
"[" {yylval = createNode(yylineno,symbol_,"LB",yytext,0);return LB;}
"]" {yylval = createNode(yylineno,symbol_,"RB",yytext,0);return RB;}
"{" {yylval = createNode(yylineno,symbol_,"LC",yytext,0);return LC;}
"}" {yylval = createNode(yylineno,symbol_,"RC",yytext,0);return RC;}
"struct" { yylval = createNode(yylineno,symbol_,"STRUCT",yytext,0);return STRUCT;}
"return" { yylval = createNode(yylineno,symbol_,"RETURN",yytext,0);return RETURN;}
"if" { yylval = createNode(yylineno,symbol_,"IF",yytext,0);return IF;}
"else" { yylval = createNode(yylineno,symbol_,"ELSE",yytext,0);return ELSE;}
"while" { yylval = createNode(yylineno,symbol_,"WHILE",yytext,0);return WHILE;}
{ID} {yylval = createNode(yylineno,id_type,"ID",yytext,0);return ID;}
{INT} {yylval = createNode(yylineno,int_,"INT",yytext,0);return INT;}
{INT_oct} {
    // printf("%s",yytext);
    //debug:
     //printf("match:%s\n",yytext);
    int x = 0;
    int neg = 0;
    int i = 0;
    if(yytext[0]=='-'){
        neg = 1;
        i++;
    }
    else if(yytext[0]=='+')
        i++;
    for(i++; i < strlen(yytext); i++,x*=8){
        if(yytext[i]>='0' && yytext[i]<='7'){
            x += yytext[i]-'0';
            // printf("i:%d\n",i);
        }
        else {
            findError = 1;
            // printf("error!\n");
            yylval = createNode(yylineno,int_,"INT","0\0",0);
            printError(yylineno,A_,oct_,yytext);
            return INT;
            break;
            // return;
        }
    }
    // if(!findError){
        x /= 8;
        if(neg)
            x = -x;
        char* str = int2char(x); 
         //printf("%s\n",str);   
        yylval = createNode(yylineno,int_,"INT",str,0);
        return INT;
    // }

}
{INT_hex} {
    
     //printf("match hex:%s\n",yytext);
    int x = 0;
    int neg = 0;
    int i = 0;
    //debug:
    // printf("match:%s\n",yytext);
	// int n = atoi(yytext);
	// printf("n:%d\n",n);
    int len = strlen(yytext);
    if(yytext[0]=='-'){
        neg = 1;
		x = -x;
        i++;
    }
    else if(yytext[0]=='+')
        i++;
    if(yytext[i]=='0'&&yytext[i+1]=='x')
        i+=2;
    else{
        findError = 1;
        // printf("No \"0x\"\n");
    }
    // printf("%d %d\n",i,len);

    for(; i < strlen(yytext); i++){
        if(yytext[i]>='0' && yytext[i]<='9'){
            x += yytext[i]-'0';
        }
        else if(yytext[i]>='a'&&yytext[i]<='f')
            x += 10+yytext[i]-'a';
        else if(yytext[i]>='A'&&yytext[i]<='F')
            x += 10+yytext[i]-'A';
        else {
            findError = 1;
            //debug:
             //printf("error_hex\n");
            yylval = createNode(yylineno,int_,"INT","0\0",0);
            
            printError(yylineno,A_,hex_,yytext);
            return INT;
            break;
            // return;
        }
		if(i < strlen(yytext)-1)
			x *= 16;
		// printf("x:%d ",x);
    }





    // for(; i < strlen(yytext); i++,x*=16){
    //     if(yytext[i]>='0' && yytext[i]<='9'){
    //         x += yytext[i]-'0';
    //     }
    //     else if(yytext[i]>='a'&&yytext[i]<='f')
    //         x += 10+yytext[i]-'a';
    //     else if(yytext[i]>='A'&&yytext[i]<='F')
    //         x += 10+yytext[i]-'A';
    //     else {
    //         // printf("not match yytext[%d]:%c\n",i,yytext[i]);
    //         findError = 1;
    //         yylval = createNode(yylineno,int_,"INT","0\0",0);
            
    //         printError(yylineno,A_,hex_,yytext);
    //         return INT;
    //         break;
    //         // return;
    //     }
    // }
    // if(!findError){
        // x /= 16;
        if(neg)
            x = -x;
        char* str = int2char(x); 
        //debug   
         //printf("match:%s\n",str);
        yylval = createNode(yylineno,int_,"INT",str,0);
        return INT;
    // }
}
{FLOAT} {yylval = createNode(yylineno,float_,"FLOAT",yytext,0);return FLOAT;}
{FLOAT}{e_sign}{SIGN}?{INT} {\
    //printf("%s,%lf\n",yytext,atof(yytext));
    yylval = createNode(yylineno,float_,"FLOAT",yytext,0);return FLOAT;}

\n { yycolumn = 1;}
{delim}+ { }
. {  findError=1;printError(yylineno,A_,0,NULL);}

%%
