#include "tree.h"
#include "message.h"
#include <stdlib.h>




/*
           Function heading er vitore
 */

char* if_first( ENODE_P boolexp ){

	char *exitlab = new_symbol() ;

	if( ty_query(boolexp->type)!= TYSIGNEDCHAR){

			error("Non-Boolean expression");
			return exitlab ;
	}

	tree_evaluate_exp(boolexp);
	b_cond_jump(TYSIGNEDCHAR, B_ZERO , exitlab);
	return exitlab ;

}
char* if_second( char* elsel){

	char *exitlab = new_symbol() ;
	b_jump(exitlab);
	b_label(elsel);

	return exitlab;

}

strPair_P while_first ( ENODE_P boolexp ){

	strPair_P node = (strPair*)malloc(sizeof(strPair)) ;


	node->secondLabel = new_symbol() ;
	node->firstLabel  = new_symbol() ;

	////msg(" I am here ");
	if( ty_query(boolexp->type)!= TYSIGNEDCHAR){

		error("Non-Boolean expression");
		return node ;
	}

	tree_add_exitLabeltoList( node->secondLabel );

	b_label(node->firstLabel);
	tree_evaluate_exp(boolexp);
	b_cond_jump(TYSIGNEDCHAR, B_ZERO , node->secondLabel);

	////msg(" I finished here ");

	return node ;
}

void while_second( strPair_P pair){

	b_jump(pair->firstLabel);
	b_label(pair->secondLabel);
	tree_remove_exitLabeltoList();

}

void tree_after_function_heading_semi( 	FNODE_P funHead  ){

	 int block ;
	 ST_ID myST_ID = st_enter_id(funHead->name );
     ST_DR dr      = st_lookup( myST_ID , &block ) ;
     BOOLEAN checkargs = TRUE ;

     if( dr == NULL ){
    	 ST_DR myST_DR = stdr_alloc() ;
    	 myST_DR ->tag = FDECL ;
    	 myST_DR ->u.decl.type = tree_build_func( funHead->object , funHead->params , checkargs);
    	 myST_DR ->u.decl.v.global_func_name  =  funHead->name ;
    	 st_install(myST_ID , myST_DR ) ;
    	 ////msg("function er bhitore");

     }

     else if (dr->tag == GDECL ){
    	 	if( ty_query(dr->u.decl.type) == TYFUNC ){


    	 		ST_DR myST_DR = stdr_alloc() ;
    	 		myST_DR ->tag = FDECL ;
    	 		myST_DR ->u.decl.type = tree_build_func( funHead->object , funHead->params , checkargs);
    	 		myST_DR ->u.decl.v.global_func_name  =  funHead->name ;
    	 		st_install(myST_ID , myST_DR ) ;


    	 	}

    	 	else{

    	 		///  Not a function .....
    	 		return ;

    	 	}

     }

     else if( dr->tag == FDECL){
    	 /// error duplicate declaration
    	 return ;
     }

     st_enter_block() ;
     b_init_formal_param_offset();


}

void tree_after_any_declaration_part( FNODE_P funHead  ){

	    int block ;
	    TYPE object ;
	    BOOLEAN checkargs ;
	    PARAM_LIST param ;

	    b_func_prologue(funHead->name);

		ST_ID myST_ID = st_enter_id(funHead->name );
	    ST_DR dr      = st_lookup( myST_ID , &block ) ;

	    object = ty_query_func( dr->u.decl.type , & param , &checkargs) ;

	    if( ty_query(object) != TYVOID){
	    	b_alloc_return_value() ;
	    	//b_alloc_local_vars(8) ;
	    }


	    	b_alloc_local_vars(0) ;




	    ////msg( " inside any declaration ");

}


void tree_after_statement_semi( FNODE_P funHead  ){

	   int block ;
	   TYPE object ;
	   BOOLEAN checkargs ;
	   PARAM_LIST param ;


	   ////msg("inside after statement ");

	    ST_ID myST_ID = st_enter_id(funHead->name );
		ST_DR dr      = st_lookup( myST_ID , &block ) ;

		object = ty_query_func( dr->u.decl.type , & param , &checkargs) ;

		if(ty_query(object) != TYVOID){
			b_prepare_return(ty_query(object)) ;
		}
		b_func_epilogue(funHead->name);

}

/*   --------------------- MIMIC MALLOC and FREE ------------ */

void tree_mimic_malloc ( ENODE_P exp ){


	TYPE object ;
	ST_ID id ;
	ST_DR dr ;
	int block ;
	char * var ;

	if(exp->tag == var_exp ) {
		var =  (char*) malloc ( strlen( exp->u.var ) + 1 );
		strcpy( var , exp->u.var );

	}

	id = st_enter_id( var );
	dr = st_lookup( id , &block ) ;






    if(dr == NULL ){

    	// not present ....

    }

    if( dr->tag == GDECL){

    	if( ty_query(dr->u.decl.type) == TYPTR){

    		b_push_ext_addr(var) ;
    		b_alloc_arglist( 4 ) ;


			object = ty_query_ptr( dr->u.decl.type, &id ) ;
			b_push_const_int( checkSimple(object));

			b_load_arg(  TYUNSIGNEDINT ) ;

			b_funcall_by_name ("malloc", TYPTR ) ;
			b_assign (TYPTR) ;
			b_pop() ;
    	}

    }




}

/*
# b_push_ext_addr (Rp)
	subl	$8, %esp
	movl	$Rp, (%esp)
				# b_deref (pointer)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_load_arg (pointer)
	movl	(%esp), %eax
	addl	$8, %esp
	movl	%eax, 0(%esp)
				# b_funcall_by_name (free, void)
	call	free
*/

void  tree_mimic_free  ( ENODE_P exp ){


	TYPE object ;
	ST_ID id ;
	ST_DR dr ;
	int block ;
	char* var ;

	if(exp->tag == var_exp ) {
		var =  (char*) malloc ( strlen( exp->u.var ) + 1 );
	    strcpy( var , exp->u.var );
	}


	id = st_enter_id( var);
	dr = st_lookup( id , &block ) ;

	if(dr == NULL){

		// ai name a kisu nai ...
	}

	if( dr->tag == GDECL){

	    	if( ty_query(dr->u.decl.type) == TYPTR){
					  b_alloc_arglist(4) ;
					  b_push_ext_addr(var) ;
					  b_deref(TYPTR);
					  b_load_arg(TYPTR);
					  b_funcall_by_name( "free" , TYVOID ) ;
	    	}
	}


}


/*   --------------------- MIMIC MALLOC and FREE ------------ */



EXPR_LIST_P tree_make_actual_parameter_list( EXPR_LIST_P elist, ENODE_P enode ){

	EXPR_LIST_P  node  , it ;
	node = (EXPR_LIST *)malloc(sizeof(EXPR_LIST));



	if(elist == NULL){
		elist = (EXPR_LIST *)malloc(sizeof(EXPR_LIST));
		elist->expression = enode ;
		elist->next = NULL;

		if( enode->tag == const_exp ){
			if(enode->u.constant->tag == CONST_STRING){
				////msg(enode->u.constant->u.str);
			}
		}

		return elist ;

	}

	node ->expression = enode ;
	node ->next = NULL ;

	it = elist ;
	while( it->next != NULL) it = it->next ;

	it->next= node ;

	return elist;


}
// Handle function declaration


DNODE_P tree_make_directive_list_from_directive ( directiveName directive ){
	DNODE_P node = (DNODE*)malloc(sizeof(DNODE)) ;
	node->name = directive ;
	node->next = NULL ;

	return node ;
}

