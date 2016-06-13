#ifndef TREE_H
#define TREE_H

#include  "types.h"
#include  "symtab.h"
#include  "encode.h"
#include  "backend-x86.h"


#include  <string.h>

/*
 *
 *  Just for our convenience .......
 *
 *  {"Integer", TYSIGNEDLONGINT},
    {"Real", TYDOUBLE},
    {"Extended", TYLONGDOUBLE},
    {"Single", TYFLOAT},
    {"Word", TYSIGNEDSHORTINT},
    {"Byte", TYSIGNEDCHAR},
    {"Char", TYUNSIGNEDCHAR},
    {"Boolean", TYSIGNEDCHAR},
    {NULL, TYERROR}
 *
 */




/*   ------- Defining Numbers ------ */

typedef enum { NUMBER_REAL, NUMBER_INT } NUMBER_TAG;

typedef struct number_dr {
	NUMBER_TAG tag;
    union {
		int intval ;
		double realval ;
    } u;
} NUMBER , *NUMBER_P;

NUMBER_P tree_make_number_from_int ( int num );
NUMBER_P tree_make_number_from_real(double num );
NUMBER_P tree_make_number_from_sign( char sign , NUMBER_P number);


/*   -------    Numbers End  ------ */


/*   ------- Defining Constant Numbers ------ */


typedef enum { CONST_NUMBER , CONST_BOOL , CONST_STRING } CONSTANT_TAG;

typedef struct constant_dr {
	CONSTANT_TAG tag;
    union {
		NUMBER_P number ;
		BOOLEAN boole ;
		char * str ;
    } u;
} CONSTANT , *CONSTANT_P;


/*   ------- Constant Definition Ends ------ */





///////// structures to handle function and directive list


typedef enum {
    dir_ext , dir_forward
} directiveName;

typedef struct directiveList {
	directiveName name;
    struct directiveList *next ;
} DNODE, *DNODE_P;

typedef struct functionHeading {
	char * name ;
    TYPE  object ;
    PARAM_LIST params ;
} FNODE, *FNODE_P;

/////////  Define Strucutre for Expression /////////////


typedef enum {
    unop, binop,  const_exp , var_exp , fun_exp , empty_exp , array_exp
} exptagtype;


typedef enum {
    UN_PLUS, UN_MINUS, UN_ORD , UN_CHR , UN_SUCC ,  UN_PRED , UN_CARET
} unoptype;

typedef struct expressionlist EXPR_LIST, *EXPR_LIST_P;

typedef struct expression_node {
	TYPE type;
	BOOLEAN isLvalue ;
	exptagtype tag ;

	union{
		struct{
			unoptype op ;
			struct expression_node * operand ;
		} unop ;

		struct{
					B_ARITH_REL_OP op ;
					struct expression_node *loperand ;
					struct expression_node *roperand ;
		}binop ;

		struct{

			struct expression_node *funName  ;
			struct expression_node *funParam ;

		} funexp ;

		struct{
	    	char * funName ;
			EXPR_LIST_P actualParams ;
		} fun ;

		struct{

			struct expression_node *arrayFront  ;
			struct expression_node *arrayIndex  ;

		} arr ;

		char * var ;
		CONSTANT_P constant ;
	}u ;


} ENODE, *ENODE_P;


struct expressionlist {
	ENODE_P expression ;
    struct expressionlist *next;
} ;


typedef struct twoString {

	char *firstLabel  ;
	char *secondLabel ;

} strPair, *strPair_P ;

/* Different control statement processing */

strPair_P while_first ( ENODE_P boolexp );
void while_second(strPair_P pair);

char* if_first( ENODE_P boolexp );
char* if_second( char* exit);

/* Different control statement processing */



int tree_get_size_of_element ( TYPE element );

EXPR_LIST_P tree_make_actual_parameter_list( EXPR_LIST_P elist, ENODE_P enode );

