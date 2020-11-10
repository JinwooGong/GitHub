#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symrec{
    char *name;
    struct symrec *next;
};
typedef struct symrec symRec;
symRec *sym_table = (symRec *)0;

symRec *putsym(char *sym_name){
    symRec * newSym;
    newSym = (symRec *)malloc(sizeof(symRec));
    newSym->name = (char *)malloc(sizeof(sym_name)+1);
    strcpy(newSym->name,sym_name);

    newSym->next = (symRec *)sym_table;
    sym_table = newSym;

    return newSym;
}

symRec *getsym(char *sym_name){
    symRec *ptr;
    for(ptr = sym_table; ptr!=(symRec *)0 ; ptr = (symRec *) ptr->next){
        if(strcmp(ptr->name, sym_name) ==0){
            return ptr;
        }
    }
    return 0;
}