DNODE_P tree_make_directive_list ( DNODE_P list , DNODE_P directive ){


	if( list == NULL ){


		return directive ;
	}

	else{
		list->next = directive ;
		return list ;
	}


}

FNODE_P tree_make_function_heading( TYPE object, PARAM_LIST params ,  char* functionName ){

	FNODE_P node = (FNODE*)malloc(sizeof(FNODE)) ;

	node->object = object ;
	node->params = params ;

	node->name =  (char*)malloc(strlen(functionName) + 1 ) ;
	strcpy( node->name , functionName ) ;

	return node ;
}


void  tree_insert_function_declaration_to_symtab( FNODE_P funHead , DNODE_P dirlist) {

	ST_DR myST_DR = stdr_alloc() ;
	ST_DR check ;
	TYPE tp ;
	DNODE_P it = dirlist ;
	int block , checkargs = 1 ;


	//Checks only the first directive
	if( it!= NULL){
		if( it->name == dir_forward ){
			myST_DR->tag = GDECL ;
		}

		else if (it->name == dir_ext ){
			myST_DR->tag = GDECL ;
			myST_DR->u.decl.sc = EXTERN_SC ;
			checkargs = 0 ;

		}
	}



	 myST_DR ->u.decl.type = tree_build_func( funHead->object , funHead->params , checkargs);
	 myST_DR ->u.decl.v.global_func_name  =  funHead->name ;

	 ST_ID myST_ID = st_enter_id(funHead->name );
	 check = st_lookup( myST_ID , &block ) ;

	 if(check== NULL){
		 st_install(myST_ID , myST_DR ) ;
	 }

	 else{
	          error("Duplicate forward or external function declaration");
	 }


}

// End handling function declaration










// ---------- Handle Assignment Statement ------------------



// ENODE_P tree_make_exp

void  tree_process_assignment ( ENODE_P lhs , ENODE_P rhs ){


	int block ;
	TYPE object ;
    PARAM_LIST params;
	EXPR_LIST_P actualparams ;
	BOOLEAN check_args ;

	if(lhs== NULL){
		return ;
	}

	///// Handle empty RHS
	if ( rhs != NULL && rhs->tag == empty_exp){

		if( lhs->tag == var_exp ){

		            error("Expected procedure name, saw data");
		            return ;
		}

		else if( lhs->tag != fun_exp ){
				error("Procedure call expected");
				return ;
		}

		tree_evaluate_exp(lhs);

		return ;
	}

	if(rhs == NULL) {

		  return ;
	}

	ST_ID id ;
	ST_DR dr ;

	TYPETAG lhs_type , rhs_type ;
	lhs_type = ty_query(lhs->type) ;
	rhs_type = ty_query(rhs->type) ;

	if(rhs_type == TYVOID){

	        error("Cannot convert between nondata types");
	        return;
	}

	if( rhs != NULL && lhs ->tag == fun_exp ){


		tree_evaluate_exp(rhs) ;

		id = st_enter_id( lhs->u.fun.funName) ;
		dr = st_lookup( id  , & block );

		if(block > 1){
			object = ty_query_func( dr->u.decl.type ,  &params , &check_args);

			if( ty_query(object) == TYVOID) {
				error("Cannot set the return value of a procedure") ;
				return ;
			}
			b_set_return(ty_query(object));

			return ;
		}

	}



	/// process lhs

	if( lhs->isLvalue ){

		if(lhs->tag == var_exp){



			id = st_enter_id( lhs->u.var ) ;
		    dr = st_lookup ( id  , &block  ) ;

		    ////msg(lhs->u.var) ;
		    if( dr == NULL ){

		    	return ;
		    	// error variable not declared before
		    }

		    else if (dr->tag == GDECL){


		    	b_push_ext_addr(lhs->u.var ) ;


		    }
		}

		else if( lhs->tag == array_exp){
			tree_evaluate_exp(lhs);
		}
	}

	else{
		// lhs is r value !!!!
		error("Assignment requires l-value on the left");

	}

	// lhs process ends


	////// Evaluate right expression and do the assignment as discussed in the class ...

	rhs_type =  ty_query(rhs->type) ;

	if( rhs->tag == const_exp ){

		if( rhs->u.constant->tag == CONST_NUMBER){
			NUMBER_P number = rhs->u.constant->u.number ;



			if(ty_query(lhs->type) == TYDOUBLE && rhs_type == TYSIGNEDLONGINT ){
					 //b_convert( TYSIGNEDLONGINT ,  TYDOUBLE  ) ;
					 b_push_const_double( (double) number->u.intval);
					 rhs_type = lhs_type ;
				}



			else if( ty_query(lhs->type) == TYFLOAT && rhs_type == TYDOUBLE ){

				  b_push_const_double( number->u.realval);
				  b_convert( TYDOUBLE ,  TYFLOAT  ) ;
				  rhs_type = lhs_type ;
			}

			else if( ty_query(lhs->type) == TYFLOAT && rhs_type == TYSIGNEDLONGINT ){
					  b_push_const_int( number->u.intval);
				      b_convert( TYSIGNEDLONGINT ,  TYFLOAT  ) ;
					  rhs_type = lhs_type ;
				}

			else if(lhs_type == rhs_type){
				if(rhs_type ==  TYSIGNEDLONGINT )  b_push_const_int( number->u.intval);
				else b_push_const_double( number->u.realval);

			}







				if( lhs_type == rhs_type){
					b_assign(lhs_type);
					b_pop() ;
				}

				else{

					error("Illegal conversion");
					/// Type mismatch in assignment operator ..
				}


				return;
		}
	}


	////msg("eval");
	tree_evaluate_exp(rhs) ;
	if(flag == TRUE){
		flag = FALSE ;
		return;
	}

    ////msg("eval end");

	if( rhs->isLvalue == TRUE ){
       if( checkSimple(rhs->type) != 0){
		b_deref(ty_query(rhs->type)) ;
       }
	}




	if(rhs_type ==  TYFLOAT ){

		 b_convert( TYFLOAT ,  TYDOUBLE  ) ;
		 rhs_type = TYDOUBLE ;
	}

	if(ty_query(lhs->type) == TYDOUBLE && rhs_type == TYSIGNEDLONGINT ){
			 b_convert( TYSIGNEDLONGINT ,  TYDOUBLE  ) ;
			 rhs_type = lhs_type ;
		}
	else if(ty_query(lhs->type) == TYDOUBLE && rhs_type == TYFLOAT ){
		 b_convert( TYFLOAT ,  TYDOUBLE  ) ;
		 rhs_type = lhs_type ;
	}

	else if( ty_query(lhs->type) == TYFLOAT && rhs_type == TYDOUBLE ){
		  b_convert( TYDOUBLE ,  TYFLOAT  ) ;
		  rhs_type = lhs_type ;
	}

	else if( ty_query(lhs->type) == TYFLOAT && rhs_type == TYSIGNEDLONGINT ){
			  b_convert( TYSIGNEDLONGINT ,  TYFLOAT  ) ;
			  rhs_type = lhs_type ;
		}
	else if( ty_query(lhs->type) == TYUNSIGNEDCHAR && rhs_type == TYSIGNEDCHAR ){
			  b_convert( TYSIGNEDCHAR ,  TYUNSIGNEDCHAR  ) ;
			  rhs_type = lhs_type ;
		}







		if( lhs_type == rhs_type){
			b_assign(lhs_type);
			b_pop() ;
		}

		else{

			error("Illegal conversion");
			/// Type mismatch in assignment operator ..
		}




}


// ---------- Handle Assignment Statement End ---------------


////////      Evaluate functions


