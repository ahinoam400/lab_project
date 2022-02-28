#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constant.h"
#include "structs.h"
long int DecimalToBinary(int n);
int isCommand(char commandName[MAX_LINE_LEN]);
void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN]);
extern command cmd_arr[];
extern symbol *head = NULL, *tail = NULL;

int main(){
    addSymbol("TEST", 100, ".data");
    printf("%s, %d, %d, %d, %s\n", head->symbol, head->value, head->baseAddress, head->offset, head->attributes);
    addSymbol("EXT", 105, ".string");
    printf("%s, %d, %d, %d, %s\n", tail->symbol, tail->value, tail->baseAddress, tail->offset, tail->attributes);
}

/*this function get a line, turns on the flag if there is a label,
 calculates how many words are needed in memory (according to the 
 command and according to the operands) and puts the line in memory
int lineProcess(char* line){
    
}
*/
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
            return (index<5?1:0);
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
        strcpy(tail->symbol, symbolName);
        if(!strcmp(attribute, ".extern")){
            tail->value = 0;
            tail->baseAddress = 0;
            tail->offset = 0;
            strcat(tail->attributes, "external");
            symbol *temp = (symbol*)malloc(sizeof(symbol));
            temp = tail;
            tail = temp->next;
            return ;
        }
        tail->value = IC;
        tail->baseAddress = (IC/32)*32;
        tail->offset = IC - tail->baseAddress;
        if(!strcmp(attribute, ".data")||!strcmp(attribute, ".string")) strcat(tail->attributes, "data");
        else strcat(tail->attributes, "code");
        symbol *temp = (symbol*)malloc(sizeof(symbol));
        temp = tail;
        tail = temp->next;
    }
}
