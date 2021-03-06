#include "commonFunctions.h"
int adressingModeSecondPass(char *operand, struct images *images, code *funct, int lineNum, int src_or_dest);
external_words *addExtNode(struct images *images, int lineNum);

int secondPass(const char *filename, struct images *images){
    char fileNameCopy[MAX_LINE_LEN];
    char line[MAX_LINE_LEN];
    int errFlag = 0, lineLength, lineNum = 0;
    char firstChar = ' ', *ws = " \t";
    bool isEmptyLine = true;
    int i=0, j, operandsNum, addressingMode;
    symbol *sym;
    code *funct;
    command *cmd;
    FILE *assembly;
    strcpy(fileNameCopy, filename);
    assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL){
        return(printAndReturn("ERROR OPENING FILE", -1, 0));
    }
    images->code_tail = images->code_head->next; /*in the second pass we used the code_tail as a pointer to the current record*/
    while(fgets(line, MAX_LINE_LEN, assembly)){
        char *arr[MAX_LINE_LEN] = {0};
        i=0;
        lineNum++;
        lineLength = strlen(line);
        line[lineLength] = '\0';
        for (j = 0; i < lineLength; j++){ /*checks if the line is empty line*/
            if(strchr("\n", line[j])){
                isEmptyLine = true;
                break;
            }
            if (!strchr(ws, line[j])){
                isEmptyLine = false;
                firstChar = line[j];
                break;
            }
        }
        if(firstChar == ';' || isEmptyLine)continue;
        split(line, arr, lineNum);
        if(!(strcmp(arr[i], ".data"))||!(strcmp(arr[i], ".string"))||!(strcmp(arr[i], ".extern")))
            continue;
        if(!strcmp(arr[i], ".entry")){
            i++;
            sym = getSymbolByName(images->symbol_head, arr[i]);
            if(!sym){
                errFlag = -1;
                printf("LINE %d : ERROR: NAME IS NOT IN TABLE\n", lineNum);
                continue;
            }
            sym->isEntry = 1;
            continue;
        }
        if(arr[i][strlen(arr[i])-1] == ':')
            i++;
        if(!(strcmp(arr[i], ".data"))||!(strcmp(arr[i], ".string"))||!(strcmp(arr[i], ".extern")))
            continue;
        cmd = getCommandByName(arr[i]);
        operandsNum = cmd?cmd->operandsNum:-1;
        if(operandsNum > 0)
            i++;
        images->code_tail = images->code_tail->next;
        if(operandsNum > 0){
            funct = images->code_tail;
            addressingMode =  adressingModeSecondPass(arr[i++], images, funct, lineNum, operandsNum-1);
            if(addressingMode == -1)
                errFlag = -1;
            if(operandsNum == 2){
                addressingMode = adressingModeSecondPass(arr[i++], images, funct, lineNum, 0);
                if(addressingMode == -1)
                    errFlag = -1;
            }
            images->code_tail = images->code_tail->next;
        }
    }
    return errFlag;
}

/*finds the addressing mode of the operand and add it to the code*/
int adressingModeSecondPass(char *operand, struct images *images, code *funct, int lineNum, int src_or_dest){
    enum addressingModes{immediate =0, direct,index, register_direct};
    int addressing_mode = -1, i;
    char symCopy[MAX_LINE_LEN];
    symbol *node = images->symbol_head;
    addressing_mode = src_or_dest?
                        funct->code_line.word.src_address:
                        funct->code_line.word.dest_address;
    switch (addressing_mode){/*write the code of the addressing mode*/
    case immediate:
        images->code_tail = images->code_tail->next;
        break;
    case register_direct:
        break;
    case direct:
        while (node != NULL){
            if (!strcmp(node->symbol, operand)){
                if (!strcmp(node->attributes, "external")){
                    addExtNode(images, lineNum); /*create a new external node*/
                    images->code_tail = images->code_tail->next;
                    images->code_tail->code_line.dir_word_1.external = 1;
                    images->ext_tail->ext_word.base_address = images->code_tail->ic + images->code_tail->l;
                    images->ext_tail->ext_word.symbol = node->symbol;
                    images->code_tail = images->code_tail->next;
                    images->ext_tail->ext_word.offset = images->code_tail->ic + images->code_tail->l;
                    images->code_tail->code_line.dir_word_2.external_2 = 1;
                }else{
                    images->code_tail = images->code_tail->next;
                    images->code_tail->code_line.dir_word_1.base_address = node->baseAddress;
                    images->code_tail->code_line.dir_word_1.relocatable = 1;
                    images->code_tail = images->code_tail->next;
                    images->code_tail->code_line.dir_word_2.offset = node->offset;
                    images->code_tail->code_line.dir_word_2.relocatable_2 = 1;
                }
                break;
            }
            node = node->next;
        }
        if(!getSymbolByName(images->symbol_head, operand)){/*if operand not in the symbol table*/
            images->code_tail = images->code_tail->next;
            images->code_tail = images->code_tail->next;
            return(printAndReturn("ERROR: NAME IS NOT IN TABLE", -1, lineNum));
        }
        break;
    case index:
        strcpy(symCopy, operand);
        for (i = 0; symCopy[i] != '\0' && symCopy[i] != '['; i++);
        symCopy[i] = '\0';
        if(!getSymbolByName(images->symbol_head, symCopy)){/*if operand not in the symbol table*/
            images->code_tail = images->code_tail->next;
            images->code_tail = images->code_tail->next;
            return(printAndReturn("ERROR: NAME IS NOT IN TABLE", -1, lineNum));
        }
        while (node != NULL){
            if (!strcmp(node->symbol, symCopy)){
                if (!strcmp(node->attributes, "external")){
                    addExtNode(images, lineNum);/*create a new external node*/
                    images->ext_tail->ext_word.base_address = images->code_tail->ic + images->code_tail->l;
                    images->ext_tail->ext_word.offset = images->code_tail->ic + images->code_tail->l+1;
                    images->ext_tail->ext_word.symbol = node->symbol;
                    images->code_tail = images->code_tail->next;
                    images->code_tail->code_line.inx_word_1.external = 1;
                    images->code_tail = images->code_tail->next;
                    images->code_tail->code_line.inx_word_2.external_2 = 1;
                }else{
                    images->code_tail = images->code_tail->next;
                    images->code_tail->code_line.inx_word_1.base_address = node->baseAddress;
                    images->code_tail->code_line.inx_word_1.relocatable = 1;
                    images->code_tail = images->code_tail->next;
                    images->code_tail->code_line.inx_word_2.offset = node->offset;
                    images->code_tail->code_line.inx_word_2.relocatable_2 = 1;
                }
            }
            node = node->next;
        }
    default:
        break;
    }
    return 0;
}

/*create a new external node in the external table*/
external_words *addExtNode(struct images *images, int lineNum){
    images->ext_tail->next = (external_words *)malloc(sizeof(external_words));
    if(images->ext_tail->next == NULL){
        printf("LINE : %d ERROR : MEMORY ALLOCATION FAILED", lineNum);
        return NULL;
    }
    images->ext_tail = images->ext_tail->next;
    memset(images->ext_tail,0,sizeof(external_words));
    return images->ext_tail;
}