int i=0;
int data_offset = 0;
int data_location(){return data_offset++;}

int code_offset = 0;
int reserve_loc(){
    return code_offset++;
}
int gen_label(){
    return code_offset;
}

void gen_code(enum code_ops operation, int arg){
    code[code_offset].op = operation;
    code[code_offset++].arg= arg;
    printf("%d: %-10s%11d\n",i++,op_name[operation], arg);
}
void back_patch(int addr, enum code_ops operation, int arg){
    code[addr].op = operation;
    code[addr].arg = arg;
}