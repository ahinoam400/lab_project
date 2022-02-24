#include <stdio.h>

#include "constant.h"
#include "stracts.h"
long int DecimalToBinary(int n);


int main(){
    printf("%ld", DecimalToBinary(8));
}

/*this function get a line, turns on the flag if there is a label,
 calculates how many words are needed in memory (according to the 
 command and according to the operands) and puts the line in memory*/
int lineProcess(char* line){

}

/*convert decimal number to binary number*/
long int DecimalToBinary(int decNum){
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