void tree_evaluate_exp_fun( ENODE_P fun ){

	int block ;
    TYPE object ;
    PARAM_LIST params;
    EXPR_LIST_P actualparams ;
    int count = 0 ;
    BOOLEAN check_args ;
	ST_ID id = st_enter_id( fun->u.fun.funName) ;
	ST_DR dr = st_lookup ( id  , &block  ) ;

	if( dr == NULL){
		// some error
	}

	else{

		object = ty_query_func( dr->u.decl.type , &params , &check_args);

		count = 0 ;
		actualparams  = fun->u.fun.actualParams;
		EXPR_LIST_P it = actualparams ;




        while(it!=NULL){

        	count++ ;
        	it = it->next ;
        }

		//// parameter er size onujai alloc agrlist korte hobe apatoto 0 ..

		b_alloc_arglist( count * 8  ) ;

		if ( check_args == FALSE && dr->u.decl.sc == EXTERN_SC){

			it = actualparams ;
			if(it!=NULL){
				while(it  != NULL){

					if( it->expression->tag == var_exp ){

						id = st_enter_id( it->expression->u.var ) ;
						dr = st_lookup ( id  , &block  ) ;

						b_push_ext_addr( it->expression->u.var);
						b_deref(ty_query(it->expression->type));

						if( ty_query(it->expression->type)== TYFLOAT){
							b_convert( TYFLOAT , TYDOUBLE);
							b_load_arg(TYDOUBLE);
						}

						else if(ty_query(it->expression->type)== TYUNSIGNEDCHAR){
							b_convert( TYUNSIGNEDCHAR , TYSIGNEDLONGINT);
							b_load_arg(TYSIGNEDLONGINT);

						}
						else{
						    b_load_arg( ty_query(it->expression->type) );
						}
					}

					else if( it->expression->tag == const_exp )
					{
						if(it->expression->u.constant->tag == CONST_NUMBER){

							if( it->expression->u.constant->u.number->tag == NUMBER_INT){

								b_push_const_int(it->expression->u.constant->u.number->u.intval);
								b_load_arg(TYSIGNEDLONGINT);
							}

							if( it->expression->u.constant->u.number->tag == NUMBER_REAL){

								b_push_const_int(it->expression->u.constant->u.number->u.realval);
								b_load_arg(TYDOUBLE);
							}

						}

						else if(it->expression->u.constant->tag == CONST_STRING){

							b_push_const_string(it->expression->u.constant->u.str);
							b_load_arg(TYPTR);
						}



					}

					it = it->next ;
				}
			}

			b_funcall_by_name(  fun->u.fun.funName ,  ty_query(object) ) ;

		}


		else {

			b_funcall_by_name(  fun->u.fun.funName ,  ty_query(object) ) ;

		}




	}




}

///////       End function Evaluation ..S

int tree_get_size_of_element ( TYPE element ){

	INDEX_LIST indices ;
	TYPE nextelements ;
	TYPE subelements ;
    long low , high ;

	if(checkSimple( element) != 0){
		return checkSimple(element);
	}

	else if ( ty_query( element) == TYARRAY ){
		nextelements  = ty_query_array (  element , &indices);

		if(  ty_query(indices ->type)  ==  TYSUBRANGE ){
			subelements =  ty_query_subrange( indices ->type , & low , & high) ;
			return tree_get_size_of_element(nextelements)*( high - low + 1);
		}

	}

    return 0;

}

void tree_evaluate_exp_array( ENODE_P arr ){

	   INDEX_LIST indices ;
	   TYPE elements ;
	   TYPE subelements ;
	   long low , high ;
	   unsigned int size ;

	   if( ty_query(arr->u.arr.arrayFront->type) != TYARRAY){
		   error("Nonarray in array access expression");
		   flag = TRUE;
		   return;

	   }
	   tree_evaluate_exp(arr->u.arr.arrayFront) ;

	   if( TYSIGNEDLONGINT != ty_query( arr->u.arr.arrayIndex->type)){

		   error("Incompatible index type in array access");
		   return ;
	   }

	   tree_evaluate_exp( arr->u.arr.arrayIndex );
	   if(arr->u.arr.arrayIndex->isLvalue == TRUE){
		   b_deref(ty_query(arr->u.arr.arrayIndex->type));
	   }

	   elements  = ty_query_array ( arr->u.arr.arrayFront->type , &indices);

	   if(  ty_query(indices ->type)  ==  TYSUBRANGE ){

		   subelements =  ty_query_subrange( indices ->type , & low , & high) ;
		   b_push_const_int(low);
		   b_arith_rel_op( B_SUB ,  TYSIGNEDLONGINT );
		   size  = tree_get_size_of_element(elements);
		   b_ptr_arith_op(B_ADD , TYSIGNEDLONGINT , size );

	   }

}

/////////  	   Expression Definition Start      /////////////


