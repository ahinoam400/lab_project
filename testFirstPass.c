#include "firstPass.h"


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
        print_data(images.data_head, images.ICF);
}