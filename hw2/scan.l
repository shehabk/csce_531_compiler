%{
#include <ctype.h>
#include "defs.h"
#include "y.tab.h"

%}
	int line_no = 1 ;
digit           [0-9]+
id              [A-Za-z]

%%

[ \t]	{
		
		}

{digit}	{ 	yylval.y_int = atoi(yytext);		
			return CONST;
		}

{id}	{	yylval.y_int = toupper(yytext[0]);   /* case insensitive */		
        	return VAR;
	
		}


.|\n	{	
			return yytext[0];		
		}	



%%
