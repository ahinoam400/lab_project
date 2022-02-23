#include <stdio.h>

#include "firstPass.h"
long int DecimalToBinary(int n);

int main(){
    printf("%ld", DecimalToBinary(8));
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

