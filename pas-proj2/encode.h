#ifndef ENCODE_H
#define ENCODE_H

#include  "backend-x86.h"

//// Project 2

void en_b_push_const_string (char *string);
void en_b_push_ext_addr( char * str );
void en_b_push_const_int (int value);
void en_b_push_const_double (double value);

//// project 1
void back_end_call(char *id, int alignment, unsigned int size);


#endif
