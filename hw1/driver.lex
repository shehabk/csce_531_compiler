%{
#include "my_defs.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
%}
	int gdef = 0 , gkey = 0 , gval = 0 ;
	char* store_key = NULL ;
	char* store_val = NULL ;
	
digit           [0-9]
int             {digit}+
id              [_A-Za-z][_A-Za-z0-9]*
string			\"(\\.|[^\\"])*\"
def				"#define"
ws				[ \t]

%%

{ws}*{def}		{ gdef = 1;}
{id}			{ if(gdef==1){
						
						store_key = (char*)malloc((yyleng+1)*sizeof(char));
						strcpy(store_key , yytext);
						gkey = 1;
						gdef = 0;
					}
				  else if(gkey==1){
				  		gkey = 0;
				  		gval = 1;
				  		store_val = (char*)malloc((yyleng+1)*sizeof(char));
				  		strcpy(store_val , yytext);
				  		add_id_to_dict(store_key , store_val);
				  }
				  else{
				  	store_val = (char*)malloc((yyleng+1)*sizeof(char));
				  	strcpy(store_val , yytext);
				  	output_substitution(yyout , store_val);
				  }
					
				}

{int}			{		if(gkey == 1){
						gkey = 0 ;
						gval = 1 ;
						store_val = (char*)malloc((yyleng+1)*sizeof(char));
						strcpy(store_val , yytext);
						add_int_to_dict(store_key, atol(store_val));
					}
					
					else{
						fprintf(yyout , "%s", yytext );
					}
				}
				
{string}		{ 		if(gkey == 1){
						gkey = 0 ;
						gval = 1 ;
						store_val = (char*)malloc((yyleng+1)*sizeof(char));
						strcpy(store_val , yytext);
						add_str_to_dict(store_key, store_val);
					}
					
					else{
						fprintf(yyout , "%s", yytext );
					}

				}	
				
{ws}+			{
					if(gdef!=1 && gkey!=1 && gval!=1) { fprintf(yyout , "%s", yytext ); }
				}

\n				{
					if(gdef!=1 && gkey!=1 && gval!=1) { fprintf(yyout , "%s", yytext ); }
					if(gval == 1) gval = 0 ;
					line_no++;
				}
				
.				fprintf(yyout , "%s", yytext );

%%

void init_lex(FILE *stream){
	yyin = stream ;
}