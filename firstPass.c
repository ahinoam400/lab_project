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
int addressingMode(char *operand);
int isLegalNumber(char *number);
int isRegister(char *str);
extern command cmd_arr[];
extern symbol *head = NULL, *tail = NULL, *temp = NULL;

int main(){
    /*printf("%d, %d, %d, %d", isRegister("r4"), isRegister("r16"), isRegister("ra1"), isRegister("b4"));*/
    addSymbol("TEST", 100, ".data");
    printf("%s, %d, %d, %d, %s\n", head->symbol, head->value, head->baseAddress, head->offset, head->attributes);
    addSymbol("EXT", 105, ".extern");
    printf("%s, %d, %d, %d, %s\n", tail->symbol, tail->value, tail->baseAddress, tail->offset, tail->attributes);
}

int firstPass(char *filename){
    FILE assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if(assembly == NULL ){
        printf("Error opening file\n");
        return -1;
    }
    int IC = 100, DC = 0;
    int errFlag = 0, symbolFlag = 0;
    char line[MAX_LINE_LEN];
    char *token, *temp;
    while(fgets(line, MAX_LINE_LEN, assembly)){
        token = strtok(line, " ");
        if(!strcmp(token[strlen(token)-1], ":")){
            symbolFlag = 1;
            strcpy(temp, token);
            token = strtok(NULL, " ");
            if(!strcmp(token, ".string")||!strcmp(token, ".data")){
                addSymbol(temp, IC, token);
            }
        }
    }
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

/*finds the addressing mode of the operand*/
int addressingMode(char *operand){
    enum states{ladder, label};
    enum addressingModes{immediate, direct, index, register_direct};
    int state, adrressing_mode ,i,num;
    char *copy;
    if(operand[0] == '#')
        state = ladder;
    if(isRegister(operand))
        adrressing_mode = register_direct;
    for(i=0; operand[i]!='\0';i++){
        if(state = ladder){
            copy = operand+1;
            if(num = isLegalNumber(copy)){
                addressingMode = immediate;
            }
            else{
                return 0;
            }
        }
    }
    
}

int isCommand(char commandName[MAX_LINE_LEN]){
    code *head = (code*)malloc(sizeof(code));
    code *tail = head;
    command *cmd = (command*)malloc(sizeof(command));
    int index, cmp;
    for(index = 0; index<MAX_CMD_NUM; index++){
        cmd = &cmd_arr[index];
        if((cmp = strcmp(commandName, cmd->cmdName)) == 0){
            tail->next = (code*)malloc(sizeof(code));
            tail = tail->next;
            tail->code_line.command.opcode = decimalToBinary(cmd_arr[index].cmd_opcode);
            tail->code_line.command.coding_class = ABSOLUTE; 
            if(index <= 14){
                tail->code_line.word.funct = decimalToBinary(cmd_arr[index].cmd_funct);
                tail->code_line.word.coding_class = ABSOLUTE;
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
