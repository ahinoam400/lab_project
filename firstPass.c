#include "firstPass.h"
extern command cmd_arr[];
int ICF, DCF;
int IC = BASE_ADDRESS, DC = 0, L = 0;
void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN], int lineNum, struct images *images);
int addressingModeFirstPass(char *operand, int src_or_dest , int lineNum, struct images *images, code *code_funct, int *l, int ic, char *cmdName);
int isLegalAddressingMode(int src_or_dest, char *cmd, int addrssingMode);

command cmd_arr[MAX_CMD_NUM]={
    {"mov",0,0, {0,1,2,3,-1}, {1,2,3,-1} ,2}, {"cmp",1,0, {0,1,2,3,-1}, {0,1,2,3,-1},2}, {"add",2,10, {0,1,2,3,-1}, {1,2,3,-1},2},
    {"sub",2,11, {0,1,2,3,-1}, {1,2,3,-1}, 2}, {"lea",4,0,{1,2,-1}, {1,2,3,-1},2}, {"clr",5,10, {-1}, {1,2,3,-1},1},
    {"not",5,11, {-1}, {1,2,3,-1},1}, {"inc",5,12, {-1}, {1,2,3,-1},1}, {"dec",5,13, {-1}, {1,2,3,-1},1},
    {"jmp",9,10, {-1}, {1,2,-1},1}, {"bne",9,11, {-1}, {1,2,-1},1}, {"jsr",9,12, {-1}, {1,2,-1},1},
    {"red",12,0, {-1}, {1,2,3,-1},1}, {"prn",13,0, {-1}, {0,1,2,3,-1},1}, {"rts",14,0, {-1},{-1},0},
    {"stop",15,0, {-1}, {-1},0}
};

