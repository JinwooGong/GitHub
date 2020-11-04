%{
    #include <stdio.h>
    extern int yylex();
    void yyerror(char *);
%}
%start program /* start symbol*/
%token LET IN END
%token IDENTIFIER
%token SKIP IF THEN ELSE WHILE DO READ WRITE FI
%token NUMBER

%right ':=' /* 오른쪽 우선순위 (제일 낮은 순위) */
%left '=' '<' '>'
%left '-' '+' /* 왼쪽 우선순위 */
%left '/' '*' /* 왼쪽 우선순위 */
%right '^' 
%left '('
%right ')' /* 오른쪽 우선 순위 (제일 높은 순위) */

%%
program : LET declarations IN command_sequence END {YYACCEPT;}
;

declarations : /* empty */
| INTEGER id_seq IDENTIFIER '.'
;

id_seq : /* empty */
| id_seq IDENTIFIER ','
;

command_sequence : /* empty */
| command_sequence command
;
command : SKIP ';'
| IDENTIFIER ':=' expression ';'
| IF exp THEN command_sequence ELSE command_sequence FI ';'
| WHILE exp DO command_sequence END ';'
| READ IDENTIFIER ';'
| WRITE expression ';'
;

exp : expression '=' expression 
| expression '<' expression 
| expression '>' expression
;

expression : NUMBER | IDENTIFIER | '(' expression ')'
| expression '+' expression | expression '-' expression
| expression '*' expression | expression '/' expression
|expression '^' expression
;
%%
main(int argc, char *argv[]){
    extern FILE *yyin;
    argv++;
    argc--;
    yyin = fopen(argv[0],"r");
    yydebug = 1;
    return 0;
}

void yyerror(cahr *s){
    printf("%s\n",s);
    return 1;
}