symRec *putsym();
symRec *getsym();
struct symrec{
    char *name;
    struct symrec *next;
};
typedef struct symrec symRec;
symRec *sym_table = (symRec *)0;

