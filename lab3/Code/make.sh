flex flex.l
bison -d -v -t  syntax.y
gcc -g mylib.c sem.c ir.c main.c syntax.tab.c -lfl -ly -o parser
./parser test.cmm out.ir
