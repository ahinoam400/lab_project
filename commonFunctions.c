#include "commonFunctions.h"

int isNameInTable(char symbolName[MAX_LINE_LEN], symbol *head){
    symbol *sym = head;
    while(sym != NULL){
        if(!strcmp(sym->symbol, symbolName))
            return 1;
        sym = sym->next;
    }
    return 0;
}

/*this function split the line str into array*/
int split(char *str, char *arr[], int lineNum){
    enum states{before,cmd_sym_data,after_param,after_command,
                after_data,in_operand, after_oprtand, comma};
    int stringIndex = 0, i = 0;
    int start = 0, operandsNum = 0, operandsCounter = 0, dataCounter = 0, cmd_or_data;
    int state = before;
    char ws[6] = "\t \n";
    strcat(str, " "); /* Add extra space in order to process the last param*/
    while (str[stringIndex]){
        switch (state){
        case before:
            if (strchr(ws, str[stringIndex]))
                break;
            if (!isalpha(str[stringIndex]) && str[stringIndex] != '.' && str[stringIndex] != ';')
                return (printAndReturn("ERROR : ILLEGAL CHAR", -1, lineNum));
            start = stringIndex;
            state = cmd_sym_data;
            break;

        case cmd_sym_data: /*if the word is a symbol, a data or a command*/
            if(str[stringIndex] == ',')
                return(printAndReturn("ERROR : ILLEGAL COMMA", -1, lineNum));
            if (strchr(ws, str[stringIndex])){
                str[stringIndex] = 0;
                arr[i] = str + start;
                operandsNum = isCommand(arr[i]);
                if (operandsNum > -1){
                    cmd_or_data = 1;
                    state = after_command;
                }else if (!strcmp(arr[i], ".data")){
                    cmd_or_data = 0;
                    state = after_data;
                }else{
                state = after_param;
                }
                i++;
            }
        break;

        case after_command:
            if (str[stringIndex] == ',')
                return (printAndReturn("ERROR : ILLEGAL COMMA", -1, lineNum));
            
            if (!strchr(ws, str[stringIndex])){
                start = stringIndex;
                state = in_operand;
                operandsCounter++;
            }
            break;

        case after_data:
            if (str[stringIndex] == ',')
                return(printAndReturn("ERROR : ILLEGAL COMMA", -1, lineNum));
            if (!strchr(ws, str[stringIndex])){
                start = stringIndex;
                state = in_operand;
                dataCounter++;
            }
            break;

        case after_param:
            if (str[stringIndex] == ','){
                if (operandsNum <= operandsCounter && cmd_or_data ==1)
                    return (printAndReturn("ERROR : ILLEGAL COMMA", -1, lineNum));
                if(dataCounter < 1 && cmd_or_data == 0)
                    return (printAndReturn("ERROR : ILLEGAL COMMA", -1, lineNum));
                state = comma;
                break;
            }
            if (!strchr(ws, str[stringIndex])){
                if (operandsNum > operandsCounter){
                    return (printAndReturn("ERROR : MISSING COMMA", -1, lineNum));
                }
                start = stringIndex;
                state = cmd_sym_data;
            }
            break;

        case in_operand:
            if (operandsNum < operandsCounter && cmd_or_data)
                return (printAndReturn("ERROR : EXCESS OPERAND", -1, lineNum));
            if (strchr(ws, str[stringIndex])){
                str[stringIndex] = 0;
                arr[i++] = str + start; /*not working well for the last param */
                state = after_param;
            }
            if (str[stringIndex] == ','){
                if(cmd_or_data && operandsNum <= operandsCounter)
                    return (printAndReturn("ERROR : ILLEGAL COMMA", -1, lineNum));
                if((!cmd_or_data && dataCounter < 1) || (!cmd_or_data && dataCounter == 1 && str[stringIndex+1] =='\n'))
                    return (printAndReturn("ERROR : ILLEGAL COMMA", -1, lineNum));
                str[stringIndex] = 0;
                arr[i++] = str + start;
                state = comma;
            }
            break;

        case comma:
            if (str[stringIndex] == ',')
                return (printAndReturn("ERROR : MULTIPLE CONSECUTIVE COMMAS", -1, lineNum));
            if (strchr(ws, str[stringIndex])){
                break;
            }
            if (cmd_or_data == 1)
                operandsCounter++;
            if (cmd_or_data == 0)
                dataCounter++;
            start = stringIndex;
            if (operandsNum < operandsCounter && cmd_or_data == 1)
                return (printAndReturn("ERROR : EXCESS OPERAND", -1, lineNum));
            state = in_operand;
            break;

        default:
            break;
        }
    stringIndex++;
    }
    if(cmd_or_data == 1 && operandsCounter < operandsNum)
        return(printAndReturn("ERROR : MISSIMG OPERAND" , -1, lineNum));
    if(cmd_or_data == 0) return dataCounter;
    return 0;
}



/*print str and return num*/
int printAndReturn(char *str, int num, int lineNum){
    printf("LINE %d : %s\n", lineNum, str);
    return num;
}

/*this function checks if commandName is a command. returns the number
of operands of the command or -1 if commandName is not command*/
int isCommand(char commandName[MAX_LINE_LEN]){
    command *cmd = (command *)malloc(sizeof(command));
    int index, cmp;
    for (index = 0; index < MAX_CMD_NUM; index++){
        cmd = &cmd_arr[index];
        if ((cmp = strcmp(commandName, cmd->cmdName)) == 0){ /*if commandName is command*/
            if (index < 5)
                return 2;
            if (index < 14)
                return 1;
            return 0;
        }
        cmd = (command *)malloc(sizeof(command));
    }
    free(cmd);
    return -1;
}

/*checks if num is a legal number */
int isLegalNumber(char *number){
    int i = 0;
    if (number[i] == '-' || number[i] == '+')
        i++;
    while (number[i] != '\0' && number[i] != '\n'){
        if (!isdigit(number[i]))
            return 0;
        i++;
    }
    return atoi(number);
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
void print_code(code *p_code){
    fprint_code(p_code, stdout);
}

void fprint_code(code* p_code, FILE *stream) {
    int ic = 100;
    char hexWord[32];   
    p_code = p_code->next;
    while (p_code) {
        sprintf(hexWord, "%05x",p_code->code_line.bytes);
        fprintf(stream, "%d\tA%c-B%c-C%c-D%c-E%c\n", ic++, hexWord[0], hexWord[1], hexWord[2], hexWord[3], hexWord[4]);
        p_code = p_code->next;
    }
}

void print_symbol(symbol* p_symbol) {
    while (p_symbol) {
        printf("name: %s, base address: %d, offset: %d, attributes: %s, value: %d\n", 
               p_symbol->symbol, p_symbol->baseAddress, p_symbol->offset,
               p_symbol->attributes, p_symbol->value);
        p_symbol = p_symbol->next;
    }
}

void print_data(data *p_data, int icf){
    fprint_data(p_data, icf, stdout);
}
void fprint_data(data *p_data, int icf, FILE *stream){
    unsigned int *bytes;
    char hexWord[32];
    p_data = p_data->next;
    while(p_data){
        bytes = (unsigned int *) &p_data->data_line;
        sprintf(hexWord, "%05x", *bytes);
        fprintf(stream , "%d\tA%c-B%c-C%c-D%c-E%c\n" , icf++ ,hexWord[0] ,hexWord[1] ,hexWord[2] ,hexWord[3] ,hexWord[4]);
        p_data = p_data->next;
    }
}


