#include "firstPass.h"
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
    enum addressingModes{ immediate = 0,direct,index,register_direct};
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1));
    int lineLength = 0;
    int errFlag = 0, symbolFlag = 0;
    int num, i, operandsNum, j = 0, addressing_mode;
    char line[MAX_LINE_LEN], *name, *ws = " \t";
    char firstChar = ' ';
    bool isEmptyLine = true;
    command *cmd = (command *)malloc(sizeof(command));
    while (fgets(line, MAX_LINE_LEN, assembly)){
        char *arr[MAX_LINE_LEN];
        L = 0;
        split(line, arr); /*split the line into array*/
        lineLength = strlen(line);
        line[lineLength] = '\0';
        for (i = 0; i < lineLength; i++){ /*checks if the line is empty line*/
            if (!strchr(ws, line[i])){
                isEmptyLine = false;
                firstChar = line[i];
                break;
            }
        }
        if (firstChar != ';' && isEmptyLine == false){ /*if the is not an empty line and not a comment line*/
            if (arr[j][strlen(arr[j]) - 1] == ':'){ /*if the first word is a symbol definition*/
                if (j != 0){
                    printf("ERROR: ILLEGAL SYMBOL\n");
                    errFlag = 1;
                    continue;
                }
                name = arr[j];
                name[strlen(name) - 1] = '\0'; /*remove the : from the symbol*/
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
            } else{
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
                    if ((strcmp(arr[j++], cmd->cmdName)) == 0){                                       /*if arr[j] is command*/
                        code_tail = addDataNode(code_tail); /*add the first word*/
                        code_tail->code_line.command.opcode = cmd_arr[i].cmd_opcode;
                        code_tail->code_line.command.class.absolute = 1;
                        L++;
                    }
                    if (i < 14){                                                           /*if the command have oprands */
                        code_tail->code_line.word.funct = cmd_arr[i].cmd_funct; /* add the second word*/
                        code_tail->code_line.word.class.absolute = 1;
                        L++;
                    }
                }
                if (operandsNum > 0){ /*if the command have operand*/
                    addressing_mode = addressingModeFirstPass(arr[j++], 0, symbol_head, code_tail);
                    if (addressing_mode == immediate)
                        L++;
                    if (addressing_mode == direct || addressing_mode == index)
                        L += 2;
                    if (addressing_mode == -1){
                        errFlag = 1;
                        continue;
                    }
                    if (operandsNum < 1){ /*if the command have two operands*/
                        addressing_mode = addressingModeFirstPass(arr[j++], 1, symbol_head, code_tail);
                        if (addressing_mode == immediate)L++;
                        if (addressing_mode == direct || addressing_mode == index)L += 2;
                        if (addressing_mode == -1){
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

/*finds the addressing mode of the operand and add it to the code*/
int addressingModeFirstPass(char *operand, int src_or_dest){
    enum addressingModes{immediate = 0, direct,index, register_direct};
    int state, addressing_mode = -1, i, num, len, j;
    char *copy;
    char symCopy[strlen(operand)], regCopy[strlen(operand)];
    symbol *node = symbol_head;
    /*find the addresing mode*/
    if (operand[0] == '#'){
        copy = operand + 1;
        if (num = isLegalNumber(copy))
            addressing_mode = immediate;
    }else if (isRegister(operand)){
        num = isRegister(operand);
        addressing_mode = register_direct;
    }else if (isLegalSymName(operand)){
        addressing_mode = direct;
    }else{
        strcpy(symCopy, operand);
        for (i = 0; symCopy[i] != '\0' && symCopy[i] != '['; i++);
        symCopy[i - 1] = '\0';
        strcpy(regCopy, operand + i + 1);
        if (regCopy[strlen(regCopy) - 1] != ']' || !isLegalSymName(symCopy))
            return(printAndReturn("ERROR : ILLEGAL OPERAND\n",-1));
        len = strlen(regCopy);
        regCopy[len - 1] = '\0';
        if (!(num = isRegister(regCopy)))
            return(printAndReturn("ERROR : ILLEGAL OPERAND\n", -1));
        if (num < 10)
            return(printAndReturn("ERROR : ILLEGAL REGISTER NUMBER\n", -1));
        addressing_mode = index;
    }
    if (src_or_dest == 0) /*if the operand is a source operand*/
        code_tail->code_line.word.src_address = addressing_mode;
    if (src_or_dest == 1) /*if the operand is a destination operand*/
        code_tail->code_line.word.dest_address = addressing_mode;
    switch (addressing_mode){/*write the code of the addressing mode*/
    case immediate:
        code_tail->code_line.imm_word.class.absolute = 1;
        code_tail->code_line.imm_word.word = num;
        break;
    case direct:
        /*wait til the second pass*/
        break;
    case index:
        /* wait til the second pass*/
    case register_direct:
        if (src_or_dest == 0)
            code_tail->code_line.word.src_register = num;
        if (src_or_dest == 1)
            code_tail->code_line.word.dest_register = num;
    default:
        break;
    }
    return addressing_mode;
}

void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN]){
    if (symbol_head == NULL){
        symbol_head = (symbol *)malloc(sizeof(symbol));
        strcpy(head->symbol, symbolName);
        if (!strcmp(attribute, ".extern")){
            symbol_head->value = 0;
            symbol_head->baseAddress = 0;
            symbol_head->offset = 0;
            strcat(symbol_head->attributes, "external");
            symbol_tail = (symbol *)malloc(sizeof(symbol));
            symbol_head->next = symbol_tail;
            return;
        }
        symbol_head->value = IC;
        symbol_head->baseAddress = (IC / 32) * 32;
        symbol_head->offset = IC - symbol_head->baseAddress;
        if (!strcmp(attribute, ".data") || !strcmp(attribute, ".string"))
            strcat(symbol_head->attributes, "data");
        else
            strcat(symbol_head->attributes, "code");
        symbol_tail = (symbol *)malloc(sizeof(symbol));
        symbol_head->next = symbol_tail;
    }else{
        symbol_tail = (symbol *)malloc(sizeof(symbol));
        strcpy(symbol_tail->symbol, symbolName);
        if (!strcmp(attribute, ".extern")){
            symbol_tail->value = 0;
            symbol_tail->baseAddress = 0;
            symbol_tail->offset = 0;
            strcat(symbol_tail->attributes, "external");
            symbol *temp = (symbol *)malloc(sizeof(symbol));
            symbol_tail->next = temp;
            return;
        }
        symbol_tail->value = IC;
        symbol_tail->baseAddress = (IC / 32) * 32;
        symbol_tail->offset = IC - symbol_tail->baseAddress;
        if (!strcmp(attribute, ".data") || !strcmp(attribute, ".string"))
            strcat(symbol_tail->attributes, "data");
        else
            strcat(symbol_tail->attributes, "code");
        symbol *temp = (symbol *)malloc(sizeof(symbol));
        symbol_tail->next = temp;
    }
}

/*this function checks if symbolName is a legal name for a symbol*/
int isLegalSymName(char symbolName[MAX_LINE_LEN]){
    int i;
    if (isCommand(symbolName) != -1 || isRegister(symbolName) != 0)
        return 0;
    for (i = 0; symbolName[i] != '\0'; i++){
        if (!isalpha(symbolName[i]) && !isdigit(symbolName[i]))
            return 0;
    }
    return 1;
}

/*checks if num is a legal number */
int isLegalNumber(char *number){
    int num, i = 0;
    if (number[i] == '-' || number[i] == '+')
        i++;
    while (number[i] != '\0' && number[i] != '\n'){
        if (!isdigit(number[i]))
            return 0;
        i++;
    }
    return atoi(number);
}

/*Checks if str is a register */
int isRegister(char *str){
    int i = 0, number;
    char *str2;
    if (str[i] != 'r')
        return 0;
    for (i = 1; str[i] != '\0'; i++){
        if (!isdigit(str[i]))
            return 0;
    }
    str2 = str + 1;
    number = atoi(str2);
    if (number > 15)
        return 0;
    return number;
}