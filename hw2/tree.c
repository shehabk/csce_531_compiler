#include "tree.h"


TN make_binopnode( binoptype op , TN left , TN right ){
	TN newNode = (TN) malloc(sizeof(TREE_NODE)) ;

	newNode->tag = binop ;
	newNode->u.binop.op = op ;
	newNode->u.binop.loperand = left  ;
	newNode->u.binop.roperand = right  ;

	return newNode ;
}

TN make_unopnode( unoptype op , TN operand){

	TN newNode = (TN) malloc(sizeof(TREE_NODE)) ;

	newNode->tag = unop  ;
	newNode->u.unop.op = op ;
	newNode->u.unop.operand = operand ;


	return newNode ;
}

TN make_varnode ( long variableName ){

	TN newNode = (TN) malloc(sizeof(TREE_NODE)) ;

		newNode->tag = var ;
		newNode->u.var = variableName ;

		return newNode ;
}

TN make_constnode ( long constValue ){

	TN newNode = (TN) malloc(sizeof(TREE_NODE)) ;

		newNode->tag = intconst ;
		newNode->u.intconst = constValue ;

		return newNode ;
}
