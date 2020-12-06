%{
  #include <stdio.h> 
  #include <stdlib.h>
  #include <string.h>
  #include "SM.h"
  #include "CG.h"
  #include "ST.h"

  #define YYDEBUG 1

  extern int yylex();
  void yyerror(char *);

  int errors;
  struct lbs{
    int for_goto;
    int for_jmp_false;
  };

  struct lbs * newlblrec(){
    return (struct lbs*)malloc(sizeof(struct lbs));
  }

  void install(char * sym_name){
    symRec *s;
    s = getsym(sym_name); /* 변수가 정의되어 있는지 찾음 */
    
    if(s==0){ /* 존재하지 않음 */
      s = putsym(sym_name);
    }  
    else{
      errors++;
      printf("%s는 이미 정의됨\n",sym_name);
    }
  }
  void context_check(enum code_ops operation,char *sym_name){
    symRec *identifier;
    identifier = getsym(sym_name);
    if(identifier == 0){
      errors++;
      printf("%s는 정의되지 않은 식별자임\n",sym_name);
    }
    else
      gen_code(operation, identifier->offset);
      
  }
%}
/*=========================================================================
                               TOKENS
=========================================================================*/
%start program
/* 토큰에 올바른 자료형 정보 지정 */
%token <intval> NUMBER          /* Simple integer                       */
%token <id> IDENTIFIER      /* Simple identifier                   */
%token <lbls> IF WHILE
%token IF WHILE        /* For backpatching labels              */
%token SKIP THEN ELSE FI DO END
%token INTEGER READ WRITE LET IN
%token ASSGNOP
%union{       /* yylval에 해당하는 자료형 */
    int intval; /* 정수일 경우 */
    char *id;   /* 식별자일 경우 */
    struct lbs *lbls;
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
          IN  {gen_code(DATA, sym_table->offset);}
            commands
          END { gen_code(HALT,0); YYACCEPT; }
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
   | READ IDENTIFIER  { context_check(READ_INT, $2); }
   | WRITE expression { gen_code(WRITE_INT,0);}
   | IDENTIFIER ASSGNOP expression { context_check(STORE, $1); }
   | IF expression  { $1 = (struct lbs *) newlblrec(); $1->for_jmp_false = reserve_loc();}
     THEN commands  { $1->for_goto = reserve_loc();}
     ELSE           { back_patch($1->for_jmp_false, JMP_FALSE, gen_label());}
       commands 
     FI             {back_patch($1->for_goto, GOTO, gen_label());}

   | WHILE          { $1=(struct lbs *)newlblrec(); $1->for_goto =gen_label();}
       expression   { $1->for_jmp_false = reserve_loc();}
     DO
       commands
     END            { gen_code(GOTO, $1->for_goto); back_patch($1->for_jmp_false,JMP_FALSE,gen_label());}
   ;

expression : NUMBER             { gen_code(LD_INT,$1);}
   | IDENTIFIER                 { context_check(LD_VAR, $1); }
   | expression '<' expression  { gen_code(LT, 0);}
   | expression '=' expression  { gen_code(EQ, 0);}
   | expression '>' expression  { gen_code(GT, 0);}
   | expression '+' expression  { gen_code(ADD, 0);}
   | expression '-' expression  { gen_code(SUB, 0);}
   | expression '*' expression  { gen_code(MULT, 0);}
   | expression '/' expression  { gen_code(DIV, 0);}
   | expression '^' expression  { gen_code(PWR, 0);}
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

