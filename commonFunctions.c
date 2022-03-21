#include "commonFunctions.h"

int isNameInTable(char symbolName[MAX_LINE_LEN], symbol *head){
    symbol *sym = head;
    while (sym != NULL){
        if (!strcmp(sym->symbol, symbolName))
            return 1;
        sym = sym->next;
    }
    return 0;
}

/*this function split the line str into array*/
int split(char* str, char *arr[]){
    enum states{before, cmd_sym_data, after_param, after_command,
                in_operand, after_oprtand, comma};
    int stringIndex = 0, i = 0;
    int start = 0, end ,operandsNum = 0, oprandsCounter = 0;
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

        case cmd_sym_data:/*if the word is a symbol, a data or a command*/
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
            if(str[stringIndex] == ','){
                if(operandsNum <= oprandsCounter)
                    return(printAndReturn("ERROR : ILLEGAL COMMA", -1));
                state = comma;
                break;
            }
            if(!strchr(ws,str[stringIndex])){
                if(operandsNum > oprandsCounter){
                    return(printAndReturn("ERROR : MISSING COMMA", -1));
                }
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

/*create new code node*/
code* addDataNode(code *tail){
    if(tail == NULL){
        tail = (code *)malloc(sizeof(code));
    }else{
        tail->next = (code *)malloc(sizeof(code));
        tail = tail->next;
    }
    if (tail == NULL){
        printf("ERROR : MEMORY ALLOCATION FAILED");
        return;
    }
    return tail;
}

/*print str and return num*/
int printAndReturn(char *str, int num){
    printf("%s\n", str);
    return num;
}

/*this function checks if commandName is a command. returns the number 
of operands of the command or -1 if commandName is not command*/
int isCommand(char commandName[MAX_LINE_LEN]){
    command *cmd = (command *)malloc(sizeof(command));
    int index, cmp;
    for (index = 0; index < MAX_CMD_NUM; index++){
        cmd = &cmd_arr[index];
        if ((cmp = strcmp(commandName, cmd->cmdName)) == 0){/*if commandName is command*/
            if(index < 5) return 2;
            if (index < 14) return 1;
            return 0;
        }
        cmd = (command *)malloc(sizeof(command));
    }
    free(cmd);
    return -1;
}
