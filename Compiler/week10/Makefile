simple.exe: simple.tab.c lex.yy.c
	gcc -o simple.exe simple.tab.c lex.yy.c -ll

simple.tab.c: simple.y
	bison -o simple.tab.c -d simple.y

lex.yy.c: simple.l
	flex simple.l

clean:
	rm simple.exe simple.tab.c simple.tab.h lex.yy.c
