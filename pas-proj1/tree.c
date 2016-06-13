#include "tree.h"
#include "message.h"
#include <stdlib.h>

PARAM_LIST tree_merge_paramlist( PARAM_LIST  list_1  ,  PARAM_LIST list_2){
	////msg("In tree_merge_paramlist\n");
	PARAM_LIST it = list_1 ;


	while( it->next!=NULL) it = it->next ;
	it->next = list_2 ;

	return list_1 ;
}

TYPE tree_build_func(TYPE object, PARAM_LIST params){
	//msg("In tree_build_func\n");
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
	return ty_build_func(object , params, TRUE );


}

PARAM_LIST tree_paramlist_from_idlists(MEMBER_LIST idlist , TYPE tname , BOOLEAN isref ){
	//msg("In tree_paramlist_from_idlists\n");
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
	ST_DR myST_DR = stdr_alloc() ;
	ST_ID myST_ID = st_enter_id(str);

	myST_DR->tag = TYPENAME ;
	myST_DR->u.typename.type = myType;

	BOOLEAN abool =  st_install( myST_ID, myST_DR);

	if(abool== FALSE ){
		error("Duplicate variable declaration: \"%s\"", str);
	}
	//msg("In tree_install_new_type\n");

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
	//msg("In tree_install_new_decl\n");

}

void tree_array_head_init(){
	array_head = (INDEX *)malloc(sizeof(INDEX));
	array_head->next = NULL ;
	//msg("In tree_array_head_init\n");
}


INDEX_LIST tree_insert_array_head( TYPE myPtr ){

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
	//msg("In tree_insert_array_head\n");
}

TYPE tree_build_array ( TYPE object , INDEX_LIST indices){

	if(TYERROR== ty_query(object)){
		error("Data type expected for array elements");
		return ty_build_basic(TYERROR);
	}
	else if(object!=NULL && TYFUNC ==ty_query(object)){
		error("Data type expected for array elements");
		return ty_build_basic(TYERROR);
	}

	return ty_build_array(object , indices->next );
	//msg("In tree_build_array\n");
}
void tree_unresolvedPtrList_init(){

	hd = (TLIST_NODE *)malloc(sizeof(TLIST_NODE));
	hd->next = NULL ;
	//msg("In tree_unresolvedPtrList_init\n");
}

void tree_resolve_unresolved_pointers(){

	//msg("In tree_resolve_unresolved_pointers1\n");
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
	//msg("In tree_resolve_unresolved_pointers2\n");

}

TYPE tree_build_unresolved_ptr( char * new_identifier){
	void *myST_ID = st_enter_id( new_identifier ) ;
	TYPE ret_type = ty_build_unresolved_ptr(myST_ID);
	tree_insert_unresolved_ptr(ret_type);
	//msg("In tree_build_unresolved_ptr\n");
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
	//msg("In tree_insert_unresolved_ptr\n");
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
	//msg("In tree_build_basic\n");
	return NULL ;
}

TYPE tree_build_subrange( long low , long high ){
	TYPE basic , subrange ;
	basic  = ty_build_basic(TYSIGNEDLONGINT);
	if( low > high){
		error("Empty subrange in array index");
		error("Illegal index type (ignored)");
	}

	else{
		subrange = ty_build_subrange(basic , low , high);
		//msg("In tree_build_subrange\n");
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
	//msg("In tree_insert_var_head\n");

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
		//msg("In tree_insert_var_head1\n");
	}

	else{
		//msg("In tree_insert_var_head2\n");
		node->next = var_hd->next ;
		var_hd->next = node ;
	}
     */
	return node ;


}

void tree_install_variables( MEMBER_LIST list , TYPE vartype){

	MEMBER_LIST it = list;

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

	//////msg("In calculateAlignment\n");
	int element_type ;
	long  low , high;
	int tag = ty_query(myType);

	//////msg("Subrange Allignment sdfgds %d" , tag );
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
		//////msg("Subrange Allignment %d", ty_query(newOBJ));
		int bb = calculateAlignment(newOBJ);
		//////msg("Subrange Allignment dwdr %d", bb);
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

	//msg("In calculateSize, tag = %d\n",tag);
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
				//msg("low =%ld high = %ld\n",low, high);
				retsize *=(high-low + 1);
				it = it->next;
		}
		return retsize ;
	}

}

