#include "firstPass.h"
extern command cmd_arr[];
int ICF, DCF;
int IC = 100, DC = 0, L = 0;
void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN], int lineNum, struct images images);
int addressingModeFirstPass(char *operand, int src_or_dest , int lineNum, struct images images);


command cmd_arr[MAX_CMD_NUM]={
    {"mov",0,0}, {"cmp",1,0}, {"add",2,10},
    {"sub",2,11}, {"lea",4,0}, {"clr",5,10},
    {"not",5,11}, {"inc",5,12}, {"dec",5,13},
    {"jmp",9,10}, {"bne",9,11}, {"jsr",9,12},
    {"red",12,0}, {"prn",13,0}, {"rts",14,0},
    {"stop",15,0}
};


int firstPass(char *filename, struct images images){
    enum addressingModes{ immediate = 0,direct,index,register_direct};
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1, 0));
    int lineLength = 0, lineNum =0;
    int errFlag = 0, symbolFlag = 0;
    int num, i, operandsNum, j = 0, addressing_mode, dataNum;
    char line[MAX_LINE_LEN], *name, *ws = " \t";
    char firstChar = ' ';
    bool isEmptyLine = true;
    command *cmd = (command *)malloc(sizeof(command));

    while (fgets(line, MAX_LINE_LEN, assembly)){
        printf(";%s",line);
        char *arr[MAX_LINE_LEN] = {0};
        lineNum++;
        L = 0;
        dataNum = split(line, arr, lineNum); /*split the line into array*/
        lineLength = strlen(line);
        line[lineLength] = '\0';
        j=0;
        for (i = 0; i < lineLength; i++){ /*checks if the line is empty line*/
            if(strchr("\n", line[i])){
                isEmptyLine = true;
                break;
            }
            if (!strchr(ws, line[i])){
                isEmptyLine = false;
                firstChar = line[i];
                break;
            }
        }
        if(firstChar == ';' || isEmptyLine)continue;
        if(isEmptyLine == false){ /*if the is not an empty line and not a comment line*/
            if (arr[j][strlen(arr[j]) - 1] == ':'){ /*if the first word is a symbol definition*/
                if (j != 0){
                    printf("LINE %d : ERROR: ILLEGAL SYMBOL\n", lineNum);
                    errFlag = 1;
                    continue;
                }
                name = arr[j];
                name[strlen(name) - 1] = '\0'; /*remove the : from the symbol*/
                if (!isLegalSymName(name)){
                    printf("LINE %d : ERROR: ILLEGAL SYMBOL NAME\n", lineNum);
                    errFlag = 1;
                    continue;
                }
                if (isNameInTable(name, images.symbol_head)){
                    printf("LINE %d : ERROR: SYMBOL NAME ALREADY EXISTS\n" , lineNum);
                    errFlag = 1;
                    continue;
                }
                symbolFlag = 1;
                j++;
            }
            if (!strcmp(arr[j], ".string")){
                if (symbolFlag)
                    addSymbol(name, IC, arr[j], lineNum, images);
                j++;
                for (i = 0; arr[j][i] != '\0'; i++){
                    if(arr[j][i] == '\"')continue;
                    if(!isalpha(arr[j][i])){
                        printf("LINE %d : ERROR : ILLEGAL CHAR", lineNum);
                        errFlag = 1;
                        continue;
                    }
                    images.data_tail = addDataNode(images.data_tail);
                    if(images.data_tail == NULL){
                        printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
                        errFlag = 1;
                        continue;
                    } 
                    images.data_tail->data_line.item = arr[j][i];
                    printf(".string data_tail->item : %d\n", images.data_tail->data_line.item);
                    images.data_tail->data_line.empty_bit = 0;
                    images.data_tail->data_line.class.absolute = 1; /*not working well*/
                    images.data_tail->data_line.class.relocatable = 0;
                    images.data_tail->data_line.class.external = 0;
                    DC++;
                }
            }else if (!strcmp(arr[j], ".data")){
                if (symbolFlag)
                    addSymbol(name, IC, arr[j], lineNum, images);
                j++;
                num = isLegalNumber(arr[j]);
                if(num == 0){ /*not working well - jump to line 180*/
                    printf("LINE %d : ERROR : ILLEGAL NUMBER\n", lineNum);
                    errFlag = 1;
                    continue;
                }
                for(; j<dataNum; j++){
                    images.data_tail = addDataNode(images.data_tail);
                    if(images.data_tail == NULL){
                        printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
                        errFlag = 1;
                        continue;
                    } 
                    images.data_tail->data_line.item = num;
                    printf(".data data_tail->item : %d\n", images.data_tail->data_line.item);
                    images.data_tail->data_line.class.absolute = 1;
                    images.data_tail->data_line.class.relocatable = 0;
                    images.data_tail->data_line.class.external = 0;
                    DC++;
                }
                
            }else if (!strcmp(arr[j], ".entry"))
                continue;
            else if (!strcmp(arr[j], ".extern")){
                j++;
                if(!isLegalSymName(arr[j])){
                    printf("LINE %d : ERROR: THERE IS NO SYMBOL\n", lineNum);
                    errFlag = 1;
                    continue;
                }
                addSymbol(arr[j], 0, ".extern", lineNum, images);
                continue;
            } else{
                if (symbolFlag){
                    addSymbol(name, IC, ".code", lineNum, images);
                }
                operandsNum = isCommand(arr[j]);
                if (operandsNum == -1){
                    printf("LINE %d : ERROR: COMMAND NAME\n", lineNum);
                    errFlag = 1;
                    continue;
                }
                for (i = 0; i < MAX_CMD_NUM; i++){
                    cmd = &cmd_arr[i];
                    if ((strcmp(arr[j], cmd->cmdName)) == 0){                                       /*if arr[j] is command*/
                        images.code_tail = addCodeNode(images.code_tail);/*add the first word*/
                        if(images.code_tail == NULL){
                            printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
                            errFlag = 1;
                            continue;
                        } 
                        images.code_tail->code_line.command.opcode = cmd_arr[i].cmd_opcode;
                        images.code_tail->code_line.command.class.absolute = 1;
                        images.code_tail->code_line.command.class.relocatable = 0;
                        images.code_tail->code_line.command.class.external = 0;
                        L++;
                        if (i < 14){                                                           /*if the command have oprands */
                            images.code_tail->code_line.word.funct = cmd_arr[i].cmd_funct; /* add the second word*/
                            images.code_tail->code_line.word.class.absolute = 1;
                            L++;
                        }
                        j++;
                        break;
                    }
                }
                if (operandsNum > 0){ /*if the command have operand*/
                    addressing_mode = addressingModeFirstPass(arr[j++], 1, lineNum, images);
                    if (addressing_mode == immediate)
                        L++;
                    if (addressing_mode == direct || addressing_mode == index)
                        L += 2;
                    if (addressing_mode == -1){
                        errFlag = 1;
                        continue;
                    }
                    if (operandsNum == 2){ /*if the command have two operands*/
                        addressing_mode = addressingModeFirstPass(arr[j++], 1, lineNum, images);
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
        images.code_tail->code_line.count.ic = IC;
        images.code_tail->code_line.count.l = L;
        IC += L;
        lineLength = 0;
    }
    if(errFlag)
        return 0;
    ICF = IC;
    DCF = DC;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    sym = images.symbol_head;
    while (sym != NULL){
        if (!strcmp(sym->attributes, ".data")){
            sym->value += ICF;
            sym->offset = sym->value % 16;
            sym->baseAddress = sym->value - sym->offset;
        }
        sym = sym->next;
    }
    free(sym);
    return 1;
}

/*finds the addressing mode of the operand and add it to the code*/
int addressingModeFirstPass(char *operand, int src_or_dest , int lineNum, struct images images){
    enum addressingModes{immediate = 0, direct,index, register_direct};
    int state, addressing_mode = -1, i, num, len, j;
    char *copy;
    char symCopy[strlen(operand)], regCopy[strlen(operand)];
    symbol *node = images.symbol_head;
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
            return(printAndReturn("ERROR : ILLEGAL OPERAND\n",-1, lineNum));
        len = strlen(regCopy);
        regCopy[len - 1] = '\0';
        if (!(num = isRegister(regCopy)))
            return(printAndReturn("ERROR : ILLEGAL OPERAND\n", -1, lineNum));
        if (num < 10)
            return(printAndReturn("ERROR : ILLEGAL REGISTER NUMBER\n", -1, lineNum));
        addressing_mode = index;
    }
    if (src_or_dest == 1) /*if the operand is a source operand*/
        images.code_tail->code_line.word.src_address = addressing_mode;
    if (src_or_dest == 0) /*if the operand is a destination operand*/
        images.code_tail->code_line.word.dest_address = addressing_mode;
    switch (addressing_mode){/*write the code of the addressing mode*/
    case immediate:
        images.code_tail->code_line.imm_word.class.absolute = 1;
        images.code_tail->code_line.imm_word.word = num;
        break;
    case direct:
        /*wait til the second pass*/
        break;
    case index:
        /* wait til the second pass*/
    case register_direct:
        if (src_or_dest == 1)
            images.code_tail->code_line.word.src_register = num;
        if (src_or_dest == 0)
            images.code_tail->code_line.word.dest_register = num;
    default:
        break;
    }
    return addressing_mode;
}

void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN], int lineNum, struct images images){
    if (images.symbol_tail == NULL){
        images.symbol_tail = (symbol *)malloc(sizeof(symbol));
    }else{
        images.symbol_tail->next = (symbol *)malloc(sizeof(symbol));
        images.symbol_tail = images.symbol_tail->next;
    }
    if(images.symbol_tail == NULL){
        printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
        return;
    }
    strcpy(images.symbol_tail->symbol, symbolName);
    if (!strcmp(attribute, ".extern")){
        images.symbol_tail->value = 0;
        images.symbol_tail->baseAddress = 0;
        images.symbol_tail->offset = 0;
        strcat(images.symbol_tail->attributes, "external");
        return;
        }
    images.symbol_tail->value = IC;
    images.symbol_tail->baseAddress = (IC / 32) * 32;
    images.symbol_tail->offset = IC - images.symbol_tail->baseAddress;
    if (!strcmp(attribute, ".data") || !strcmp(attribute, ".string"))
        strcat(images.symbol_tail->attributes, "data");
    else
        strcat(images.symbol_tail->attributes, "code");
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

/*create new code node*/
code *addCodeNode(code *tail){
        tail->next = (code *)malloc(sizeof(code));
        tail = tail->next;
    return tail;
}

/*create a new data node*/
data *addDataNode(data *tail){
        tail->next = (data *)malloc(sizeof(data));
        tail = tail->next;
    return tail;
}