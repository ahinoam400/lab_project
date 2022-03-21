#include "firstPassFunctions.h"
int firstPass(char *filename);
extern command cmd_arr[];
int ICF, DCF;
int IC = 100, DC = 0, L = 0;
symbol *symbol_head = NULL, *symbol_tail = NULL;
code *code_head = NULL, *code_tail = NULL;
int main(){
    code_head = (code *)malloc(sizeof(code));
    code_tail = code_head;
    firstPass("test");
}

int firstPass(char *filename){
    enum addressingModes{immediate = 0, direct,index, register_direct};
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
        return(printAndReturn("ERROR OPENING FILE\n", -1));
    int lineLength = 0;
    int errFlag = 0, symbolFlag = 0;
    int num, i, operandsNum, j = 0, adressing_mode;
    char line[MAX_LINE_LEN], *name, *ws = " \t";
    char firstChar = ' ';
    bool isEmptyLine = true;
    command *cmd = (command *)malloc(sizeof(command));
    while (fgets(line, MAX_LINE_LEN, assembly)){
        char *arr[MAX_LINE_LEN];
        L = 0;
        split(line, arr);/*split the line into array*/
        lineLength = strlen(line);
        line[lineLength] = '\0';
        for (i = 0; i < lineLength; i++){/*checks if the line is empty line*/
            if (!strchr(ws, line[i])){
                isEmptyLine = false;
                firstChar = line[i];
                break;
            }
        }
        if (firstChar != ';' && isEmptyLine == false){/*if the is not an empty line and not a comment line*/
            if (arr[j][strlen(arr[j]) - 1] == ':'){/*if the first word is a symbol definition*/
                if (j != 0){
                    printf("ERROR: ILLEGAL SYMBOL\n");
                    errFlag = 1;
                    continue;
                }
                name = arr[j];
                name[strlen(name) - 1] = '\0';/*remove the : from the symbol*/
                if (!isLegalSymName(name)){
                    printf("ERROR: ILLEGAL SYMBOL NAME\n");
                    errFlag = 1;
                    continue;
                }
                if (isNameInTable(name, symbol_head)){
                    printf("ERROR: SYMBOL NAME ALREADY EXISTS\n");
                    errFlag = 1;
                    continue;
                }
                symbolFlag = 1;
                j++;
            }
            if (!strcmp(arr[j], ".string")){
                if (symbolFlag)
                    addSymbol(name, IC, arr[j], symbol_head, symbol_tail);
                j++;
                for (i = 0; arr[j][i] != '\0'; i++){
                    if (!addDataNode(code_tail)){
                        errFlag = 1;
                        continue;
                    }
                    code_tail->code_line.string_word.str = arr[j][i];
                    code_tail->code_line.string_word.class.absolute = 1;
                    code_tail->code_line.string_word.class.relocatable = 0;
                    code_tail->code_line.string_word.class.external = 0;
                    DC++;
                }
            }else if (!strcmp(arr[j], ".data")){
                if (symbolFlag)
                    addSymbol(name, IC, arr[j], symbol_head, symbol_tail);
                j++;
                if (!(num = isLegalNumber(arr[j]))){
                    printf("ERROR : ILLEGAL NUMBER\n");
                    errFlag = 1;
                    continue;
                }
                if (!addDataNode(code_tail)){
                    errFlag = 1;
                    continue;
                }
                code_tail->code_line.data_word.data_num = num;
                code_tail->code_line.data_word.class.absolute = 1;
                code_tail->code_line.data_word.class.relocatable = 0;
                code_tail->code_line.data_word.class.external = 0;
                DC++;
            }else if (!strcmp(arr[j], ".entry"))
                continue;
            else if (!strcmp(arr[j], ".extern")){
                if (!symbolFlag){
                    printf("ERROR: THERE IS NO SYMBOL\n");
                    errFlag = 1;
                    continue;
                }
                addSymbol(name, 0, arr[j], symbol_head, symbol_tail);
                continue;
            }else{
                if (symbolFlag){
                    addSymbol(name, IC, ".code", symbol_head, symbol_tail);
                    j++;
                }
                if (operandsNum = isCommand(arr[j]) == -1){
                    printf("ERROR: COMMAND NAME\n");
                    errFlag = 1;
                    continue;
                }
                for (i = 0; i < MAX_CMD_NUM; i++){
                    cmd = &cmd_arr[i];
                    if ((strcmp(arr[j++], cmd->cmdName)) == 0){/*if arr[j] is command*/
                        code_tail = addDataNode(code_tail);/*add the first word*/
                        code_tail->code_line.command.opcode = cmd_arr[i].cmd_opcode;
                        code_tail->code_line.command.class.absolute = 1;
                        L++;
                    }
                    if (i < 14){/*if the command have oprands */
                        code_tail->code_line.word.funct = cmd_arr[i].cmd_funct;/* add the second word*/
                        code_tail->code_line.word.class.absolute = 1;
                        L++;
                    }
                }
                if (operandsNum > 0){/*if the command have operand*/
                    adressing_mode = findAndWriteAddressingMode(arr[j++],0, symbol_head, code_tail, 0);
                    if (adressing_mode == immediate)
                        L++;
                    if (adressing_mode == direct || adressing_mode == index)
                        L += 2;
                    if (adressing_mode == -1){
                        errFlag = 1;
                        continue;
                    }
                    if(operandsNum < 1){/*if the command have two operands*/
                        adressing_mode = findAndWriteAddressingMode(arr[j++], 1,symbol_head, code_tail, 0);
                        if (adressing_mode == immediate) L++;
                        if (adressing_mode == direct || adressing_mode == index) L += 2;
                        if (adressing_mode == -1){
                            errFlag = 1;
                            continue;
                        }
                    }
                }
            }
        }
        IC += L;
        lineLength = 0;
    }
    ICF = IC;
    DCF = DC;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    sym = symbol_head;
    while (sym != NULL){
        if (!strcmp(sym->attributes, ".data")){
            sym->value = ICF;
            sym->baseAddress = (ICF / 32) * 32;
            sym->offset = ICF - sym->baseAddress;
        }
        sym = sym->next;
    }
    free(sym);
    return !errFlag;
}
