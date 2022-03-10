#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constant.h"
#include "structs.h"
int firstPass(char *filename);
long int DecimalToBinary(int n);
int isCommand(char commandName[MAX_LINE_LEN]);
void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN]);
int isLegalSymName(char symbolName[MAX_LINE_LEN]);
int isNameInTable(char symbolName[MAX_LINE_LEN]);
int isLegalNumber(char *number);
int isRegister(char *str);
int findAddressingMode(char *operand, int src_or_dest);
extern command cmd_arr[];
symbol *head = NULL, *tail = NULL;
code *head_code = NULL, *tail_code = NULL;

int main(){
    head_code = (code*)malloc(sizeof(code));
    tail_code = head_code;
    findAddressingMode("r2", 0);
    /*addSymbol("TEST", 100, ".data");
    printf("%s, %d, %d, %d, %s\n", head->symbol, head->value, head->baseAddress, head->offset, head->attributes);
    addSymbol("EXT", 105, ".string");
    printf("%s, %d, %d, %d, %s\n", tail->symbol, tail->value, tail->baseAddress, tail->offset, tail->attributes);
    addSymbol("SYM", 127, ".extern");
    printf("%s, %d, %d, %d, %s\n", tail->symbol, tail->value, tail->baseAddress, tail->offset, tail->attributes);*/
}
/*convert decimal number to binary number*/
long int decimalToBinary(int decNum){
    int binaryNum[16];
    int i = 0 , counter;
    long int reverse = 0;
    for(counter=0; counter<16; counter++)
        binaryNum[counter] = 0;
  
    while(decNum > 0){
        binaryNum[i] = decNum%2;
        decNum = decNum/2;
        i++;
    }
    for (i=i-1; i>=0; i--)
        reverse = 10 * reverse + binaryNum[i];
    return reverse;
}

int firstPass(char *filename){
    char *fileNameCopy;
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if(assembly == NULL ){
        printf("Error opening file\n");
        return -1;
    }
    int IC = 100, DC = 0;
    int errFlag = 0, symbolFlag = 0;
    int num, i;
    char line[MAX_LINE_LEN];
    char *token, *name;
    while(fgets(line, MAX_LINE_LEN, assembly)){
        token = strtok(line, " ");
        if(token[strlen(token)-1] == ':'){
            strcpy(name, token);
            name = strtok(name, ":");
            if(!isLegalSymName(name)){
                printf("ERROR: ILLEGAL SYMBOL NAME");
                errFlag = 1;
                continue;
            }
            if(isNameInTable(name)){
                printf("ERROR: SYMBOL NAME ALREADY EXISTS");
                errFlag = 1;
                continue;
            }
            symbolFlag = 1;
            token = strtok(NULL, " ");
        }

        if(!strcmp(token, ".string")){
            if(symbolFlag)
                addSymbol(name, IC, token);
            token = strtok(NULL, " ");
            for(i=0; token[i]!='\0'; i++){  
                tail_code->next = (code*)malloc(sizeof(code));
                tail_code->code_line.string_word.str = token[i];
                tail_code->code_line.string_word.class.absolute = 1;
                DC++;
            }
            continue;
        }
        if(!strcmp(token, ".data")){
            if(symbolFlag)
                addSymbol(name, IC, token);
            token = strtok(NULL, " ");
            if(!(num = isLegalNumber(token))){
                printf("ERROR : ILLEGAL DATA");
                return 0;
            }
            tail_code->next = (code*)malloc(sizeof(code));
            tail_code->code_line.data_word.data_num = decimalToBinary(num);
            tail_code->code_line.data_word.class.absolute = 1;
            DC++;
            continue;
        }
        if(!strcmp(token, ".entry")) continue;
        if(!strcmp(token, ".extern")){
            if(!symbolFlag){
                printf("ERROR : NO SYMBOL");
                errFlag = 1;
                continue;
            }
            if()
            addSymbol(name, 0, token);
            continue;
        }
        if(symbolFlag)
            addSymbol(name, IC, ".code");
        if(isCommand(token)==-1){
            printf("ERROR: COMMAND NAME");
            errFlag = 1;
            continue;
        }

        if(!strcmp(token, ".string")){
            token = strtok(NULL, " ");
            for(i=0; token[i]!='\0'; i++){  
                tail_code->next = (code*)malloc(sizeof(code));
                tail_code->code_line.string_word.str = token[i];
                tail_code->code_line.string_word.class.absolute = 1;
                DC++;
            }
            continue;
        }
        if(!strcmp(token, ".data")){
            addSymbol(name, IC, token);
            token = strtok(NULL, " ");
            if(!(num = isLegalNumber(token))){
                printf("ERROR : ILLEGAL DATA");
                return 0;
            }
            tail_code->next = (code*)malloc(sizeof(code));
            tail_code->code_line.data_word.data_num = decimalToBinary(num);
            tail_code->code_line.data_word.class.absolute = 1;
            DC++;
            continue;
        }
    }

}

