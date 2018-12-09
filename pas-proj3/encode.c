#include "encode.h"

void en_b_push_const_int (int value){
	b_push_const_int (value);
}
void en_b_push_const_double (double value){
	b_push_const_double (value);
}

void en_b_push_const_string (char *string){
	b_push_const_string (string);
}

void en_b_push_ext_addr( char * str ){
	b_push_ext_addr(str);
}

void back_end_call(char *id, int alignment, unsigned int size)
{
	b_global_decl (id, alignment , size);
	b_skip(size);
}
