#include <stdio.h>
#include <stdlib.h>
#include "constant.h"
#include "structs.h"
long int DecimalToBinary(int n);
int isCommand(char commandName[MAX_LINE_LEN]);
extern command cmd_arr[];

int main(){
    printf("%d\n", isCommand("stopf"));
    
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
    command *cmd = (command*)malloc(sizeof(command));
    int index, cmp;
    for(index = 0; index<MAX_CMD_NUM; index++){
        cmd = &cmd_arr[index];
        if((cmp = strcmp(commandName, cmd->cmdName)) == 0){
            return (index<5?1:0);
        }
        cmd = (command*)malloc(sizeof(command));
    }
    return -1;
}