void tree_evaluate_exp( ENODE_P exp){

	if(exp == NULL) return ;

	TYPETAG lefttype , righttype ;
	TYPE object ;
	ST_ID id ;
	/*
	 * ........Unary Operator Evaluation
	 */

	if( exp->tag == fun_exp ){


		tree_evaluate_exp_fun( exp );
		return ;


	}

	if ( exp->tag == array_exp){

		tree_evaluate_exp_array(exp);
		return ;
	}

	if(exp->tag == unop){

		   if(exp->u.unop.operand->tag == const_exp &&
				   exp->u.unop.operand->u.constant->tag == CONST_NUMBER
				   && exp->u.unop.op == UN_MINUS){

			   if(exp->u.unop.operand->u.constant->u.number->tag == NUMBER_REAL){
				   b_push_const_double( -1* exp->u.unop.operand->u.constant->u.number->u.realval);
			   }
			   else b_push_const_int( -1*exp->u.unop.operand->u.constant->u.number->u.intval);

		   }

		   else {
						tree_evaluate_exp(exp->u.unop.operand) ;

						if( exp->u.unop.operand->isLvalue == TRUE) {
							  b_deref(ty_query(exp->u.unop.operand->type));
						}


						if( TYSIGNEDLONGINT == ty_query(exp->u.unop.operand->type)||
								TYSIGNEDCHAR == ty_query(exp->u.unop.operand->type)||
									TYUNSIGNEDCHAR == ty_query(exp->u.unop.operand->type)) {

							if(exp->u.unop.op == UN_CHR){
								//////msg("inside un_chr");
								if(ty_query(exp->u.unop.operand->type)!= TYUNSIGNEDCHAR ){
									b_convert(ty_query(exp->u.unop.operand->type) , TYUNSIGNEDCHAR );
								}
							}

							else if(exp->u.unop.op == UN_ORD){
								if(ty_query(exp->u.unop.operand->type)!= TYSIGNEDLONGINT ){
									b_convert(ty_query(exp->u.unop.operand->type) , TYSIGNEDLONGINT );
								}

							}

							else if(exp->u.unop.op == UN_SUCC){
								//////msg("inside un_succ");
								if(ty_query(exp->u.unop.operand->type)!= TYSIGNEDLONGINT){
									//b_deref(ty_query(exp->u.unop.operand->type));
									b_convert(ty_query(exp->u.unop.operand->type) , TYSIGNEDLONGINT );
									b_push_const_int(1);
									b_arith_rel_op(B_ADD , TYSIGNEDLONGINT );
									b_convert(TYSIGNEDLONGINT ,ty_query(exp->u.unop.operand->type) );
								}

								else{
									b_push_const_int(1);
									b_arith_rel_op(B_ADD , TYSIGNEDLONGINT );
								}
							}

							else if(exp->u.unop.op == UN_PRED ){
								if(ty_query(exp->u.unop.operand->type)!= TYSIGNEDLONGINT ){
									//b_deref(ty_query(exp->u.unop.operand->type));
									b_convert(ty_query(exp->u.unop.operand->type) , TYSIGNEDLONGINT );
									b_push_const_int(-1);
									b_arith_rel_op(B_ADD , TYSIGNEDLONGINT );
									b_convert(TYSIGNEDLONGINT ,ty_query(exp->u.unop.operand->type) );
								}
							}
						}

						else{
							// not ordinal type ....
						}


						if( exp->u.unop.op == UN_MINUS){
							if(ty_query(exp->u.unop.operand->type) == TYFLOAT ){
								 b_convert( TYFLOAT , TYDOUBLE);
								 b_negate(TYDOUBLE);
							}
							else if(ty_query(exp->u.unop.operand->type)== TYSIGNEDLONGINT ||
									ty_query(exp->u.unop.operand->type)== TYDOUBLE){
									 b_negate(ty_query(exp->u.unop.operand->type));
							}
							else{

								// Illegal type argument to unary minus

							}

						}

						else if(exp->u.unop.op == UN_PLUS){
							if(ty_query(exp->u.unop.operand->type) == TYFLOAT ){
								 b_convert( TYFLOAT , TYDOUBLE);
							}
						}


						else if( exp->u.unop.op == UN_CARET ){


							object = ty_query_ptr( exp->u.unop.operand ->type , &id ) ;
							b_deref( ty_query( object)) ;

							if(ty_query(object) == TYFLOAT ){
									b_convert( TYFLOAT , TYDOUBLE);
							}

						}

		   }

	}


	/*
	 * ........Binary Operator Evaluation...
	 */

	else if(exp->tag == binop){


		// Simulate Unary Conversion

		lefttype  =  ty_query(exp->u.binop.loperand->type) ;
		righttype =  ty_query(exp->u.binop.roperand->type) ;

		if(TYFLOAT == lefttype)  { lefttype =  TYDOUBLE ;  }
		if(TYFLOAT == righttype) { righttype =  TYDOUBLE ; }


		// Process the left operand first
		tree_evaluate_exp(exp->u.binop.loperand) ;

		if( exp->u.binop.loperand->isLvalue == TRUE){
			b_deref(ty_query(exp->u.binop.loperand->type));
		}

		if( TYFLOAT == ty_query(exp->u.binop.loperand->type)){
			b_convert(TYFLOAT , TYDOUBLE) ;
		}

		if( lefttype == TYSIGNEDLONGINT && righttype == TYDOUBLE){
					b_convert(TYSIGNEDLONGINT , TYDOUBLE ) ;
					lefttype = TYDOUBLE ;
		}


		if( lefttype == righttype){

			if(exp->u.binop.op ==  B_LT || exp->u.binop.op ==  B_LE || exp->u.binop.op ==  B_GT ||
					exp->u.binop.op ==  B_GE ||  exp->u.binop.op ==  B_EQ ||
					exp->u.binop.op ==  B_NE) {


				if( TYUNSIGNEDCHAR == ty_query(exp->u.binop.loperand->type)){
									b_convert(TYUNSIGNEDCHAR , TYSIGNEDLONGINT) ;

				}

				if( TYSIGNEDCHAR == ty_query(exp->u.binop.loperand->type)){
									b_convert(TYSIGNEDCHAR , TYSIGNEDLONGINT) ;

				}

			}

		}

		/*
		if( TYUNSIGNEDCHAR == ty_query(exp->u.binop.loperand->type)){
					b_convert(TYUNSIGNEDCHAR , TYSIGNEDLONGINT) ;
					lefttype =  TYSIGNEDLONGINT ;
		}

		if( TYSIGNEDCHAR == ty_query(exp->u.binop.loperand->type)){
							b_convert(TYSIGNEDCHAR , TYSIGNEDLONGINT) ;
							lefttype =  TYSIGNEDLONGINT ;
		}



		if( lefttype == TYSIGNEDLONGINT && righttype == TYDOUBLE){
			b_convert(TYSIGNEDLONGINT , TYDOUBLE ) ;
			lefttype = TYDOUBLE ;
		}

		*/


		// Process the right operand next

		if( exp->u.binop.roperand->tag == const_exp
			&&lefttype == TYDOUBLE && exp->u.binop.roperand->u.constant->tag == CONST_NUMBER &&
				 exp->u.binop.roperand->u.constant->u.number->tag == NUMBER_INT ){

			b_push_const_double((double) exp->u.binop.roperand->u.constant->u.number->u.intval );
			righttype = TYDOUBLE ;


		}

		else {
						tree_evaluate_exp(exp->u.binop.roperand) ;


						if( exp->u.binop.roperand->isLvalue == TRUE){
							b_deref(ty_query(exp->u.binop.roperand->type));
						}

						if( TYFLOAT == ty_query(exp->u.binop.roperand->type)){
							b_convert(TYFLOAT , TYDOUBLE) ;
						}

						if( righttype == TYSIGNEDLONGINT && lefttype == TYDOUBLE){
									b_convert(TYSIGNEDLONGINT , TYDOUBLE ) ;
									righttype= TYDOUBLE ;
						}

						if( lefttype == righttype){

									if(exp->u.binop.op ==  B_LT || exp->u.binop.op ==  B_LE || exp->u.binop.op ==  B_GT ||
											exp->u.binop.op ==  B_GE ||  exp->u.binop.op ==  B_EQ ||
											exp->u.binop.op ==  B_NE) {


													if( TYUNSIGNEDCHAR == ty_query(exp->u.binop.roperand->type)){
																			b_convert(TYUNSIGNEDCHAR , TYSIGNEDLONGINT) ;
																			lefttype = righttype =  TYSIGNEDLONGINT ;

													}

													if( TYSIGNEDCHAR == ty_query(exp->u.binop.roperand->type)){
																			b_convert(TYSIGNEDCHAR , TYSIGNEDLONGINT) ;
																			lefttype =righttype =  TYSIGNEDLONGINT ;

												 }

										}

							}

		}
		/*

		if( TYUNSIGNEDCHAR == ty_query(exp->u.binop.roperand->type)){
			b_convert(TYUNSIGNEDCHAR , TYSIGNEDLONGINT) ;
			righttype =  TYSIGNEDLONGINT ;
		}

		if( TYSIGNEDCHAR == ty_query(exp->u.binop.roperand->type)){
				b_convert(TYSIGNEDCHAR , TYSIGNEDLONGINT) ;
				lefttype =  TYSIGNEDLONGINT ;
		}


		if( righttype == TYSIGNEDLONGINT && lefttype == TYDOUBLE){
			b_convert(TYSIGNEDLONGINT , TYDOUBLE ) ;
			righttype= TYDOUBLE ;
		}

        */
		if( lefttype == righttype){

			if(exp->u.binop.op ==  B_LT || exp->u.binop.op ==  B_LE || exp->u.binop.op ==  B_GT ||
										exp->u.binop.op ==  B_GE ||  exp->u.binop.op ==  B_EQ ||
										exp->u.binop.op ==  B_NE) {

				b_arith_rel_op(exp->u.binop.op , righttype);
				b_convert( TYSIGNEDLONGINT ,  TYSIGNEDCHAR );

			}
			else {
					b_arith_rel_op(exp->u.binop.op , righttype);
			}
		}
		else{

			//// some error ....

		}


	}


	/*
	 *  ........Variable Evaluation.........
	 */

	else if(exp->tag == var_exp){
		    int block ;

		    ////msg("var check in eval");
		    ////msg(exp->u.var);
		    ////msg("end var check in eval");
			ST_ID id = st_enter_id( exp->u.var ) ;
			ST_DR dr = st_lookup ( id  , &block  ) ;

			if(dr == NULL){
				/// nothing declared before
			}

			else if( dr->tag == GDECL){
			  b_push_ext_addr( exp->u.var) ;
			}

	}

	/*
	 *
	 * ..........Constant Evaluation .......
	 *
	 */

	else if(exp->tag == const_exp){

		////msg("inside const ") ;
		if(exp->u.constant->tag == CONST_NUMBER ){

			if(exp->u.constant->u.number->tag == NUMBER_REAL){
				////msg("real number paise ") ;
				//////msg("%lf" ,  exp->u.constant->u.number->u.realval) ;
				b_push_const_double(exp->u.constant->u.number->u.realval);
			}

			else if(exp->u.constant->u.number->tag == NUMBER_INT){

				////msg( "constant paise ") ;
				b_push_const_int(exp->u.constant->u.number->u.intval);
			}

		}

		else if(exp->u.constant->tag == CONST_BOOL){


			if(exp->u.constant->u.boole == FALSE){
				b_push_const_int(0);
				b_convert( TYSIGNEDLONGINT ,TYSIGNEDCHAR );
			}

			else if(exp->u.constant->u.boole == TRUE){
				b_push_const_int(1);
				b_convert( TYSIGNEDLONGINT ,TYSIGNEDCHAR );
			}

		}

		else if(  exp->u.constant->tag  == CONST_STRING ){
				//
			    b_push_const_int( *(exp->u.constant->u.str)) ;
			    b_convert( TYSIGNEDLONGINT ,TYUNSIGNEDCHAR );

			//
		}

	}
}

