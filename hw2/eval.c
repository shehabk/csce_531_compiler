#include "defs.h"
#include "eval.h"


typedef struct LN{

  evalstatus st ;
  long value ;
  TN rootNode ;

} listNode ;

int counter = 1 ;
listNode mylist[10010];

int getCounter(){
	return counter ;
}
void addExpressionToList( TN node ){

	listNode newNode ;
	newNode.st = unevaluated ;
	newNode.value = 0;
	newNode.rootNode = node ;

	mylist[counter++] = newNode ;

}

void unMarkAll(){
	int i ;
	for(i=1; i<counter; i++ ){
		mylist[i].st = unevaluated ;
	}
}

long hashReturn( int index){

	if(index<=0 || index >= counter){
		fprintf(stderr, "index %d out of range\n", index );
		exit(0);
		//printf("here");
		return 0 ;
	}

	listNode *present = &mylist[index] ;
	long ret = 0 ;

	if(present->st == evaluating){
		fprintf(stderr, "circular reference found\n");
		exit(0);
		//printf("there");
		return 0 ;
	}

	else if( present->st == evaluated ){

		return present->value ;
	}

	present->st = evaluating ;

	ret = evalNode( present->rootNode );

	present->st = evaluated ;
	present->value = ret ;

	return ret ;

}


long evalNode(  TN node ){

		if( node->tag == intconst){
			return node->u.intconst  ;
		}

		else if( node->tag == var){
			return get_val(node->u.var);
		}

		else if (node->tag == unop ){
			if(node->u.unop.op == UN_MINUS){
				return -1* evalNode(node->u.unop.operand) ;
			}
			else if(node->u.unop.op == UN_PLUS){
				return evalNode(node->u.unop.operand) ;
			}

			else if(node->u.unop.op == UN_HASH){
				return hashReturn(evalNode(node->u.unop.operand)) ;
			}
		}

		else if (node->tag == binop){
			if(node->u.binop.op == PLUS ){
				return evalNode(node->u.binop.loperand) + evalNode( node->u.binop.roperand);
			}
			else if( node->u.binop.op == MINUS){
				return evalNode(node->u.binop.loperand) - evalNode( node->u.binop.roperand);
			}
			else if( node->u.binop.op == MUL){
				return evalNode(node->u.binop.loperand) * evalNode( node->u.binop.roperand);
			}
			else if( node->u.binop.op == DIV){
				return evalNode(node->u.binop.loperand) / evalNode( node->u.binop.roperand);
			}
			else if( node->u.binop.op == MOD){
				return evalNode(node->u.binop.loperand) %  evalNode( node->u.binop.roperand);
			}

		}


		return 0 ;
}
