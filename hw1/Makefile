babycpp: lex.yy.o main.o my_dict.o
	gcc lex.yy.o main.o my_dict.o -o babycpp -lfl

main.o: main.c my_defs.h
	gcc -c main.c

my_dict.o: my_dict.c my_defs.h
	gcc -c my_dict.c
	
lex.yy.o: lex.yy.c my_defs.h
	gcc -c lex.yy.c
	
lex.yy.c: driver.lex my_defs.h
	flex driver.lex
	
clean: 
	rm *.o lex.yy.c babycpp