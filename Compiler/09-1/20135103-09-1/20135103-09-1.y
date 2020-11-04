/* simplest version of calculator */
%{
#include <stdio.h>

extern int yylex();
int yyerror(char *);
%}
/* declare tokens */
%token NUMBER
%token ADD SUB MUL DIV
%token EOL
%%
calclist: /* nothing */
 | calclist exp EOL { printf("= %d\n", $2);}
 ;
exp: factor {$$ = $1; }
 | exp ADD factor { $$ = $1 + $3; printf("ADD Reduce\n"); }
 | exp SUB factor { $$ = $1 - $3; printf("SUB Reduce\n");}
 ;
factor: term
 | factor MUL term { $$ = $1 * $3; printf("MUL Reduce\n"); }
 | factor DIV term { $$ = $1 / $3; printf("DIV Reduce\n");}
 ;
term: NUMBER { printf("NUMBER Reduce\n");}
;
%%
int main(int argc, char **argv)
{
  printf("> ");
  yyparse();

  return 0;
}
int yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
  return 1;
}
