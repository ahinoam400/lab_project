#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "fileMaker.h"
int main(int argc, char const *argv[]){
    int i;
    struct images images;
    images.symbol_head = (symbol*)malloc(sizeof(symbol));
    images.code_head = (code *)malloc(sizeof(code));
    images.data_head = (data*)malloc(sizeof(data));
    images.ext_head = (external_words*)malloc(sizeof(external_words));
    for(i=1; i<argc; i++){
        images.symbol_tail = images.symbol_head;
        images.code_tail = images.code_head;
        images.data_tail = images.data_head;
        images.ext_tail = images.ext_head;
        if(macroSpread(argv[i]) == 0)continue;
        if(firstPass(argv[i], &images) == 0)continue;
        if(secondPass(argv[i], &images) == 0)continue;
        objectFile(argv[i], &images);
        entryFile(argv[i], &images);
        externalFile(argv[i], &images);
        freeImages(&images);
    }
    return 0;
}

void freeImages(struct images *images){
    code *code_current = images->code_head->next;
    code *code_next;
    data *data_current = images->data_head->next;
    data *data_next;
    symbol *symbol_current = images->symbol_head->next;
    symbol *symbol_next;
    external_words *ext_current = images->ext_head->next;
    external_words *ext_next;
    while(code_current){
        code_next = code_current->next;
        free(code_current);
        code_current = code_next;
    }
    while(data_current){
        data_next = data_current->next;
        free(data_current);
        data_current = data_next;
    }
    while(symbol_current){
        symbol_next = symbol_current->next;
        free(symbol_current);
        symbol_current = symbol_next;
    }
    while(ext_current){
        ext_next = ext_current->next;
        free(ext_current);
        ext_current = ext_next;
    }
}