ENODE_P tree_make_exp_array ( ENODE_P arrayFirst, ENODE_P arrayIndex ){

	ENODE_P node = (ENODE*)malloc(sizeof(ENODE)) ;
	INDEX_LIST indices ;

	node->tag = array_exp ;
	node->u.arr.arrayFront = arrayFirst ;
	node->u.arr.arrayIndex = arrayIndex ;
	node->isLvalue = TRUE ;

	if(arrayFirst ->tag == array_exp ){

		node->type = ty_query_array( arrayFirst->type, &indices) ;

	}

	else if ( arrayFirst ->tag == var_exp && TYARRAY == ty_query(arrayFirst->type)){

		node->type = ty_query_array( arrayFirst->type, &indices) ;
	}

	else {
		node->type = ty_build_basic(TYERROR);
	}


	return node ;
}

ENODE_P tree_make_exp_unop ( unoptype op , ENODE_P operand ){

	if(operand == NULL) return NULL ;
	ENODE_P node = (ENODE*)malloc(sizeof(ENODE)) ;
    TYPE objtype ;
    ST_ID  id ;

	node->tag = unop ;
	node->isLvalue = FALSE ;


	if(TYFLOAT ==  ty_query( operand->type)){
		node->type = ty_build_basic(TYDOUBLE) ;
	}

	else{

		if( op ==  UN_CHR ){
			node->type = ty_build_basic( TYUNSIGNEDCHAR ) ;
		}

		else if( op == UN_ORD){
			node->type = ty_build_basic(TYSIGNEDLONGINT);

		}
		else if( op == UN_CARET){

			if(ty_query(operand->type) == TYPTR ){
				objtype = ty_query_ptr(operand->type , &id );
				// resolved naki check kora jaite pare...
				node->type = objtype ;


			}

		}
		else
		node->type = operand->type ;
	}

	node->u.unop.op = op ;
	node->u.unop.operand = operand ;

	return node ;
}




ENODE_P tree_make_exp_binop( B_ARITH_REL_OP op , ENODE_P loperand  , ENODE_P roperand ){

	if( loperand == NULL || roperand == NULL){
		return NULL;
	}
	ENODE_P node = (ENODE*)malloc(sizeof(ENODE)) ;
	TYPETAG lefttype , righttype ;
	node->tag = binop ;
	node->isLvalue = FALSE ;


	// Setting the type during binary operation ...
	lefttype  =  ty_query(loperand->type) ;
	righttype =  ty_query(roperand->type) ;

	if(TYFLOAT == lefttype)  { lefttype =  TYDOUBLE ;  }
	if(TYFLOAT == righttype) { righttype =  TYDOUBLE ; }

	/*

	if(TYUNSIGNEDCHAR == lefttype)  { lefttype =  TYSIGNEDLONGINT ;  }
    if(TYUNSIGNEDCHAR == righttype) { righttype =  TYSIGNEDLONGINT ; }
    if(TYSIGNEDCHAR == lefttype)  { lefttype =  TYSIGNEDLONGINT ;  }
    if(TYSIGNEDCHAR == righttype) { righttype =  TYSIGNEDLONGINT ; }

   */

	if( lefttype == TYSIGNEDLONGINT && righttype == TYDOUBLE){
			lefttype = TYDOUBLE ;
	}

	if( righttype == TYSIGNEDLONGINT && lefttype == TYDOUBLE){
			righttype= TYDOUBLE ;
	}


	if( lefttype == righttype){


		if( op == B_LT || op == B_LE || op == B_GT || op == B_GE ||
				op == B_EQ || op == B_NE  ) {

			node->type = ty_build_basic(TYSIGNEDCHAR);
		}


		else {node->type = ty_build_basic(righttype);}



	}



	else {

		// Print Type Mismatch type er kisu akta ....
		if( op == B_LT || op == B_LE || op == B_GT || op == B_GE ||
		op == B_EQ || op == B_NE  ) {

		error("Incompatible type arguments to comparison operator");
		}


		else { error("Nonnumerical type argument(s) to arithmetic operation"); }

		return NULL;

	}

	node->u.binop.op = op ;
	node->u.binop.loperand = loperand ;
	node->u.binop.roperand = roperand ;

	return node ;
}




ENODE_P tree_make_exp_add_actual_params( ENODE_P name , EXPR_LIST_P paramList){

	if( name->tag == fun_exp ){

		name->u.fun.actualParams = paramList ;

		////msg("actual param paise");

		if(paramList->expression->tag == const_exp ){
			if( paramList->expression->u.constant->tag == CONST_STRING){
				////msg("a") ;
				////msg( paramList->expression->u.constant->u.str);
			}
		}
	}


	////msg("actual param paise");
	return name ;
}

ENODE_P s( char * varname ){

	int block ;
	TYPE object ;
	PARAM_LIST params;
	BOOLEAN check_args ;


	ENODE_P node = (ENODE*)malloc(sizeof(ENODE)) ;
	TYPETAG fun_or_var ;

	////msg("var begin");





	ST_ID id = st_enter_id( varname ) ;
	ST_DR dr = st_lookup ( id  , &block  ) ;

	////msg("var begin 1");
	if( dr == NULL){

		////msg("var begin 1");
		// print variable not found error

	}

	if( dr->tag == GDECL){

        node->type = dr->u.decl.type ;
        fun_or_var = ty_query( dr->u.decl.type) ;

	}


	if(fun_or_var == TYFUNC ){

		////msg("var begin 1");
		object = ty_query_func( dr->u.decl.type , &params , &check_args);
		node->type = object ;
		node->tag = fun_exp ;
		node->u.fun.funName = (char*) malloc(  sizeof(char)* (strlen(varname) + 1)) ;
		strcpy( node->u.fun.funName , varname ) ;
		node->u.fun.actualParams = NULL  ;
		// is_ref not set yet ..

		/*
		node->u.funParams.funName = (char*) malloc(  sizeof(char)* (strlen(varname) + 1)) ;
		strcpy( node->u.var , varname ) ;

		*/
		return node ;
	}

	// this part is for varible ....

	node->tag = var_exp ;
	node->isLvalue = TRUE ;

	node->u.var = (char*) malloc(  sizeof(char)* (strlen(varname) + 1)) ;
	strcpy( node->u.var , varname ) ;

	//return tree_make_exp_unop(UN_PLUS , node) ;

	return node ;
}

