#include "firstPass.h"

void print_code(code* p_code) {
    while (p_code) {
        printf("%x\n", p_code->code_line.bytes);
        p_code = p_code->next;
    }
}

void print_symbol(symbol* p_symbol) {
    while (p_symbol) {
        printf("name: %s, base address: %d, offset: %d, attributes: %s, value: %d\n", 
               p_symbol->symbol, p_symbol->baseAddress, p_symbol->offset,
               p_symbol->attributes, p_symbol->value);
        p_symbol = p_symbol->next;
    }
}

void print_data(data *p_data){
    while(p_data){
        printf("item: %d, absolute: %d, relocatable: %d, external: %d \n",
                 p_data->data_line.item, p_data->data_line.absolute,
                 p_data->data_line.relocatable, p_data->data_line.external);
        p_data = p_data->next;
    }
}

int main(int argc, char const *argv[]){
    int i;
    char* filename = "testsForFirstPass/example";
    
    printf("aaaaa %s \n",filename);
        struct images images;
        images.symbol_head = (symbol*)malloc(sizeof(symbol));
        images.symbol_tail = images.symbol_head;
        images.code_head = (code *)malloc(sizeof(code));
        images.code_tail = images.code_head;
        images.data_head = (data*)malloc(sizeof(data));
        images.data_tail = images.data_head;
        printf("-----------------------%s-----------------------\n", filename);
        firstPass(filename, &images);
        print_symbol(images.symbol_head);
        print_code(images.code_head);
        print_data(images.data_head);
}