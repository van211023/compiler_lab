#include "mylib.h"
//lineno, A_|B_,error type(oct_|hex_|char_),print_msg
void printError(int lineno, int type, int error, char* msg)
{//Error type A at Line 4: Mysterious character "~"

    //debug:
    // return;
    // printf("yytext:%s\n",yytext);
    if(error_line==lineno)
        return;
    printf("Error type %c at line %d: ",(type==A_)?'A':'B',lineno);
    if(error==oct_)
        printf("Illegal octal number \'%s\'\n",msg);
    else if(error==hex_)
        printf("Illegal hexadecimal number \'%s\'\n",msg);
    else if(type==B_ && error==char_){
        printf("Missing \"%s\"\n",msg);
    }
    else if(type==B_ && error==func_){
        printf("%s\n", msg);
    }
    error_line=lineno;
}
char* int2char(int n)
{
	
	char* str = (char*)malloc(sizeof(char)*100);//存储数字的字符串，长度任意指定
	// x = atoi(str);
	// scanf("%d",&x);
	int i;
	int neg = 0;
	if(n<0){
		n = -n;
		neg = 1;
	}
	// printf("%d\n",n);
	for(i=0;n>0;i++){
		str[i] = n%10+'0';
		// printf("%d\n",n%10);
		n /= 10;
	}
	if(neg)
		str[i] = '-';
	str[i+1] = '\0';
	// printf("%s\n",str);
	int len = strlen(str);
	char tmp;
	for(int i = 0; i < len/2; i++){
		tmp = str[i];
		str[i] = str[len-i-1];
		str[len-i-1] = tmp;
	}
	// printf("%s\n",str);
    return str;

}
pNode createNode(int lineno, int type, char* token, char* val, int childNum, ...)
{
    va_list valist;
    va_start(valist, childNum);

    pNode x = (pNode)malloc(sizeof(Node));
    x->lineno = lineno;
    x->childNum = childNum;
    x->type = type;
    x->child = NULL;
    if(token){
        x->token = (char*)malloc(sizeof(char)*strlen(token));
        strcpy(x->token, token);
    }
    else
        x->token = NULL;
    if(val){//所以理论上讲，储存的所有的val都是字符串？
        x->val = (char*)malloc(sizeof(char)*strlen(val));
        strcpy(x->val, val);
    }
    else
        x->val = NULL;
    if(childNum>0){
        x->child = (Node**)malloc(sizeof(Node*)*childNum);
        for(int i = 0; i < childNum; i++)
            x->child[i] = (va_arg(valist, pNode));
        va_end(valist);
    }
    else
        x->child = NULL;
    return x;
}
void printTree(pNode root, int level)
{
    // printf("%d\n",root->type);
    if(findError){
        //debug:
        // printf("findError!\n");
        return;
    }
    // if()
    if(!root){
        // printf("root is NULL\n");
        return;
    }
    printNode(root, level);
    // if(root->childNum<=0 || root->child==NULL)
    //     return;
    for(int i = 0; i < root->childNum; i++)
        printTree(root->child[i],level+1);
    
}
void printNode(pNode node, int indent)
{
    if(!node)//empty
        return;
    for(int i = 0; i < indent; i++){
        printf("  ");
    }
    if(node->type==grammer_)
        printf("%s (%d)\n",node->token,node->lineno);
    else if(node->type==int_)
        printf("%s: %d\n",node->token,atoi(node->val));
    else if(node->type==float_)
        printf("%s: %lf\n",node->token,atof(node->val));
    else if(node->type==symbol_)
        printf("%s\n",node->token);
    else if(node->type==id_type)
        printf("%s: %s\n",node->token,node->val);//数字也是以字符串的格式打印
    else{
        printf("Warning:error!!\n");
    }
}
