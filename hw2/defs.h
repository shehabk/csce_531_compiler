#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include<stdlib.h>

#define DEBUG 0

#define debug(s)      if (DEBUG) fprintf(stderr,s),getchar()
#define debug1(s,t)   if (DEBUG) fprintf(stderr,s,t),getchar()
#define debug2(s,t,u) if (DEBUG) fprintf(stderr,s,t,u),getchar()

typedef int boolean;

#define FALSE 0
#define TRUE 1




int  get_val(int);
void set_val(int, int);

typedef enum {
    unop, binop, intconst , var
} tagtype;

typedef enum {
    UN_PLUS, UN_MINUS, UN_HASH 
} unoptype;

typedef enum {
	PLUS , MINUS , MUL , DIV , MOD
} binoptype;

typedef struct tn{
	tagtype tag ;
	union{
		struct{
			unoptype op ;
			struct tn * operand ;
		} unop ;

		struct{
			binoptype op ;
			struct tn *loperand ;
			struct tn *roperand ;
		}binop ;

		long intconst ;
		char var ;
	} u ;

} TREE_NODE , *TN ;

#endif
