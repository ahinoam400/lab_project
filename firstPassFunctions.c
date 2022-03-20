#include "firstPassFunctions.h"

int main(){
    return 0;
}

/*finds the addressing mode of the operand and add it to the code*/
int findAddressingMode(char *operand, int src_or_dest, symbol head){
    enum addressingModes{immediate = 0, direct,index, register_direct};
    int state, addressing_mode = -1, i, num, len, j;
    char *copy;
    char symCopy[strlen(operand)], regCopy[strlen(operand)];
    symbol *node = head;
    /*find the addresing mode*/
    if (operand[0] == '#'){
        copy = operand + 1;
        if (num = isLegalNumber(copy)){
            addressing_mode = immediate;
            L++;
        }
    }else if (isRegister(operand)){
        num = isRegister(operand);
        addressing_mode = register_direct;
    }else if (isLegalSymName(operand)){
        addressing_mode = direct;
        L+=2;
    }else{
        strcpy(symCopy, operand);
        for (i = 0; symCopy[i] != '\0' && symCopy[i] != '['; i++);
        symCopy[i - 1] = '\0';
        strcpy(regCopy, operand + i + 1);
        if (regCopy[strlen(regCopy) - 1] != ']' || !isLegalSymName(symCopy))
            return(printAndReturn("ERROR : ILLEGAL OPERAND",0));
        len = strlen(regCopy);
        regCopy[len - 1] = '\0';
        if (!(num = isRegister(regCopy)))
            return(printAndReturn("ERROR : ILLEGAL OPERAND", 0));
        if (num < 10)
            return(printAndReturn("ERROR : ILLEGAL REGISTER NUMBER", 0));
        addressing_mode = index;
        L+=2;
    }
    if (addressing_mode < 0)
        return -1;
    if (src_or_dest == 0) /*if the operand is a source operand*/
        tail_code->code_line.word.src_address = addressing_mode;
    if (src_or_dest == 1) /*if the operand is a destination operand*/
        tail_code->code_line.word.dest_address = addressing_mode;
    switch (addressing_mode){/*write the code of the addressing mode*/
    case immediate:
        tail_code->code_line.imm_word.class.absolute = 1;
        tail_code->code_line.imm_word.word = num;
        break;
    case direct:
        while (node != NULL){
            if (!strcmp(node->symbol, operand)){
                if (!strcmp(node->attributes, "external")){
                    tail_code->code_line.dir_words.class.external = 1;
                    tail_code->code_line.dir_words.class_2.external = 1;
                    break;
                }
                tail_code->code_line.dir_words.base_address = node->baseAddress;
                tail_code->code_line.dir_words.offset = node->offset;
                tail_code->code_line.dir_words.class.relocatable = 1;
                tail_code->code_line.dir_words.class_2.relocatable = 1;
            }
            node = node->next;
        }
        break;
    case index:
        while (node != NULL){
            if (!strcmp(node->symbol, symCopy)){
                if (!strcmp(node->attributes, "external")){
                    tail_code->code_line.inx_words.class.external = 1;
                    tail_code->code_line.inx_words.class_2.external = 1;
                    break;
                }
                tail_code->code_line.inx_words.base_address = node->baseAddress;
                tail_code->code_line.inx_words.offset = node->offset;
                tail_code->code_line.inx_words.class.relocatable = 1;
                tail_code->code_line.inx_words.class_2.relocatable = 1;
                if (src_or_dest == 0)
                    tail_code->code_line.word.src_register = num;
                if (src_or_dest == 1)
                    tail_code->code_line.word.dest_register = num;
            }
            node = node->next;
        }
    case register_direct:
        if (src_or_dest == 0)
            tail_code->code_line.word.src_register = num /*decimalToBinary(num)*/;
        if (src_or_dest == 1)
            tail_code->code_line.word.dest_register = num /*decimalToBinary(num)*/;
    default:
        break;
    }
}


void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN]){
    if (head == NULL){
        head = (symbol *)malloc(sizeof(symbol));
        strcpy(head->symbol, symbolName);
        if (!strcmp(attribute, ".extern")){
            head->value = 0;
            head->baseAddress = 0;
            head->offset = 0;
            strcat(head->attributes, "external");
            tail = (symbol *)malloc(sizeof(symbol));
            head->next = tail;
            return;
        }
        head->value = IC;
        head->baseAddress = (IC / 32) * 32;
        head->offset = IC - head->baseAddress;
        if (!strcmp(attribute, ".data") || !strcmp(attribute, ".string"))
            strcat(head->attributes, "data");
        else
            strcat(head->attributes, "code");
        tail = (symbol *)malloc(sizeof(symbol));
        head->next = tail;
    }else{
        tail = (symbol *)malloc(sizeof(symbol));
        strcpy(tail->symbol, symbolName);
        if (!strcmp(attribute, ".extern")){
            tail->value = 0;
            tail->baseAddress = 0;
            tail->offset = 0;
            strcat(tail->attributes, "external");
            symbol *temp = (symbol *)malloc(sizeof(symbol));
            tail->next = temp;
            return;
        }
        tail->value = IC;
        tail->baseAddress = (IC / 32) * 32;
        tail->offset = IC - tail->baseAddress;
        if (!strcmp(attribute, ".data") || !strcmp(attribute, ".string"))
            strcat(tail->attributes, "data");
        else
            strcat(tail->attributes, "code");
        symbol *temp = (symbol *)malloc(sizeof(symbol));
        tail->next = temp;
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
