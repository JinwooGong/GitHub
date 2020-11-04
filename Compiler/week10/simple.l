%{
	#include "simple.tab.h"
	
	int yylval;
%}
letter [a-zA-Z]
%%
[ \t\n] ;

[0-9]+ 	{yylval = atoi(yytext); return NUMBER;}
let 	{return LET;}
in 		{return IN;}
end		{return END;}
integer {return INTEGER;}
skip 	{return SKIP;}
if 		{return IF;}
then 	{return THEN;}
else 	{return ELSE;}
fi		{return FI;}
while 	{return WHILE;}
do 		{return DO;}
write 	{return WRITE;}
read 	{return READ;}
{letter}+ {return IDENTIFIER;}
%%