ENODE_P tree_make_exp_identifier( char * varname ){

	int block ;
	TYPE object ;
	PARAM_LIST params;
	BOOLEAN check_args ;
	INDEX_LIST indices ;

	ENODE_P node = (ENODE*)malloc(sizeof(ENODE)) ;
	TYPETAG fun_or_var ;

	////msg("var begin");





	ST_ID id = st_enter_id( varname ) ;
	ST_DR dr = st_lookup ( id  , &block  ) ;

	////msg("var begin 1");
	if( dr == NULL){

		error("Undeclared identifier \"%s\" in expression", varname) ;
		return NULL ;
		//////msg("var begin 1");
		// print variable not found error

	}

	if( dr->tag == GDECL ||  dr->tag  == FDECL){

        node->type = dr->u.decl.type ;
        fun_or_var = ty_query( dr->u.decl.type) ;

	}


	if(fun_or_var == TYFUNC ){

		////msg("var begin 5");
		object = ty_query_func( dr->u.decl.type , &params , &check_args);
		node->type = object ;
		node->tag = fun_exp ;
		node->u.fun.funName = (char*) malloc(  sizeof(char)* (strlen(varname) + 1)) ;
		strcpy( node->u.fun.funName , varname ) ;
		node->u.fun.actualParams = NULL  ;
		// is_ref not set yet ..

		/*
		node->u.funParams.funName = (char*) malloc(  sizeof(char)* (strlen(varname) + 1)) ;
		strcpy( node->u.var , varname ) ;

		*/
		return node ;
	}



	// this part is for varible ....

	node->tag = var_exp ;
	node->isLvalue = TRUE ;

	node->u.var = (char*) malloc(  sizeof(char)* (strlen(varname) + 1)) ;
	strcpy( node->u.var , varname ) ;

	//return tree_make_exp_unop(UN_PLUS , node) ;

	return node ;
}
ENODE_P tree_make_empty_exp(){
	ENODE_P node = (ENODE*)malloc(sizeof(ENODE)) ;
	node->tag = empty_exp ;
	return node;

}
ENODE_P tree_make_exp_const( CONSTANT_P constant){
	ENODE_P node = (ENODE*)malloc(sizeof(ENODE)) ;

	if( constant->tag == CONST_NUMBER){
		if(constant->u.number->tag == NUMBER_REAL){
			node->type = ty_build_basic(TYDOUBLE);
		}
		else if( constant->u.number->tag == NUMBER_INT){
			node->type = ty_build_basic(TYSIGNEDLONGINT);
		}
		node->tag = const_exp ;
	    node->isLvalue = FALSE ;
	    node->u.constant = constant ;
	}

	if( constant->tag == CONST_BOOL){

		    node->type = ty_build_basic(TYSIGNEDCHAR) ;
			node->tag = const_exp ;
		    node->isLvalue = FALSE ;
		    node->u.constant = constant ;
	}

	if( constant->tag == CONST_STRING){

		   node->tag = const_exp ;
		   node->u.constant = constant ;
		   node->type = ty_build_basic(TYUNSIGNEDCHAR);

			////msg( constant->u.str) ;
	}






	return node ;
}

/////////  	   Expression Definition End       /////////////


NUMBER_P tree_make_number_from_int ( int num ){

	NUMBER_P node = (NUMBER*)malloc(sizeof(NUMBER)) ;

	node->tag = NUMBER_INT ;
	node->u.intval = num ;
	////msg( " int theke number hoise") ;
	return node ;
}
NUMBER_P tree_make_number_from_real(double num ){

	NUMBER_P node = (NUMBER*)malloc(sizeof(NUMBER)) ;

		node->tag = NUMBER_REAL;
		node->u.realval = num ;

	return node ;

}

NUMBER_P tree_make_number_from_sign( char sign , NUMBER_P number){

	////msg("number check kortese");
	if(sign== '-'){
		NUMBER_P node = (NUMBER*)malloc(sizeof(NUMBER)) ;
		node->tag = number->tag ;

		if(node->tag == NUMBER_INT){
			node->u.intval = -1* number->u.intval ;
		;
		}
		else if(node->tag == NUMBER_REAL){
			node->u.realval = -1* number->u.realval ;
		}

		return node ;
	}
	else{
		return number ;
	}
}
CONSTANT_P tree_make_constant_from_string ( char * string){

	CONSTANT_P node = (CONSTANT*)malloc(sizeof(CONSTANT)) ;

	node->tag = CONST_STRING ;
	node->u.str = string ;

	// pore lagle abar kore nibo ...
	//node->u.str = (char*) malloc( strlen(string) + 1 ) ;
	//strcpy( node->u.str , string );

	////msg("Here am I") ;
	////msg(node->u.str) ;
	return node ;
}

CONSTANT_P tree_make_constant_from_boolean( char * boole ){
	CONSTANT_P node = (CONSTANT*)malloc(sizeof(CONSTANT)) ;


	if(strcmp(boole , "False")==0 || strcmp(boole , "false")==0 ){
		node->tag = CONST_BOOL ;
		node->u.boole = FALSE ;
	}
	else if(strcmp(boole , "True")==0 || strcmp(boole , "true")==0){

		node->tag = CONST_BOOL ;
		node->u.boole = TRUE ;
	}



	return node ;

}

CONSTANT_P tree_make_constant_from_number( NUMBER_P number ){
	////msg( " number theke const dhukse ") ;
	CONSTANT_P node = (CONSTANT*)malloc(sizeof(CONSTANT)) ;



	////msg( " number theke const dhukse ") ;
	node->tag = CONST_NUMBER ;


	node->u.number = number ;
	////msg( " number theke const hoise ") ;
	return node ;
}


////////// Project 2 ////////////


ENODE_P tree_make_exp_from_unsigned_number( NUMBER_P number ){

	 CONSTANT_P cons= tree_make_constant_from_number(number);
	 ENODE_P node = tree_make_exp_const(cons);
     return node ;
}






void tree_init_exitLabelList(){
	exitLabelList = NULL ;
	flag = FALSE;
}

void tree_add_exitLabeltoList( char * str ){

	if(exitLabelList == NULL){
		exitLabelList =  (CLIST*)malloc(sizeof(CLIST)) ;
		exitLabelList->next = NULL ;
		exitLabelList->str = str ;
		return ;
	}

	CLIST_P node = (CLIST*)malloc(sizeof(CLIST)) ;
	node->str = str ;
	node->next = exitLabelList ;
	exitLabelList = node ;
}

void tree_remove_exitLabeltoList(){

	if(exitLabelList != NULL)
		exitLabelList = exitLabelList->next ;

}

void tree_handle_break_statement(){


	if (  exitLabelList == NULL){
		error("Break statement not inside loop");
		return ;
	}

	//msg("break statement handle") ;

	b_jump(exitLabelList->str);

}


/// Project I /////

PARAM_LIST tree_merge_paramlist( PARAM_LIST  list_1  ,  PARAM_LIST list_2){
	////////msg("In tree_merge_paramlist\n");
	PARAM_LIST it = list_1 ;


	while( it->next!=NULL) it = it->next ;
	it->next = list_2 ;

	return list_1 ;
}

