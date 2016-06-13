#ifndef TREE_H
#define TREE_H

#include  "types.h"
#include  "symtab.h"
#include  <string.h>

typedef struct charlist {
	char * str ;
    struct member *next, *prev;
} CLIST, *CLIST_P;

MEMBER_LIST var_head ;
TLIST_NODE *hd , *array_head ;

PARAM_LIST tree_merge_paramlist( PARAM_LIST  list_1  ,  PARAM_LIST list_2);
PARAM_LIST tree_paramlist_from_idlists(MEMBER_LIST idlist , TYPE tname , BOOLEAN isref );
TYPE tree_build_func(TYPE object, PARAM_LIST params);

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
TYPE tree_build_subrange( long low , long high );

#endif
