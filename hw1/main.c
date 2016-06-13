#include "my_defs.h"

int line_no = 1 ;

main( int argc, char **argv )
{

	FILE *fp ;
    ++argv, --argc;
    /* skip over program name */
    if ( argc > 0 )
        fp = fopen( argv[0], "r" );
    else
        fp  = stdin;

    init_lex(fp);
    init_dict();

    yylex();
}
