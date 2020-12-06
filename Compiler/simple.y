%{
/*************************************************************************
                   Compiler for the Simple language
***************************************************************************/
/*=========================================================================
       C Libraries, Symbol Table, Code Generator & other C code
=========================================================================*/
#include <stdio.h>    /* For I/O */
#include <stdlib.h>   /* For malloc here and in symbol table */
#include <string.h>   /* For strcmp in symbol table */
#include "ST.h"       /* Symbol Table */
#include "SM.h"       /* Stack Machine */
#include "CG.h"       /* Code Generator */

#define YYDEBUG 1       /* For Debugging */

int errors;           /* Error Count-incremented in CG, ckd here */
struct lbs{           /* For labels: if and while */
  int for_goto;
  int for_jmp_false;
};

struct lbs * newlblrec(){   /* Allocate space for the labels */
  return (struct lbs *) malloc(sizeof(struct lbs));
}

void install(char * sym_name){
  symRec *s;
  s = getsym(sym_name);
  if(s==0)
    s=putsym(sym_name);
  else{
    errors++;
    print("%s는 이미 정의됨\n",sym_name);
  }
}

void context_check(enum code_ops operation, char *sym_name){
  symRec *identifier;
  identifier = getsym(sym_name);
  if(identifier == 0){
    errors++;
    printf("%s",sym_name);
    printf("%s\n"," is an undeclared identifier");
  }
  else{
    gen)code(operation, identifier->offset);
  }
}
extern int yylex();   
void yyerror(char *);
%}
/*=========================================================================
                               TOKENS
=========================================================================*/
%union{               /* The Semantic Records */
  int intval;         /* Integer values */
  char *id;           /* Identifiers */
  struct lbs *lbls;   /* For backpatching */
};
%start program
%token <intval> NUMBER          /* Simple integer                       */
%token <id> IDENTIFIER      /* Simple identifier                    */
%token <lbls> IF WHILE        /* For backpatching labels              */
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
          IN {get_code(DATA, sym_table->offset);}
            commands
          END { gen_code(HALT,0); YYACCEPT; }
;

declarations : /* empty */
   | INTEGER id_seq IDENTIFIER '.' {install($3);}
   ;

id_seq : /* empty */
   | id_seq IDENTIFIER ',' {install($2);}
   ;

commands : /* empty */
   | commands command ';'
   ;

command : SKIP
   | READ IDENTIFIER { context_check(READ_INT, $2);}
   | WRITE expression {gen_code(WRITE_INT,0);}
   | IDENTIFIER ASSGNOP expression {context_check(STORE,$1);}
   | IF expression {$1 = (struct lbs *)newlblrec();
                    $1->for_jmp_false = reserve_loc();}
     THEN commands {$1->for_goto = reserve_loc();}
     ELSE {back_patch($1->for_jmp_false, JMP_FALSE,get_label());}
       commands

     FI {back_patch($1->for_goto, GOTO, gen_label());}

   | WHILE {$1 = (struct lbs *) newlblrec();
            $1->for_goto = gen_label();}
       expression {$1->for_jmp_false = reserve_loc();}
     DO
       commands
     END {gen_code(GOTO, $1->for_goto);
          back_patch($1->for_jmp_false, JMP_FALSE, get_label());}
   ;

expression : NUMBER             { gen_code(LD_INT,$1);}
   | IDENTIFIER                 { context_check(LD_VAR, $1);}
   | expression '<' expression  { gen_code(LT,0);}
   | expression '=' expression  { gen_code(EQ,0);}
   | expression '>' expression  { gen_code(GT,0);}
   | expression '+' expression  { gen_code(ADD,0);}
   | expression '-' expression  { gen_code(SUB,0);}
   | expression '*' expression  { gen_code(MULT,0);}
   | expression '/' expression  { gen_code(DIV,0);}
   | expression '^' expression  { gen_code(PWR,0);}
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

