#include "secondPass.h"

int secondPass(char *filename, code *code_tail , code *code_head , symbol *symbol_head ,symbol *symbol_tail){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL){
        printf("ERROR OPENING FILE\n");
        return -1;
    }
    char line[MAX_LINE_LEN];
    int errFlag = 0;
    int i=0, operandsNum, addressingMode;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    while(fgets(line, MAX_LINE_LEN, assembly)){
        char *arr[MAX_LINE_LEN];
        split(line, arr);
        if(!isLegalSymName(arr[i]))
            continue;
        if(!(strcmp(arr[i], ".data"))||!(strcmp(arr[i], ".string"))||!(strcmp(arr[i], ".extern")))
            continue;
        if(!strcmp(arr[i], ".entry")){
            i++;
            if(!isNameInTable(arr[i], symbol_head)){
                errFlag = 1;
                printf("ERROR: NAME IS NOT IN TABLE\n");
                continue;
            }
            sym = head;
            while(sym != NULL){
                if(!strcmp(sym->symbol, arr[i])){
                    strcat(sym->attributes, ", entry");
                    break;
                }
                sym = sym->next;
            }
            continue;
        }
        if(arr[i][strlen(arr[i])] == ':')i++;
        if(operandsNum = isCommand(arr[i]))i++;
        if(operandsNum >= 1){
            addressingMode =  adressingModeSecondPass(arr[i++], symbol_head, code_head);
            if(addressingMode == -1){
                errFlag = 1;
                continue;
            }
            if(operandsNum == 2){
                addressingMode = adressingModeSecondPass(arr[i++], symbol_head, code_head);
                if(addressingMode == -1){
                    errFlag = 1;
                    continue;
                }
            }
        }
    }
    return !errFlag;
}

/*finds the addressing mode of the operand and add it to the code*/
int adressingModeSecondPass(char *operand, symbol *sym_head, code *code_haed){
    enum addressingModes{immediate =0, register_direct, direct,index};
    int state, addressing_mode = -1, i, num, len, j;
    char *copy;
    char symCopy[strlen(operand)], regCopy[strlen(operand)];
    symbol *node = sym_head;
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
    switch (addressing_mode){/*write the code of the addressing mode*/
    case immediate:
        break;
    case register_direct:
        break;
    case direct:
        while (node != NULL){
            if (!strcmp(node->symbol, operand)){
                if (!strcmp(node->attributes, "external")){
                    code_tail->code_line.dir_words.class.external = 1;
                    code_tail->code_line.dir_words.class_2.external = 1;
                    break;
                }
                code_tail->code_line.dir_words.base_address = node->baseAddress;
                code_tail->code_line.dir_words.offset = node->offset;
                code_tail->code_line.dir_words.class.relocatable = 1;
                code_tail->code_line.dir_words.class_2.relocatable = 1;
            }
            node = node->next;
        }
        break;
    case index:
        while (node != NULL){
            if (!strcmp(node->symbol, symCopy)){
                if (!strcmp(node->attributes, "external")){
                    code_tail->code_line.inx_words.class.external = 1;
                    code_tail->code_line.inx_words.class_2.external = 1;
                    break;
                }
                code_tail->code_line.inx_words.base_address = node->baseAddress;
                code_tail->code_line.inx_words.offset = node->offset;
                code_tail->code_line.inx_words.class.relocatable = 1;
                code_tail->code_line.inx_words.class_2.relocatable = 1;
                if (src_or_dest == 0)
                    code_tail->code_line.word.src_register = num;
                if (src_or_dest == 1)
                    code_tail->code_line.word.dest_register = num;
            }
            node = node->next;
        }
    default:
        break;
    }
}