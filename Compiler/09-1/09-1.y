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
 | exp ADD factor { $$ = $1 + $3; }
 | exp SUB factor { $$ = $1 - $3; }
 ;
factor: term
 | factor MUL term { $$ = $1 * $3; }
 | factor DIV term { $$ = $1 / $3; }
 ;
term: NUMBER
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
