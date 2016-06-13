#ifndef EVAL_H
#define EVAL_H

#include "defs.h"

typedef enum {
    unevaluated , evaluating, evaluated
} evalstatus;
int getCounter();
void addExpressionToList( TN node );
void unMarkAll();
long hashReturn( int index);
long evalNode(  TN node );


#endif
