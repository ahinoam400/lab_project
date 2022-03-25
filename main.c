#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
int main(int argc, char const *argv[]){
    int i;
    for(i=0; i<argc; i++){
        if(macroSpread(argv[i]) == 0)continue;
        if(firstPass(argv[i]) == 0)continue;
        if(secondPass(argv[i]) == 0)continue;
    }
    return 0;
}
