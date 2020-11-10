CC = CC

simple: simple.l simple.y
	bison -d -o simple.tab.c simple.y
	flex -o simple.lex.c simple.l
	$(CC) -o simple simple.tab.c simple.lex.c 

clean:
	rm *.c *.h
