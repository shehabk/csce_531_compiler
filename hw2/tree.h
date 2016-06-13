#ifndef TREE_H
#define TREE_H

#include "defs.h"


TN make_binopnode( binoptype op , TN left , TN right );
TN make_unopnode( unoptype op , TN operand);
TN make_varnode ( long variableName );
TN make_constnode ( long constValue );


#endif
