/* Dictionary implementation */

#include <stdlib.h>
#include <string.h>
#include "my_defs.h"

#define INITIAL_HASH_SIZE 8
#define MAX_LOAD_FACTOR 2
#define SCALE_FACTOR 2

static DR get_item(const char *key);
static int insert_or_update(DR new_item);
static void mark_cycle(DR item);
static void unmark_cycle(DR item);
static int hash(const char *key);
static void insert_at_front(DR *list, DR new_item);
static DR remove_from_front(DR *list);
static void resize(int size);

int h_size;
int num_items;
DR *hash_tab;

void init_dict()
{
    h_size = 0;
    hash_tab = NULL;
    resize(INITIAL_HASH_SIZE);
    num_items = 0;
}

// Add a key with an integer constant value to the dictionary
void add_int_to_dict(const char *key, long val)
{
    DR p = (DR) malloc(sizeof(DICT_REC));
    p->in_cycle = FALSE;
    p->key = key;
    p->tag = INT_CONST;
    p->u.intconstval = val;
    if (insert_or_update(p) == 0)
        fprintf(stderr, "Warning: redefinition of %s to %ld at line %d\n",
                key, val, line_no);
}

// Add a key with a string constant value to the dictionary
void add_str_to_dict(const char *key, const char *val)
{
        // redacted

	DR p = (DR) malloc(sizeof(DICT_REC));
	    p->in_cycle = FALSE;
	    p->key = key;
	    p->tag = STR_CONST;
	    p->u.strconstval = val ;
	    //strcpy( p->u.strconstval , val );
	    if (insert_or_update(p) == 0)
	        fprintf(stderr, "Warning: redefinition of %s to %s at line %d\n",
	        		key, val, line_no);
}

// Add a key with an identifier value to the dictionary
void add_id_to_dict(const char *key, const char *val)
{
        // redacted

	DR p = (DR) malloc(sizeof(DICT_REC));
		    p->in_cycle = FALSE;
		    p->key = key;
		    p->tag = ID;
		    p->u.idval =  val;
		    //strcpy( p->u.idval , val );
		    if (insert_or_update(p) == 0)
		        fprintf(stderr, "Warning: redefinition of %s to %s at line %d\n",
		        		key, val, line_no);
}

// Output the substituted value of a defined ID to the output stream
void output_substitution(FILE *stream, const char *id)
{
        // redacted
	DR result = get_item( id );
	debug1("%s\n", id );
	if(result==NULL || result->in_cycle == 1){
		//debug1("%s", id );
		fprintf( stream , "%s", id );
		return ;
	}

	while(result->tag == ID && get_item(result->u.idval)!= NULL && result->in_cycle == 0) result = get_item(result->u.idval);

	debug("HERE\n");
	if(result->tag != ID){
		if(result->tag == INT_CONST) fprintf( stream , "%ld" , result->u.intconstval);
		if(result->tag == STR_CONST) fprintf( stream , "%s"  , result->u.strconstval);

	}
	else if(get_item(result->u.idval)== NULL){
		fprintf( stream , "%s"  , result->u.idval);
	}
	else fprintf( stream , "%s", result->key );


}


/* Local routines */

/* Returns NULL if item not found */
static DR get_item(const char *key)
{
    int index = hash(key);
    DR p = hash_tab[index];
    debug1("get_item: p %s NULL\n", p==NULL?"==":"!=");
    while (p!=NULL && strcmp(key,p->key)!=0)
        p = p->next;


    return p;
}

/* Returns the number of new items inserted: 1 if new insertion; 0 otherwise */
static int insert_or_update(DR new_item)
{
    int ret;
    const char *key = new_item->key;
    int index = hash(key);
    DR p = hash_tab[index], *prev = hash_tab+index;
    
    debug1("insert_or_update(key=%s) called\n", new_item->key);
    while (p!=NULL && strcmp(key,p->key)) {
        prev = &p->next;
        p = p->next;
    }

    if (p == NULL) {
            /* Insertion */
        debug1("  p is NULL -- inserting at index %d\n", index);
        insert_at_front(prev, new_item);
        debug("    done\n");
        if (num_items++ > MAX_LOAD_FACTOR*h_size) {
            debug("    resizing\n");
            resize(SCALE_FACTOR*h_size);
        }
        ret = 1;
    }
    else {
            /* Update */
        debug1("  p is not NULL -- updating at index %d\n", index);
        unmark_cycle(p);
        free(remove_from_front(prev));
        insert_at_front(prev, new_item);
        ret = 0;
    }

    mark_cycle(new_item);
    debug("    cycle marked\n");
    return ret;
}

// Marks the new cycle, if there is one
static void mark_cycle(DR item)
{
        // redacted
	 DR loop ;

	 if(item->tag != ID) return ;

	 loop = get_item(item->u.idval);

	 while(loop!=NULL && loop!=item){
		 if(loop->tag != ID) break ;
		 loop  = get_item(loop->u.idval);
	 }

	 if(loop == item ){
		 item->in_cycle = 1 ;
		 loop = get_item(item->u.idval);
		 while(loop!=NULL && loop!=item){
				 loop->in_cycle = 1 ;
				 loop  = get_item(loop->u.idval);
			 }
	 }

	 return ;

}

// Unmark an existing cycle
static void unmark_cycle(DR item)
{
        // redacted
	DR loop ;
	if(item->in_cycle == 1){

		item->in_cycle = 0 ;
		loop = get_item(item->u.idval);
		while(loop != item){
			loop->in_cycle = 0 ;
			loop = get_item(loop->u.idval);
		}

	}

	return ;

}


static int hash(const char *key)
{
    int sum = 0;
    for (; *key; key++)
        sum = (37*sum + *key) % h_size;
    return sum;
}

static void insert_at_front(DR *list, DR new_item)
{
    new_item->next = *list;
    *list = new_item;
}

static DR remove_from_front(DR *list)
{
    DR ret = *list;
    *list = (*list)->next;
    return ret;
}

static void resize(int size)
{
    int i;
    
    DR *temp = hash_tab;
    int temp_size = h_size;

    h_size = size;
    hash_tab = (DR *) malloc(h_size * sizeof(DR));
    for (i=0; i<h_size; i++)
        hash_tab[i] = NULL;

        // This only occurs on the initial sizing, with empty dictionary
    if (temp == NULL)
        return;
    
    for (i=0; i<temp_size; i++)
        while (temp[i] != NULL) {
            int index = hash(temp[i]->key);
            insert_at_front(hash_tab+index, remove_from_front(temp+i));
        }
    free(temp);
}
