/* Simple Expression Evaluator */

/* parse.y
 *
 * Grammer rules for bison.
 * Includes the lexical analyzer routine yylex().
 */


%{
#include <ctype.h>
#include<stdlib.h>
#include "defs.h"
#include "tree.h"
#include "eval.h"

#define YYDEBUG 1

int line_no;

%}
	
	
%union{
	long y_int ;
	TN	 y_tree;
}

%type <y_tree> expr term factor hfactor assign
%token <y_int> CONST VAR 

%%

session
    : { print_welcome(); }
      eval			{ printf("\n");}
    ;

eval
    : eval line			{ printf("%4d: ", getCounter() ) ;}
    | /* empty */		{ printf("%4d: ", getCounter() ) ;}
    ;

line
    : assign '\n'		{ printf("\n");}
    ;

assign
    : VAR '=' expr		{ int val = evalNode($3) ; set_val($1, val); unMarkAll(); $$ = $3; addExpressionToList($3) ; printf("%d", val ); }
    | expr				{ $$ = $1 ;   addExpressionToList($1) ; printf("%d",  (int)evalNode($1));}
    ;

expr
    : expr '+' term		{ $$ = make_binopnode( PLUS , $1 ,  $3); }
    | expr '-' term		{ $$ = make_binopnode( MINUS , $1 , $3); }
    | term
    | '+' term			{ $$ = make_unopnode( UN_PLUS,  $2 )  ;	 }
    | '-' term			{ $$ = make_unopnode( UN_MINUS, $2 ) ;   }    
    ;

term
    : term '*' hfactor		{ $$ = make_binopnode( MUL , $1 ,  $3); }
    | term '/' hfactor		{ $$ = make_binopnode( DIV , $1 ,  $3); }
    | term '%' hfactor		{ $$ = make_binopnode( MOD , $1 ,  $3); }
    | hfactor
    ;

hfactor
	: '#' hfactor			{ $$ = make_unopnode( UN_HASH , $2) ;}
	| factor
factor
    : '(' expr ')'		{ $$ = $2; } 
    | CONST				{ $$ = make_constnode($1) ;}
    | VAR				{ $$ = make_varnode($1);   }
    ;

%%



yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}

print_welcome()
{
    printf("Welcome to the Simple Expression Evaluator.\n");
    printf("Enter one expression per line, end with ^D\n\n");
}

static int val_tab[26];

int get_val(int v)
{
    return val_tab[v - 'A'];
}

void set_val(int v, int val)
{

    val_tab[v - 'A'] = val;
}
