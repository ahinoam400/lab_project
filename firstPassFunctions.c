#include "firstPassFunctions.h"

int main(){
    char *arr[MAX_LINE_LEN];
    char test[100] = " .data 123";
    split(test, arr);
}

/*this function split the line into array*/
int split(char* str, char *arr[]){
    enum states{before, cmd_sym_data, after_param, after_command,
                in_operand, after_oprtand, comma};
    int stringIndex = 0, i = 0;
    int start = 0, end ,operandsNum = 0, oprandsCounter;
    int state = before;
    char ws[6] = "\t \n";
    strcat(str," "); /* Add extra space in order to process the last param*/
    while (str[stringIndex]){
        switch (state){
        case before:
            if (strchr(ws, str[stringIndex]))
                break;
            if(!isalpha(str[stringIndex]) && str[stringIndex]!='.')
                return(printAndReturn("ERROR : ILLEGAL CHAR", -1));
            start = stringIndex;
            state = cmd_sym_data;
            break;

        case cmd_sym_data:
            if (str[stringIndex] == ',')
                return(printAndReturn("ERROR : ILLEGAL COMMA", -1));
            if(strchr(ws, str[stringIndex])){
                str[stringIndex] = 0;
                arr[i] = str + start;
                operandsNum = isCommand(arr[i]);
                if((operandsNum > -1)){
                    state = after_command;
                    i++;
                }else{
                    state = after_param;
                    i++;
                }
            }
            break;

        case after_command:
            if(str[stringIndex] == ',')
                return(printAndReturn("ERROR : ILLEGAL COMMA", -1));
            if(!strchr(ws,str[stringIndex])){
                start = stringIndex;
                state = in_operand;
            }
            break;

        case after_param:
            if(str[stringIndex] == ',')
                return(printAndReturn("ERROR : ILLEGAL COMMA", -1));
            if(!strchr(ws,str[stringIndex])){
                start = stringIndex;
                state = cmd_sym_data;
            }
            break;

        case in_operand:
            if(strchr(ws,str[stringIndex])){
                str[stringIndex]=0;
                arr[i++] = str+start;
                state = after_param;
            }
            if(str[stringIndex] == ','){
                if(operandsNum <= oprandsCounter)
                    return(printAndReturn("ERROR : ILLEGAL COMMA", -1));
                str[stringIndex]=0;
                arr[i++] = str+start;
                state = comma;
                oprandsCounter++;
            }
            break;

        case comma:
            if(str[stringIndex] == ',')
                return(printAndReturn("ERROR : MULTIPLE CONSECUTIVE COMMAS", -1));
            if(strchr(ws,str[stringIndex])){
                break;
            }
            start = stringIndex;
            if(operandsNum <= oprandsCounter)
                return(printAndReturn("ERROR : EXCESS OPERAND", -1));
            state = in_operand;
            break;

        default:
            break;
        }
        stringIndex++;
    }
}

int addDataNode()
{
    tail_code->next = (code *)malloc(sizeof(code));
    if (tail_code->next == NULL)
        return(printAndReturn("ERROR : MEMORY ALLOCATION FAILED", 0));
    tail_code = tail_code->next;
    return 1;
}
int isNameInTable(char symbolName[MAX_LINE_LEN])
{
    symbol *sym = head;
    while (sym != NULL)
    {
        if (!strcmp(sym->symbol, symbolName))
            return 1;
        sym = sym->next;
    }
    return 0;
}

/*finds the addressing mode of the operand*/
int findAddressingMode(char *operand, int src_or_dest){
    enum addressingModes{immediate = 0, direct,index, register_direct};
    int state, addressing_mode = -1, i, num, len, j;
    char *copy;
    char symCopy[strlen(operand)], regCopy[strlen(operand)];
    symbol *node = head;

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
            return(printAndReturn("ERROR : ILLEGAL OPERAND",0));
        len = strlen(regCopy);
        regCopy[len - 1] = '\0';
        if (!(num = isRegister(regCopy)))
            return(printAndReturn("ERROR : ILLEGAL OPERAND", 0));
        if (num < 10)
            return(printAndReturn("ERROR : ILLEGAL REGISTER NUMBER", 0));
        addressing_mode = index;
    }
    if (addressing_mode < 0)
        return -1;
    if (src_or_dest == 0) /*if the operand is a source operand*/
        tail_code->code_line.word.src_address = decimalToBinary(addressing_mode);
    if (src_or_dest == 1) /*if the operand is a destination operand*/
        tail_code->code_line.word.dest_address = decimalToBinary(addressing_mode);
    switch (addressing_mode){
    case immediate:
        tail_code->code_line.imm_word.class.absolute = 1;
        tail_code->code_line.imm_word.word = decimalToBinary(num);
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
                    tail_code->code_line.word.src_register = decimalToBinary(num);
                if (src_or_dest == 1)
                    tail_code->code_line.word.dest_register = decimalToBinary(num);
            }
            node = node->next;
        }
    case register_direct:
        if (src_or_dest == 0)
            tail_code->code_line.word.src_register = decimalToBinary(num);
        if (src_or_dest == 1)
            tail_code->code_line.word.dest_register = decimalToBinary(num);
    default:
        break;
    }
}

int isCommand(char commandName[MAX_LINE_LEN]){
    command *cmd = (command *)malloc(sizeof(command));
    int index, cmp;
    for (index = 0; index < MAX_CMD_NUM; index++){
        cmd = &cmd_arr[index];
        if ((cmp = strcmp(commandName, cmd->cmdName)) == 0){
            addDataNode();
            tail_code->code_line.command.opcode = decimalToBinary(cmd_arr[index].cmd_opcode);
            tail_code->code_line.command.class.absolute = 1;
            if (index <= 14){
                tail_code->code_line.word.funct = decimalToBinary(cmd_arr[index].cmd_funct);
                tail_code->code_line.word.class.absolute = 1;
            }
            if(index < 6){
                L+=2;
                return 2;
            }
            if (index < 15){
                L+=2;
                return 1;
            }
            L+=1;
            return 0;
        }
        cmd = (command *)malloc(sizeof(command));
    }
    free(cmd);
    return -1;
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

/*convert decimal number to binary number*/
long int decimalToBinary(int decNum){
    int binaryNum[16];
    int i = 0, counter;
    long int reverse = 0;
    for (counter = 0; counter < 16; counter++)
        binaryNum[counter] = 0;

    while (decNum > 0){
        binaryNum[i] = decNum % 2;
        decNum = decNum / 2;
        i++;
    }
    for (i = i - 1; i >= 0; i--)
        reverse = 10 * reverse + binaryNum[i];
    return reverse;
}

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

int printAndReturn(char *str, int num){
    printf("%s", str);
    return num;
}