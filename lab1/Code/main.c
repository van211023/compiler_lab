#include "mylib.h"
extern pNode root;
extern void yyrestart();
extern int yyparse();
int main(int argc, char** argv)
{
    
    if (argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    // 
    // yydebug = 1;
    findError = 0;
    error_line = 0;
    // root = NULL;
    yyparse();
    printTree(root,0);
    return 0;
}
