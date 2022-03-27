#include "commonFunctions.h"

/*this function split the line str into array*/
int split(char *str, char *arr[], int lineNum){
    enum states{before,cmd_sym_data,after_param,after_command,
                after_data,in_operand, after_oprtand, comma};
    int stringIndex = 0, i = 0;
    int start = 0, operandsNum = 0, operandsCounter = 0, dataCounter = 0, cmd_or_data;
    int state = before;
    char ws[6] = "\t \n";
    command *cmd;
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
                cmd = getCommandByName(arr[i]);
                operandsNum = cmd?cmd->operandsNum:-1;
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

/*this function searchs a command name in cmd_arr and returns its address
if it exists and NULL else*/
command* getCommandByName(char *cmdName){
    int index;
    for (index = 0; index < MAX_CMD_NUM; index++){
        if ((strcmp(cmdName, cmd_arr[index].cmdName)) == 0){ /*if commandName is command*/
            return &cmd_arr[index];
        }
    }
    return NULL;
}

/*this function searchs a symbol in the symbol list and returns its address
if it exists and NULL else*/
symbol* getSymbolByName(symbol *sym_head, char *symName){
    symbol *sym = sym_head;
    while(sym != NULL){
        if ((strcmp(symName, sym->symbol)) == 0){ /*if symName is symbol*/
            return sym;
        }
        sym = sym->next;
    }
    return NULL;
}

/*checks if num is a legal number */
int isLegalNumber(char *number){
    int i = 0;
    if (number[i] == '-' || number[i] == '+')
        i++;
    while (number[i] != '\0' && number[i] != '\n'){
        if (!isdigit(number[i]))
            return -1;
        i++;
    }
    return 0;
}

/*this function checks if symbolName is a legal name for a symbol*/
int isLegalSymName(char symbolName[MAX_LINE_LEN]){
    int i;
    if (getCommandByName(symbolName) || isRegister(symbolName))
        return 0;
    for (i = 0; symbolName[i] != '\0'; i++){
        if (!isalpha(symbolName[i]) && !isdigit(symbolName[i]))
            return 0;
    }
    return 1;
}

/*Checks if str is a register and retrun the register number*/
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
/*prints the given code in the special base method*/
void fprint_code(code* p_code, FILE *stream) {
    int ic = 100;
    char hexWord[32];   
    p_code = p_code->next;
    while (p_code) {
        sprintf(hexWord, "%05x",p_code->code_line.bytes);
        fprintf(stream, "%04d\tA%c-B%c-C%c-D%c-E%c\n", ic++, hexWord[0], hexWord[1], hexWord[2], hexWord[3], hexWord[4]);
        p_code = p_code->next;
    }
}

void print_data(data *p_data, int icf){
    fprint_data(p_data, icf, stdout);
}
/*prints the given data in the special base method*/
void fprint_data(data *p_data, int icf, FILE *stream){
    unsigned int *bytes;
    char hexWord[32];
    p_data = p_data->next;
    while(p_data){
        bytes = (unsigned int *) &p_data->data_line;
        sprintf(hexWord, "%05x", *bytes);
        fprintf(stream , "%04d\tA%c-B%c-C%c-D%c-E%c\n" , icf++ ,hexWord[0] ,hexWord[1] ,hexWord[2] ,hexWord[3] ,hexWord[4]);
        p_data = p_data->next;
    }
}