ENODE_P tree_make_exp_array ( ENODE_P arrayFirst, ENODE_P arrayIndex );
ENODE_P tree_make_exp_unop ( unoptype op , ENODE_P operand );
ENODE_P tree_make_exp_binop( B_ARITH_REL_OP op , ENODE_P loperand  , ENODE_P roperand );
ENODE_P tree_make_exp_identifier( char * varname );

ENODE_P tree_make_empty_exp();
ENODE_P tree_make_exp_const( CONSTANT_P constant) ;
ENODE_P tree_make_exp_add_actual_params( ENODE_P name , EXPR_LIST_P paramList);
void tree_evaluate_exp( ENODE_P exp);
void tree_evaluate_exp_fun( ENODE_P fun ) ;
void tree_evaluate_exp_array( ENODE_P arr );

/////////  	   Expression Definition End       /////////////

/*
 *     MIMIC free and malloc
 *
 */
void tree_mimic_malloc ( ENODE_P exp );
void  tree_mimic_free  ( ENODE_P exp );


/*
 *     MIMIC free and malloc
 *
 */



////////       Function Definitions         /////////////

DNODE_P tree_make_directive_list_from_directive ( directiveName directive );
DNODE_P tree_make_directive_list ( DNODE_P list , DNODE_P directive );
FNODE_P tree_make_function_heading( TYPE object, PARAM_LIST params ,  char* functionName );
void  tree_insert_function_declaration_to_symtab( FNODE_P funHead , DNODE_P dirlist);

/////////  	   Function Definition End       /////////////

void tree_after_function_heading_semi( FNODE_P funHead  );

void tree_after_any_declaration_part( FNODE_P funHead  );

void tree_after_statement_semi( FNODE_P funHead );

////////       Function Declaration Second Prod

////////       Assignment Definitions         /////////////

void  tree_process_assignment ( ENODE_P lhs , ENODE_P rhs );

/////////  	   Assignment Definition End       /////////////


typedef struct charlist {
	char * str ;
    struct charlist *next, *prev;
} CLIST, *CLIST_P;



CONSTANT_P tree_make_constant_from_boolean( char * boole );
CONSTANT_P tree_make_constant_from_string ( char * string) ;
CONSTANT_P tree_make_constant_from_number( NUMBER_P number );

//////// Project 2 //////////////

ENODE_P tree_make_exp_from_identifier( char *identifier );
ENODE_P tree_make_exp_from_unsigned_number( NUMBER_P number );

//////// Project 1 //////////////

MEMBER_LIST var_head ;
TLIST_NODE *hd , *array_head ;
strPair_P whileP , ifelseP ;
CLIST_P exitLabelList ;
BOOLEAN flag ;
/* TRY to IMPLEMENT BREAK STATEMENT */

void tree_init_exitLabelList();
void tree_add_exitLabeltoList( char * str );
void tree_remove_exitLabeltoList();
void tree_handle_break_statement();


PARAM_LIST tree_merge_paramlist( PARAM_LIST  list_1  ,  PARAM_LIST list_2);
PARAM_LIST tree_paramlist_from_idlists(MEMBER_LIST idlist , TYPE tname , BOOLEAN isref );
TYPE tree_build_func(TYPE object, PARAM_LIST params , int check );

void tree_var_head_init();
MEMBER_LIST tree_insert_var_head( MEMBER_LIST list ,char * varname );
void tree_install_variables( MEMBER_LIST list , TYPE vartype);


void tree_install_program_name(char *str);

void tree_install_new_type( char *str , TYPE myType);
void tree_array_head_init();
INDEX_LIST tree_insert_array_head( TYPE myPtr );
TYPE tree_build_array ( TYPE object , INDEX_LIST indices);

void tree_unresolvedPtrList_init();
void tree_resolve_unresolved_pointers();
TYPE tree_build_unresolved_ptr( char * new_identifier);
void tree_insert_unresolved_ptr( TYPE myPtr );

TYPE tree_build_basic( char *  myTypeTag );
TYPE tree_build_subrange( CONSTANT_P low , CONSTANT_P high );

#endif