TYPE tree_build_func(TYPE object, PARAM_LIST params , int check ){
	//////msg("In tree_build_func\n");
	PARAM_LIST it = params ;
	PARAM_LIST it2 ;
	/*
	 * Duplicate parameter checking
	 *
	 */

	while(it!=NULL){

		it2 = it->next ;

		while(it2 !=NULL){


			if(strcmp( st_get_id_str(it2->id), st_get_id_str(it-> id))==0 ){
				error("Duplicate parameter name: \"%s\"", st_get_id_str(it-> id));

			}
			it2 = it2->next ;
		}

		it = it->next ;
	}
	if(object!= NULL && checkSimple(object)==0){
		error("Function return type must be simple type") ;
		//return ty_build_basic(TYERROR);
	}

	if(object==NULL){
		object = ty_build_basic(TYVOID);
	}
	if(check == 1){
	   return ty_build_func(object , params, TRUE );
	}

	else{
		 return ty_build_func(object , params, FALSE );
	}

}

PARAM_LIST tree_paramlist_from_idlists(MEMBER_LIST idlist , TYPE tname , BOOLEAN isref ){
	//////msg("In tree_paramlist_from_idlists\n");
	PARAM_LIST param_head = (PARAM *)malloc(sizeof(PARAM)) ;
	PARAM_LIST node ;
	MEMBER_LIST it = idlist  ;
	PARAM_LIST it2 ;
	if(checkSimple(tname)==0){
		error("Parameter type must be a simple type");
	}
	int flag = 0;
	while(it!=NULL){
		if(flag == 0){
			flag = 1 ;
			param_head = (PARAM *)malloc(sizeof(PARAM));
			param_head ->id = it->id ;
			param_head ->type =tname ;
			param_head ->is_ref = isref ;
			param_head ->sc = NO_SC ;
			param_head ->err = it->err ;
		}

		else{
			node = (PARAM *)malloc(sizeof(PARAM));
			node ->id = it->id ;
			node ->type = tname ;
			node ->is_ref = isref ;
			node ->sc = NO_SC ;
			node ->err = it->err ;

			node->next = param_head;
			param_head = node ;
			/*
			it2 = param_head ;
			while (it2->next != NULL) it2 = it2->next ;

			it2->next = node ;
			*/
		}

		it = it->next ;
	}

	return param_head ;
}

void tree_install_program_name(char *str){
	ST_ID myST_ID = st_enter_id(str);
}

void tree_install_new_type( char *str , TYPE myType){

	//msg( " here" );
	ST_DR myST_DR = stdr_alloc() ;
	ST_ID myST_ID = st_enter_id(str);

	myST_DR->tag = TYPENAME ;
	myST_DR->u.typename.type = myType;

	BOOLEAN abool =  st_install( myST_ID, myST_DR);

	if(abool== FALSE ){
		error("Duplicate variable declaration: \"%s\"", str);
	}
	//////msg("In tree_install_new_type\n");

}

void tree_install_new_decl( char *str , TYPE myType){
	ST_DR myST_DR = stdr_alloc() ;
	ST_ID myST_ID = st_enter_id(str);

	myST_DR->tag = GDECL ;
	myST_DR->u.decl.type = myType;

	BOOLEAN abool =  st_install( myST_ID, myST_DR);

	if(abool== FALSE ){
		error("Duplicate variable declaration: \"%s\"", str);
	}
	//////msg("In tree_install_new_decl\n");

}

void tree_array_head_init(){
	////msg("In tree array head init");
	array_head = (INDEX *)malloc(sizeof(INDEX));
	array_head->next = NULL ;
	////msg("In tree array head finish");
	//////msg("In tree_array_head_init\n");
}


INDEX_LIST tree_insert_array_head( TYPE myPtr ){

	////msg("inside array head");

	if(myPtr == NULL){
		return array_head ;
	}
	INDEX_LIST node ;
	node  = (INDEX *)malloc(sizeof(INDEX));
	node->type = myPtr ;
	node->next = NULL;
	INDEX_LIST it = array_head;


	while(it->next != NULL){
		it = it->next;
	}
	it->next = node ;

	return array_head ;
	//////msg("In tree_insert_array_head\n");
}

TYPE tree_build_array ( TYPE object , INDEX_LIST indices){

	////msg("In tree_build_array\n");
	if(TYERROR== ty_query(object)){
		error("Data type expected for array elements");
		return ty_build_basic(TYERROR);
	}
	else if(object!=NULL && TYFUNC ==ty_query(object)){
		error("Data type expected for array elements");
		return ty_build_basic(TYERROR);
	}

	return ty_build_array(object , indices->next );
	//////msg("In tree_build_array\n");
}
void tree_unresolvedPtrList_init(){

	hd = (TLIST_NODE *)malloc(sizeof(TLIST_NODE));
	hd->next = NULL ;
	//////msg("In tree_unresolvedPtrList_init\n");
}

void tree_resolve_unresolved_pointers(){

	//////msg("In tree_resolve_unresolved_pointers1\n");
	TYPE_LIST loop = NULL;
	if(hd!=NULL) loop = hd->next ;
	while(loop!=NULL){

		TYPE objtype ;
		int block ;
		ST_ID id ;

		objtype = ty_query_ptr(loop->type , &id);
		ST_DR dataRecord = st_lookup( id, &block);
		if ( dataRecord== FALSE)
			error("Unresolved type name: \"%s\"",st_get_id_str(id));
		else
			ty_resolve_ptr(loop->type , dataRecord->u.typename.type);

		loop = loop->next ;
	}
	//////msg("In tree_resolve_unresolved_pointers2\n");

}

TYPE tree_build_unresolved_ptr( char * new_identifier){
	void *myST_ID = st_enter_id( new_identifier ) ;
	TYPE ret_type = ty_build_unresolved_ptr(myST_ID);
	tree_insert_unresolved_ptr(ret_type);
	//////msg("In tree_build_unresolved_ptr\n");
	return ret_type ;
}

void tree_insert_unresolved_ptr( TYPE myPtr ){

	TYPE_LIST node ;
	node  = (TLIST_NODE *)malloc(sizeof(TLIST_NODE));
	node->type = myPtr ;
	node->next = NULL;

	if(hd->next == NULL){
		hd->next = node ;
	}

	else{
		node->next = hd->next ;
		hd->next = node ;
	}
	//////msg("In tree_insert_unresolved_ptr\n");
}

TYPE tree_build_basic( char *  myTypeTag ){


	void * myST_ID  = st_enter_id( myTypeTag ) ;
	int block  = 0 ;
	ST_DR datarecord ;
	datarecord  = st_lookup(myST_ID, &block);
	if(datarecord == NULL){
		error("Undeclared type name: \"%s\"" , myTypeTag) ;
		return ty_build_basic(TYERROR);
	}
	else{
		if( datarecord->tag  ==  TYPENAME ){

			return datarecord->u.typename.type ;

		}
		else if( datarecord->tag  ==  GDECL ){

			return datarecord->u.decl.type ;

		}
	}
	//////msg("In tree_build_basic\n");
	return NULL ;
}

TYPE tree_build_subrange( CONSTANT_P llow , CONSTANT_P lhigh ){

	//msg("In build subrange");

	TYPE basic , subrange ;

	long low , high ;
	low  = llow->u.number->u.intval ;
	high = lhigh->u.number->u.intval ;

	basic  = ty_build_basic(TYSIGNEDLONGINT);
	if( low > high){
		error("Empty subrange in array index");
		error("Illegal index type (ignored)");
	}

	else{
		subrange = ty_build_subrange(basic , low , high);
		//msg("In tree_build_subrange finish \n");
		return subrange ;
	}

	return NULL ;

}