int firstPass(const char *filename, struct images *images){
    enum addressingModes{ immediate = 0,direct,index,register_direct};
    char fileNameCopy[MAX_LINE_LEN];
    int lineLength, lineNum;
    int errFlag = 0, symbolFlag = 0;
    int num, i, operandsNum, j = 0, addressing_mode, dataNum, dataLoop;
    char line[MAX_LINE_LEN], *name, *ws = " \t";
    char firstChar = ' ';
    bool isEmptyLine = true;
    command *cmd;
    code *code_funct;
    symbol *sym;
    FILE *assembly;
    strcpy(fileNameCopy, filename);
    assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1, 0));
    cmd = (command *)malloc(sizeof(command));
    DC = 0, IC = BASE_ADDRESS, L=0 ,lineLength = 0, lineNum =0;
    while (fgets(line, MAX_LINE_LEN, assembly)){
        char *arr[MAX_LINE_LEN] = {0};
       /* printf(";%d %s\n", IC, line);*/
        lineNum++;
        L = 0 , symbolFlag = 0;
        dataNum = split(line, arr, lineNum); /*split the line into array*/
        if(dataNum == -1)continue;
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
                if (j!= 0){
                    printf("LINE %d : ERROR: ILLEGAL SYMBOL\n", lineNum);
                    errFlag = -1;
                    continue;
                }
                name = arr[j];
                name[strlen(name) - 1] = '\0'; /*remove the : from the symbol*/
                if (!isLegalSymName(name)){
                    printf("LINE %d : ERROR: ILLEGAL SYMBOL NAME\n", lineNum);
                    errFlag = -1;
                    continue;
                }
                if (isNameInTable(name, images->symbol_head)){
                    printf("LINE %d : ERROR: SYMBOL NAME ALREADY EXISTS\n" , lineNum);
                    errFlag = -1;
                    continue;
                }
                symbolFlag = 1;
                j++;
            }
            if (!strcmp(arr[j], ".string")){
                if (symbolFlag){
                    images->symbol_tail = addSymbolNode(images->symbol_tail, lineNum);
                    addSymbol(name, DC, ".string", lineNum, images);
                }
                j++;
                for (i = 0; arr[j][i] != '\0'; i++){
                    if(arr[j][i] == '\"')continue;
                    if(!isalpha(arr[j][i])){
                        printf("LINE %d : ERROR : ILLEGAL CHAR", lineNum);
                        errFlag = -1;
                        continue;
                    }
                    images->data_tail = addDataNode(images->data_tail, lineNum);
                    if(images->data_tail == NULL){
                        printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
                        errFlag = -1;
                        continue;
                    } 
                    images->data_tail->data_line.item = arr[j][i];
                    images->data_tail->data_line.empty_bit = 0;
                    images->data_tail->data_line.absolute = 1; 
                    DC++;
                }
                images->data_tail = addDataNode(images->data_tail, lineNum);
                if(images->data_tail == NULL){
                    printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
                    errFlag = -1;
                    continue;
                }
                images->data_tail->data_line.absolute = 1;
                DC++; 
            }else if (!strcmp(arr[j], ".data")){
                if (symbolFlag){
                    images->symbol_tail = addSymbolNode(images->symbol_tail, lineNum);
                    addSymbol(name, DC, ".data", lineNum, images);
                }
                j++;
                for(dataLoop=0; dataLoop<dataNum; dataLoop++,j++){
                    if(isLegalNumber(arr[j]) == -1){
                        printf("LINE %d : ERROR : ILLEGAL NUMBER\n", lineNum);
                        errFlag = -1;
                        continue;
                    }
                    num = atoi(arr[j]);
                    images->data_tail = addDataNode(images->data_tail, lineNum);
                    if(images->data_tail == NULL){
                        printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
                        errFlag = -1;
                        continue;
                    } 
                    images->data_tail->data_line.item = num;
                    images->data_tail->data_line.absolute = 1;
                    DC++;
                }
            }else if (!strcmp(arr[j], ".entry"))
                continue;
            else if (!strcmp(arr[j], ".extern")){
                j++;
                if(!isLegalSymName(arr[j])){
                    printf("LINE %d : ERROR: THERE IS NO SYMBOL\n", lineNum);
                    errFlag = -1;
                    continue;
                }
                images->symbol_tail = addSymbolNode(images->symbol_tail, lineNum);
                addSymbol(arr[j], IC, ".extern", lineNum, images);
                continue;
            } else{
                if (symbolFlag){
                    images->symbol_tail = addSymbolNode(images->symbol_tail, lineNum);
                    addSymbol(name, IC, ".code", lineNum, images);
                }
                operandsNum = isCommand(arr[j]);
                if (operandsNum == -1){
                    printf("LINE %d : ERROR: COMMAND NAME\n", lineNum);
                    errFlag = -1;
                    continue;
                }
                cmd = getCommandByName(arr[j]);
                images->code_tail = addCodeNode(images->code_tail);/*add the first word*/
                if(images->code_tail == NULL){
                    printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
                    errFlag = -1;
                    continue;
                } 
                images->code_tail->code_line.command.opcode = 1<<(cmd->cmd_opcode);
                images->code_tail->code_line.command.absolute = 1;
                L++;
                if (cmd->operandsNum>0){/*if the command have oprands */
                    images->code_tail = addCodeNode(images->code_tail);/*add the first word*/
                    if(images->code_tail == NULL){
                        printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
                        errFlag = -1;
                        continue;
                    } 
                    images->code_tail->code_line.word.funct = cmd->cmd_funct; /* add the second word*/
                    images->code_tail->code_line.word.absolute = 1;
                    L++;
                }
                j++;
                code_funct = images->code_tail; /* save thee pointer to the original funct instuction */
                if (operandsNum > 0){ /*if the command have operands*/
                    /* process first operand */
                    addressing_mode = addressingModeFirstPass(arr[j++], operandsNum-1, lineNum, images, code_funct, &L, IC, cmd->cmdName);
                    if (addressing_mode == -1){
                        errFlag = -1;
                        continue;
                    }
                    if (operandsNum == 2){ /*if the command have two operands*/
                        addressing_mode = addressingModeFirstPass(arr[j++], 0, lineNum, images, code_funct, &L, IC, cmd->cmdName);
                        if (addressing_mode == -1){
                            errFlag = -1;
                            continue;
                        }
                    }
                }
            }
        }        
        IC += L;
        lineLength = 0;
    }
    if(errFlag==-1)
        return -1;
    ICF = IC;
    DCF = DC;
    sym = images->symbol_head;
    while (sym != NULL){
        if (!strcmp(sym->attributes, "data")){
            sym->value += ICF;
            sym->offset = sym->value % 16;
            sym->baseAddress = sym->value - sym->offset;
        }
        sym = sym->next;
    }
    images->ICF = ICF;
    images->DCF = DCF;
    free(sym);
    return 0;
}

