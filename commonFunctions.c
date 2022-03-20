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
int split(char* str, char *arr[], code *head, code *tail){
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
                operandsNum = isCommand(arr[i], tail);
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
    if(tail == NULL)
        tail = (code *)malloc(sizeof(code));
    tail->next = (code *)malloc(sizeof(code));
    if (tail->next == NULL){
        printf("ERROR : MEMORY ALLOCATION FAILED");
        return;
    }
    tail = tail->next;
    return tail;
}

/*print str and return num*/
int printAndReturn(char *str, int num){
    printf("%s\n", str);
    return num;
}

/*this function checks if commandName is a command and writes the 
  code of the found command. the function returns the number of 
  operands of the command and -1 if commandName is not command*/
int isCommand(char commandName[MAX_LINE_LEN], code *current_tail){
    command *cmd = (command *)malloc(sizeof(command));
    code *new_tail;
    int index, cmp;
    for (index = 0; index < MAX_CMD_NUM; index++){
        cmd = &cmd_arr[index];
        if ((cmp = strcmp(commandName, cmd->cmdName)) == 0){/*if commandName is command*/
            new_tail = addDataNode(current_tail);/*add the first word of code*/
            new_tail->code_line.command.opcode = cmd_arr[index].cmd_opcode;
            new_tail->code_line.command.class.absolute = 1;
            if (index < 14){/*if the command have oprands - add the second word of code*/
                new_tail->code_line.word.funct = cmd_arr[index].cmd_funct;
                new_tail->code_line.word.class.absolute = 1;
            }
            if(index < 6){
                return 2;
            }
            if (index < 15){
                return 1;
            }
            return 0;
        }
        cmd = (command *)malloc(sizeof(command));
    }
    free(cmd);
    return -1;
}
