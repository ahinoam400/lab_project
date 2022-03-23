#include "firstPass.h"

void print_code(code* p_code) {
    while (p_code) {
        printf("%x\n", p_code->code_line.bytes);
        p_code = p_code->next;
    }
}

void print_symbol(symbol* p_symbol) {
    while (p_symbol) {
        printf("%s\n", p_symbol->symbol);
        p_symbol = p_symbol->next;
    }
}

int main(){
    code mycode;
    mycode.code_line.command.opcode = 0;//1<<15;
    mycode.code_line.command.absolute = 0;
    mycode.code_line.command.relocatable = 0;
    mycode.code_line.command.external = 1;
    mycode.code_line.command.empty_bit = 0;//1<<15;
    printf("%x\n",mycode.code_line.bytes);
    printf("%x\n",0b0100001111000001);
    return 0;
    struct images images;
    images.symbol_head = (symbol*)malloc(sizeof(symbol));
    images.symbol_tail = images.symbol_head;
    images.code_head = (code *)malloc(sizeof(code));
    images.code_tail = images.code_head;
    images.data_head = (data*)malloc(sizeof(data));
    images.data_tail = images.data_head;
    firstPass("assemblyExample", images);
    print_symbol(images.symbol_head);
    print_code(images.code_head);
}