void tree_var_head_init(){
	var_head = (MEMBER *)malloc(sizeof(MEMBER));
    var_head->next = NULL ;
}

MEMBER_LIST tree_insert_var_head( MEMBER_LIST var_hd , char * varname ){

	ST_ID myST_ID = st_enter_id(varname);
	MEMBER_LIST  node ;
	node = (MEMBER *)malloc(sizeof(MEMBER));
	//////msg("In tree_insert_var_head\n");

	if(var_hd == NULL){
		var_hd = (MEMBER *)malloc(sizeof(MEMBER));
		var_hd->id = myST_ID ;
		var_hd->next = NULL;

		return var_hd ;

	}

	node->id = myST_ID ;
	node->next = var_hd;

	/*
	if(var_hd->next == NULL){
		var_hd->next = node ;
		//////msg("In tree_insert_var_head1\n");
	}

	else{
		//////msg("In tree_insert_var_head2\n");
		node->next = var_hd->next ;
		var_hd->next = node ;
	}
     */
	return node ;


}

void tree_install_variables( MEMBER_LIST list , TYPE vartype ){

	MEMBER_LIST it = list;

	//error("In tree_install_variables\n");
	tree_resolve_unresolved_pointers();
	tree_unresolvedPtrList_init();
	////msg("In tree_install_variables\n");
	while( it!=NULL){

		ST_DR myST_DR = stdr_alloc() ;
		myST_DR->tag = GDECL ;
		myST_DR->u.decl.type = vartype;
		myST_DR->u.decl.sc = NO_SC;
		if( TYERROR == ty_query(vartype)) 	myST_DR->u.decl.err = TRUE;
		else myST_DR->u.decl.err = FALSE;
		myST_DR->u.decl.is_ref = FALSE;
		BOOLEAN abool =  st_install( it->id, myST_DR);
		if(abool== FALSE ){
				error("Duplicate variable declaration: \"%s\"", st_get_id_str(it->id));
		}

		else if(checkBasic( vartype )== 0){

			error("Variable(s) must be of data type");
		}



		else{
			int size = calculateSize(vartype);
			back_end_call(st_get_id_str(it->id), calculateAlignment(vartype), size);
		}

		it = it->next;

	}


}

int checkSimple( TYPE tp ){

	int tag = ty_query(tp);


	if(tag == TYSIGNEDLONGINT){

		return 4;
	}
	else if( tag == TYSIGNEDINT){
		return 4;
	}
	else if(tag == TYDOUBLE){
		return 8;
	}
	else if(tag == TYLONGDOUBLE){
		return 10; // size of Extended = 10 Byte
		}
	else if(tag == TYFLOAT){
		return 4;
		}
	else if(tag == TYSIGNEDSHORTINT){
		return 2;	// size of Word = 2 Byte
		}
	else if(tag == TYSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYUNSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYPTR){
			return 4;  //size of any pointer
		}


	return 0 ;
}
int checkBasic( TYPE tp ){

	int tag = ty_query(tp);

	if(tag == TYSIGNEDLONGINT){

		return 4;
	}
	else if( tag == TYSIGNEDINT){
		return 4;
	}
	else if(tag == TYDOUBLE){
		return 8;
	}
	else if(tag == TYLONGDOUBLE){
		return 10; // size of Extended = 10 Byte
		}
	else if(tag == TYFLOAT){
		return 4;
		}
	else if(tag == TYSIGNEDSHORTINT){
		return 2;	// size of Word = 2 Byte
		}
	else if(tag == TYSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYUNSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYPTR){
			return 4;  //size of any pointer
		}
	else if(tag == TYARRAY) {
		return 5 ;
	}
	else if(tag == TYSUBRANGE){
		return 5;
	}

	return 0 ;
}


int calculateAlignment( TYPE myType ){

	//////////msg("In calculateAlignment\n");
	int element_type ;
	long  low , high;
	int tag = ty_query(myType);

	//////////msg("Subrange Allignment sdfgds %d" , tag );
/*
    {"Integer", TYSIGNEDLONGINT},
    {"Real", TYDOUBLE},
    {"Extended", TYLONGDOUBLE},
    {"Single", TYFLOAT},
    {"Word", TYSIGNEDSHORTINT},
    {"Byte", TYSIGNEDCHAR},
    {"Char", TYUNSIGNEDCHAR},
    {"Boolean", TYSIGNEDCHAR},
	*/
	if(tag == TYSIGNEDLONGINT){

		return 4;
	}
	else if( tag == TYSIGNEDINT){
		return 4;
	}
	else if(tag == TYDOUBLE){
		return 8;
	}
	else if(tag == TYLONGDOUBLE){
		return 10; // size of Extended = 10 Byte
		}
	else if(tag == TYFLOAT){
		return 4;
		}
	else if(tag == TYSIGNEDSHORTINT){
		return 2;	// size of Word = 2 Byte
		}
	else if(tag == TYSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYUNSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYPTR){
			return 4;  //size of any pointer
		}
	else if(tag == TYSUBRANGE){
		TYPE newOBJ ;
		newOBJ = ty_query_subrange(myType,&low, &high);
		//////////msg("Subrange Allignment %d", ty_query(newOBJ));
		int bb = calculateAlignment(newOBJ);
		//////////msg("Subrange Allignment dwdr %d", bb);
		return bb ;
		}
	else if(tag == TYARRAY){
		INDEX_LIST indices , it ;
		TYPE newOBJ ;
		newOBJ = ty_query_array(myType , &indices);
		element_type = calculateAlignment(newOBJ);

		return element_type ;
	}

}


int calculateSize( TYPE myType ){



	int retsize;
	long low , high;
	int tag = ty_query(myType);

	//////msg("In calculateSize, tag = %d\n",tag);
/*
    {"Integer", TYSIGNEDLONGINT},
    {"Real", TYDOUBLE},
    {"Extended", TYLONGDOUBLE},
    {"Single", TYFLOAT},
    {"Word", TYSIGNEDSHORTINT},
    {"Byte", TYSIGNEDCHAR},
    {"Char", TYUNSIGNEDCHAR},
    {"Boolean", TYSIGNEDCHAR},
*/
	if(tag == TYSIGNEDLONGINT){

		return 4;
	}
	else if( tag == TYSIGNEDINT){
			return 4;
		}
	else if(tag == TYDOUBLE){
		return 8;
	}
	else if(tag == TYLONGDOUBLE){
		return 10; // size of Extended = 10 Byte
		}
	else if(tag == TYFLOAT){
		return 4;
		}
	else if(tag == TYSIGNEDSHORTINT){
		return 2;	// size of Word = 2 Byte
		}
	else if(tag == TYSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYUNSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYSIGNEDCHAR){
		return 1;
		}
	else if(tag == TYPTR){
			return 4;  //size of any pointer
		}
	else if(tag == TYSUBRANGE){
			TYPE newOBJ ;
			newOBJ = ty_query_subrange(myType,&low, &high);
			return calculateSize(newOBJ);
			}
	else if(tag == TYARRAY){
		INDEX_LIST indices , it ;
		TYPE newOBJ ;
		newOBJ = ty_query_array(myType , &indices);
		retsize = calculateSize(newOBJ);

		it = indices ;
		while(it != NULL ){
				newOBJ = ty_query_subrange(it->type , &low , &high);
				//////msg("low =%ld high = %ld\n",low, high);
				retsize *=(high-low + 1);
				it = it->next;
		}
		return retsize ;
	}

}