/*finds the addressing mode of the operand and add it to the code*/
int addressingModeFirstPass(char *operand, int src_or_dest , int lineNum, struct images *images, code *code_funct, int *l, int ic, char *cmdName){
    enum addressingModes{immediate = 0, direct,index, register_direct};
    int addressing_mode = -1, i, num, len, k;
    char *copy;
    char symCopy[MAX_LINE_LEN], regCopy[MAX_LINE_LEN];
    /*find the addresing mode*/
    if (operand[0] == '#'){
        copy = operand + 1;
        if(isLegalNumber(copy) == 0){
            addressing_mode = immediate;
            num = atoi(copy);
        }
    }else if(isRegister(operand)){
        num = isRegister(operand);
        addressing_mode = register_direct;
    }else if(isLegalSymName(operand)){
        addressing_mode = direct;
    }else {
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
    if(isLegalAddressingMode(src_or_dest, cmdName, addressing_mode) == -1){
        return(printAndReturn("ERROR : ILLEGAL ADDRESSING MODE", -1, lineNum));
    }
    if (src_or_dest == 1) /*if the operand is a source operand*/
        code_funct->code_line.word.src_address = addressing_mode;
    if (src_or_dest == 0) /*if the operand is a destination operand*/
        code_funct->code_line.word.dest_address = addressing_mode;
    switch (addressing_mode){/*write the code of the addressing mode*/
    case immediate:
        images->code_tail = addCodeNode(images->code_tail, lineNum);/*add the first word*/
        if(images->code_tail == NULL){
            return(printAndReturn("ERROR : MEMORY ALLOCATION FAILED",-1, lineNum));
        } 
        (*l)++;
        images->code_tail->code_line.imm_word.absolute = 1;
        images->code_tail->code_line.imm_word.word = num;
        break;
    case direct:
        for(k=0; k<2; k++){
            images->code_tail = addCodeNode(images->code_tail, lineNum);/*add the first word*/
            if(images->code_tail == NULL){
                return(printAndReturn("ERROR : MEMORY ALLOCATION FAILED",-1, lineNum));
            } 
            images->code_tail->ic = ic;
            images->code_tail->l = *l;
            (*l)++;
        }
        /*wait til the second pass*/
        break;
    case index:
        if (src_or_dest == 1)
            code_funct->code_line.word.src_register = num;
        if (src_or_dest == 0)
            code_funct->code_line.word.dest_register = num;
        for(k=0; k<2; k++){
            images->code_tail = addCodeNode(images->code_tail, lineNum);/*add the first word*/
            if(images->code_tail == NULL){
                return(printAndReturn("ERROR : MEMORY ALLOCATION FAILED",-1, lineNum));
            }
            images->code_tail->ic = ic;
            images->code_tail->l = *l;
            (*l)++; 
        }
        /* wait til the second pass*/
    case register_direct:
        if (src_or_dest == 1)
            code_funct->code_line.word.src_register = num;
        if (src_or_dest == 0)
            code_funct->code_line.word.dest_register = num;
    default:
        break;
    }
    return addressing_mode;
}

/*adds a new symbol to the symbol table*/
void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN], int lineNum, struct images *images){
    if(images->symbol_tail == NULL){
        printf("LINE %d : ERROR : MEMORY ALLOCATION FAILED", lineNum);
        return;
    }
    strcpy(images->symbol_tail->symbol, symbolName);
    if (!strcmp(attribute, ".extern")){
        images->symbol_tail->value = 0;
        images->symbol_tail->baseAddress = 0;
        images->symbol_tail->offset = 0;
        strcat(images->symbol_tail->attributes, "external");
        return;
        }
    images->symbol_tail->value = IC;
    images->symbol_tail->baseAddress = (IC / 32) * 32;
    images->symbol_tail->offset = IC - images->symbol_tail->baseAddress;
    if (!strcmp(attribute, ".data") || !strcmp(attribute, ".string"))
        strcat(images->symbol_tail->attributes, "data");
    else
        strcat(images->symbol_tail->attributes, "code");
}

/*checks if the operands' addressing mode is legal*/
int isLegalAddressingMode(int src_or_dest, char *cmd, int addrssingMode){
    int i, j;
    for (i=0; i<MAX_CMD_NUM; i++){
        if(strcmp(cmd, cmd_arr[i].cmdName) == 0){
            if(src_or_dest == 1){
                for(j=0; cmd_arr[i].srcAddressingModes[j]!=-1; j++)
                    if(addrssingMode == cmd_arr[i].srcAddressingModes[j])
                        return  0;
            }else if(src_or_dest == 0){
                for(j=0; cmd_arr[i].destAddressingModes[j]!=-1; j++)
                    if(addrssingMode == cmd_arr[i].destAddressingModes[j])
                        return 0;
            }
        }
    }
    return -1;
}

/*create new code node*/
code *addCodeNode(code *tail, int lineNum){
    tail->next = (code *)malloc(sizeof(code));
    if(tail->next == NULL) return printAndReturn("ERROR : MEMORY ALLOCATION FAILED", NULL, lineNum);
    tail = tail->next;
    memset(tail,0,sizeof(code));
    return tail;
}

/*create a new data node*/
data *addDataNode(data *tail, int lineNum){
    tail->next = (data *)malloc(sizeof(data));
    if(tail->next == NULL) return printAndReturn("ERROR : MEMORY ALLOCATION FAILED", NULL, lineNum);
    tail = tail->next;
    memset(tail,0,sizeof(data));
    return tail;
}

/*create a new symbol node*/
symbol *addSymbolNode(symbol *tail, int lineNum){
    tail->next = (symbol *)malloc(sizeof(symbol));
    if(tail->next == NULL) return printAndReturn("ERROR : MEMORY ALLOCATION FAILED", NULL, lineNum);
    tail = tail->next;
    memset(tail,0,sizeof(symbol));
    return tail;
}