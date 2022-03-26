#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
int main(int argc, char const *argv[]){
    int i;
    struct images images;
    images.symbol_head = (symbol*)malloc(sizeof(symbol));
    images.symbol_tail = images.symbol_head;
    images.code_head = (code *)malloc(sizeof(code));
    images.code_tail = images.code_head;
    images.data_head = (data*)malloc(sizeof(data));
    images.data_tail = images.data_head;
    for(i=0; i<argc; i++){
        if(macroSpread(argv[i]) == 0)continue;
        if(firstPass(argv[i], &images) == 0)continue;
        if(secondPass(argv[i], &images) == 0)continue;
        
    }
    return 0;
}
