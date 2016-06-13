#
#	makefile for the Simple Expression Evaluator
#
#

CC = gcc

CFLAGS = -g

LIBS = -lfl

YACC = bison

LEX = flex

SOURCE = parse.y main.c

OBJECTS = parse.o main.o lex.yy.o tree.o eval.o

see: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o see

y.tab.h parse.c: parse.y defs.h tree.h
	$(YACC) -y -d -t -v parse.y
	mv y.tab.c parse.c

lex.yy.c: scan.l defs.h y.tab.h
	flex scan.l

clean:
	-rm *.o *.out *.err parse.c y.output y.tab.h lex.yy.c see
