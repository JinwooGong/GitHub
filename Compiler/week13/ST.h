#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symrec{
    char *name; /* 심볼 이름 */
    int offset;
    struct symrec *next; /* 링크 */
};
/* 심볼 테이블 초기화 */
typedef struct symrec symRec;
symRec *sym_table = (symRec *)0; /* 빈 심볼테이블 초기화 */

/* 심볼 테이블 관련 함수 */
symRec *putsym(char *sym_name){
    symRec *ptr;
    ptr = (symRec *)malloc(sizeof(symRec));
    ptr->name = (char *)malloc(strlen(sym_name)+1);
    strcpy(ptr->name, sym_name);
    ptr->offset = data_location();
    ptr->next = (symRec *)sym_table;
    sym_table = ptr;
    return ptr;
}

symRec *getsym(char *sym_name){
    symRec *ptr;
    for(ptr=sym_table; ptr != NULL; ptr= ptr->next){
        if(strcmp(ptr->name,sym_name)==0)
            return ptr;
    }
    return 0;
}