/*finds the addressing mode of the operand*/
int findAddressingMode(char *operand, int src_or_dest){
    enum addressingModes{immediate = 0, direct, index, register_direct};
    int state, addressing_mode=-1 ,i,num, len, j;
    char *copy;
    char symCopy[strlen(operand)], regCopy[strlen(operand)];
    symbol *node = head;
    
    if(operand[0] == '#'){
        copy = operand+1;
        if(num = isLegalNumber(copy)){
            addressing_mode = immediate;
        }
    }
    else if(isRegister(operand)){
        num = isRegister(operand);
        addressing_mode = register_direct;
    }
    else if(isLegalSymName(operand)){
        addressing_mode = direct;
    }
    else{
        strcpy(symCopy, operand);
        for(i=0; symCopy[i]!='\0' && symCopy[i]!='['; i++);
        symCopy[i-1] = '\0';
        strcpy(regCopy,operand+i+1);
        if(regCopy[strlen(regCopy)-1]!=']' || !isLegalSymName(symCopy)){
            printf("ERROR : ILLEGAL OPERAND");
            return 0;
        }
        len = strlen(regCopy);
        regCopy[len-1] = '\0';
        if(!(num = isRegister(regCopy))){
            printf("ERROR : ILLEGAL OPERAND");
            return 0;            
        }
        if(num < 10){
            printf("ERROR : ILLEGAL REGISTER NUMBER");
            return 0;
        }
        addressing_mode = index;
    }
    if(addressing_mode < 0)return -1;
    if(src_or_dest == 0){/*if the operand is a source operand*/
        tail_code->code_line.word.src_address = decimalToBinary(addressing_mode);
    }
    if(src_or_dest == 1){/*if the operand is a destination operand*/
        tail_code->code_line.word.dest_address = decimalToBinary(addressing_mode);
    }
    switch (addressing_mode){
    case immediate:
        tail_code->code_line.imm_word.class.absolute = 1;
        tail_code->code_line.imm_word.word = decimalToBinary(num);
        break;
    case direct:
        while(node != NULL){
            if(!strcmp(node->symbol, operand)){
                if(!strcmp(node->attributes,"external")){
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
        while(node != NULL){
            if(!strcmp(node->symbol, symCopy)){
                if(!strcmp(node->attributes,"external")){
                    tail_code->code_line.inx_words.class.external = 1;
                    tail_code->code_line.inx_words.class_2.external = 1;
                    break;    
                }
                tail_code->code_line.inx_words.base_address = node->baseAddress;
                tail_code->code_line.inx_words.offset = node->offset;
                tail_code->code_line.inx_words.class.relocatable = 1;
                tail_code->code_line.inx_words.class_2.relocatable = 1;
                if(src_or_dest == 0){
                    tail_code->code_line.word.src_register = decimalToBinary(num);
                }
                if(src_or_dest == 1){
                    tail_code->code_line.word.dest_register = decimalToBinary(num);
                }
            }
            node = node->next;
        }
    case register_direct:
        if(src_or_dest == 0){
            tail_code->code_line.word.src_register = decimalToBinary(num);
        }
        if(src_or_dest == 1){
            tail_code->code_line.word.dest_register = decimalToBinary(num);
        }

    default:
        break;
    }
}

int isCommand(char commandName[MAX_LINE_LEN]){
    command *cmd = (command*)malloc(sizeof(command));
    int index, cmp;
    for(index = 0; index<MAX_CMD_NUM; index++){
        cmd = &cmd_arr[index];
        if((cmp = strcmp(commandName, cmd->cmdName)) == 0){
            tail_code->next = (code*)malloc(sizeof(code));
            tail_code = tail_code->next;
            tail_code->code_line.command.opcode = decimalToBinary(cmd_arr[index].cmd_opcode);
            tail_code->code_line.command.class.absolute = 1; 
            if(index <= 14){
                tail_code->code_line.word.funct = decimalToBinary(cmd_arr[index].cmd_funct);
                tail_code->code_line.word.class.absolute = 1;
            }
            if(index<6) return 2;
            if(index<15)return 1;
            return 0;
        }
        cmd = (command*)malloc(sizeof(command));
    }
    return -1;
}

void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN]){
    if(head == NULL){
        head = (symbol*)malloc(sizeof(symbol));
        strcpy(head->symbol, symbolName);
        if(!strcmp(attribute, ".extern")){
            head->value = 0;
            head->baseAddress = 0;
            head->offset = 0;
            strcat(head->attributes, "external");
            tail = (symbol*)malloc(sizeof(symbol));
            head->next = tail;
            return ;
        }
        head->value = IC;
        head->baseAddress = (IC/32)*32;
        head->offset = IC - head->baseAddress;
        if(!strcmp(attribute, ".data")||!strcmp(attribute, ".string")) strcat(head->attributes, "data");
        else strcat(head->attributes, "code");
        tail = (symbol*)malloc(sizeof(symbol));
        head->next = tail;
    }
    else{
        tail = (symbol*)malloc(sizeof(symbol));
        strcpy(tail->symbol, symbolName);
        if(!strcmp(attribute, ".extern")){
            tail->value = 0;
            tail->baseAddress = 0;
            tail->offset = 0;
            strcat(tail->attributes, "external");
            symbol *temp = (symbol*)malloc(sizeof(symbol));
            tail->next = temp;
            return ;
        }
        tail->value = IC;
        tail->baseAddress = (IC/32)*32;
        tail->offset = IC - tail->baseAddress;
        if(!strcmp(attribute, ".data")||!strcmp(attribute, ".string")) strcat(tail->attributes, "data");
        else strcat(tail->attributes, "code");
        symbol *temp = (symbol*)malloc(sizeof(symbol));
        tail->next = temp;
    }
}
int isLegalSymName(char symbolName[MAX_LINE_LEN]){
    int i;
    if(isCommand(symbolName)!=-1||isRegister(symbolName)!=0) return 0;
    for(i=0; symbolName[i]!='\0'; i++){
        if(!isalpha(symbolName[i]) && !isdigit(symbolName[i]))return 0;
    }
    return 1;
}

int isNameInTable(char symbolName[MAX_LINE_LEN]){
    symbol *sym = head;
    while(sym != NULL){
        if(!strcmp(sym->symbol, symbolName)) return 1;
        sym = sym->next;
    }
    return 0;
}

/*checks if num is a legal number */
int isLegalNumber(char *number){
    int num, i=0;
    if(number[i] == '-' || number[i] == '+')i++;
    while(number[i]!='\0'){
        if(!isdigit(number[i]))return 0;
        i++;
    }
    return atoi(number);
}

int isRegister(char *str){
    int i=0, number;
    char *str2;
    if(str[i]!='r')return 0;
    for(i=1; str[i]!='\0'; i++){
        if(!isdigit(str[i]))return 0;
    }
    str2 = str+1;
    number = atoi(str2);
    if(number > 15)return 0;
    return number;
}
