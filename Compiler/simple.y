%{
/*************************************************************************
                   Compiler for the Simple language
***************************************************************************/
/*=========================================================================
       C Libraries, Symbol Table, Code Generator & other C code
=========================================================================*/
#include <stdio.h>        /* For I/O                                     */
extern int yylex();
void yyerror(char *);
%}
/*=========================================================================
                               TOKENS
=========================================================================*/
%start program
%token NUMBER          /* Simple integer                       */
%token IDENTIFIER      /* Simple identifier                    */
%token IF WHILE        /* For backpatching labels              */
%token SKIP THEN ELSE FI DO END
%token INTEGER READ WRITE LET IN
%token ASSGNOP
/*=========================================================================
                          OPERATOR PRECEDENCE
=========================================================================*/
%left '-' '+'
%left '*' '/'
%right '^'
/*=========================================================================
                   GRAMMAR RULES for the Simple language
=========================================================================*/
%%
program : LET
            declarations
          IN
            commands
          END { YYACCEPT; }
;

declarations : /* empty */
   | INTEGER id_seq IDENTIFIER '.'
   ;

id_seq : /* empty */
   | id_seq IDENTIFIER ','
   ;

commands : /* empty */
   | commands command ';'
   ;

command : SKIP
   | READ IDENTIFIER
   | WRITE expression
   | IDENTIFIER ASSGNOP expression
   | IF expression
     THEN commands
     ELSE
       commands
     FI

   | WHILE
       expression
     DO
       commands
     END
   ;

expression : NUMBER     
   | IDENTIFIER 
   | expression '<' expression
   | expression '=' expression
   | expression '>' expression
   | expression '+' expression
   | expression '-' expression
   | expression '*' expression
   | expression '/' expression
   | expression '^' expression
   | '(' expression ')'
   ;
%%
/*=========================================================================
                                 YYERROR
=========================================================================*/
void yyerror ( char *s )  /* Called by yyparse on error */
{
  printf ("%s\n", s);
}
/*=========================================================================
                                  MAIN
=========================================================================*/
int main( int argc, char *argv[] )
{
  extern FILE *yyin;

  ++argv; --argc;
  yyin = fopen( argv[0], "r" );
  yyparse ();
  printf ( "Parse Completed\n" );

  return 0;
}
/**************************** End Grammar File ***************************/

