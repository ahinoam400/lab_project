#include "firstPassFunctions.h"
/*#include "functions.h"*/
int firstPass(char *filename);
extern command cmd_arr[];
int ICF, DCF;
int main(){
    head_code = (code *)malloc(sizeof(code));
    tail_code = head_code;
    firstPass("test");
}

int firstPass(char *filename){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
        return(printAndReturn("ERORR OPENING FILE\n", -1));
    int lineLength = 0;
    int errFlag = 0, symbolFlag = 0;
    int num, i, command, j=0;
    char line[MAX_LINE_LEN], ws[6] = "\t \n", *name;
    char *token, name2[MAX_LINE_LEN], *name, firstChar = ' ';
    bool isEmptyLine = true;
    while (fgets(line, MAX_LINE_LEN, assembly)){
        char *arr[MAX_LINE_LEN];
        L = 0;
        split(line, arr);
        lineLength = strlen(line);
        line[lineLength] = '\0';
        for (i = 0; i < lineLength; i++){
            if (!strchr(ws, line[i])){
                isEmptyLine = false;
                firstChar = line[i];
                break;
            }
        }
        if (firstChar != ';' && isEmptyLine == false){
            if (arr[j][strlen(arr[j]) - 1] == ':'){
                if(j!=0){
                    printf("ERROR: ILLEGAL SYMBOL\n");
                    errFlag = 1;
                    continue; 
                }
                name = arr[j];
                name[j][strlen(name[j])-1] = '\0';
                if (!isLegalSymName(name)){
                    printf("ERROR: ILLEGAL SYMBOL NAME\n");
                    errFlag = 1;
                    continue;
                }
                if (isNameInTable(name)){
                    printf("ERROR: SYMBOL NAME ALREADY EXISTS\n");
                    errFlag = 1;
                    continue;
                }
                symbolFlag = 1;
                j++;
            }
            if (!strcmp(arr[j], ".string")){
                if (symbolFlag)
                    addSymbol(name, IC, arr[j]);
                j++;
                for (i = 0; arr[j][i]!= '\0'; i++){
                    if (!addDataNode()){
                        errFlag = 1;
                        continue;
                    }
                    tail_code->code_line.string_word.str = token[i]; /*not working well*/
                    tail_code->code_line.string_word.class.absolute = 1;
                    tail_code->code_line.string_word.class.relocatable = 0;
                    tail_code->code_line.string_word.class.external = 0;
                    DC++;
                }
            }else if (!strcmp(arr[j], ".data")){
                if (symbolFlag)
                    addSymbol(name, IC, arr[j]);
                j++;
                if (!(num = isLegalNumber(arr[j])))
                    return(printAndReturn("ERROR : ILLEGAL DATA", 0));
                if (!addDataNode()){
                    errFlag = 1;
                    continue;
                }
                tail_code->code_line.data_word.data_num = decimalToBinary(num); /*not working well*/
                tail_code->code_line.data_word.class.absolute = 1;
                tail_code->code_line.string_word.class.relocatable = 0;
                tail_code->code_line.string_word.class.external = 0;
                DC++;
            }
            else if (!strcmp(arr[j], ".entry"))
                continue;
            else if (!strcmp(arr[j], ".extern")){
                if (!symbolFlag){
                    printf("ERROR : THERE IS NO SYMBOL");
                    errFlag = 1;
                    continue;
                }
                addSymbol(name, 0, token);
                continue;
            }else{
                if (symbolFlag){
                    addSymbol(name, IC, ".code");
                    j++;
                }
                if (command = isCommand(arr[j]) == -1){
                    printf("ERROR: COMMAND NAME");
                    errFlag = 1;
                    continue;
                }
            }

            /*enter steps 13-15 here, i couldn't understand them and i got stuck because of them... 0_0*/
            IC += lineLength;
            L += lineLength;
            lineLength = 0;
        }
    }
    ICF = IC;
    DCF = DC;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    sym = head;
    while (sym != NULL){
        if (!strcmp(sym->attributes, ".data")){
            sym->value = ICF;
            sym->baseAddress = (ICF / 32) * 32;
            sym->offset = ICF - sym->baseAddress;
        }
        sym = sym->next;
    }
    free(sym);
}
