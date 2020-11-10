%{
/*************************************************************************
                   Compiler for the Simple language
***************************************************************************/
/*=========================================================================
       C Libraries, Symbol Table, Code Generator & other C code
=========================================================================*/
#include <stdio.h>        /* For I/O                                     */
#include "ST.h"
extern int yylex();
void yyerror(char *);
void install(char * sym_name){
  symRec *s;
  s = getsym(sym_name); /* 변수가 정의되어 있는지 찾음 */
  
  if(s==0){ /* 존재하지 않음 */
    s =putsym(sym_name);
  }  
  else{
    printf("%s는 이미 정의됨\n",sym_name);
  }
}
void context_check(char *sym_name){
  if(getsym(sym_name) == 0)
    printf("%s는 정의되지 않은 식별자임\n",sym_name);
}
%}
/*=========================================================================
                               TOKENS
=========================================================================*/
%start program
/* 토큰에 올바른 자료형 정보 지정 */
%token <intval> NUMBER          /* Simple integer                       */
%token <id> IDENTIFIER      /* Simple identifier                    */
%token IF WHILE        /* For backpatching labels              */
%token SKIP THEN ELSE FI DO END
%token INTEGER READ WRITE LET IN
%token ASSGNOP
%union{       /* yylval에 해당하는 자료형 */
    int intval; /* 정수일 경우 */
    char *id;   /* 식별자일 경우 */
};
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
   | INTEGER id_seq IDENTIFIER '.' { install($3); }
   ;

id_seq : /* empty */
   | id_seq IDENTIFIER ',' { install($2); }
   ;

commands : /* empty */
   | commands command ';'
   ;

command : SKIP
   | READ IDENTIFIER { context_check($2); }
   | WRITE expression
   | IDENTIFIER ASSGNOP expression { context_check($1); }
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
   | IDENTIFIER { context_check($1); }
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

