#include "firstPass.h"

int testStructCodeCommand() {
    char output[10];
    char expected[10]="10004";
    code mycode;
    mycode.code_line.command.opcode = 1<<(3-1);
    mycode.code_line.command.external = 1;
    mycode.code_line.command.relocatable = 0;
    mycode.code_line.command.absolute = 0;
    mycode.code_line.command.empty_bit = 0;//1<<15;
    sprintf(output, "%x",mycode.code_line.bytes);
    if (!strcmp(output,expected)) {
        printf("%s succeded !\n",__func__);
        return 0;
    } else {
        printf("%s failed: expected %s found %s !\n",__func__,expected,output);
        return 1;
    }
}

int testStructCodeWord() {
    char output[10];
    char expected[10]="4a4f6";
    code mycode;
    mycode.code_line.word.dest_address = 2;
    mycode.code_line.word.dest_register = 13;
    mycode.code_line.word.src_address = 3;
    mycode.code_line.word.src_register = 4;
    mycode.code_line.word.funct = 10;
    mycode.code_line.word.external = 0;
    mycode.code_line.word.relocatable = 0;
    mycode.code_line.word.absolute = 1;
    sprintf(output, "%x",mycode.code_line.bytes);
    if (!strcmp(output,expected)) {
        printf("%s succeded !\n",__func__);
        return 0;
    } else {
        printf("%s failed: expected %s found %s !\n",__func__,expected,output);
        return 1;
    }
}

int testStructCodeImmediate_word() {
    char output[10];
    char expected[10]="10030";
    code mycode;
    mycode.code_line.imm_word.word = 48;
    mycode.code_line.imm_word.external = 1;
    mycode.code_line.imm_word.relocatable = 0;
    mycode.code_line.imm_word.absolute = 0;
    sprintf(output, "%x",mycode.code_line.bytes);
    if (!strcmp(output,expected)) {
        printf("%s succeded !\n",__func__);
        return 0;
    } else {
        printf("%s failed: expected %s found %s !\n",__func__,expected,output);
        return 1;
    }
}

int testStructDirectWord() {
    char output[20];
    char expected[20]="40030 20005";
    code mycode;
    mycode.code_line.dir_words.base_address = 48;
    mycode.code_line.dir_words.external = 0;
    mycode.code_line.dir_words.relocatable = 0;
    mycode.code_line.dir_words.absolute = 1;

    mycode.code_line.dir_words.offset = 5;
    mycode.code_line.dir_words.external_2 = 0;
    mycode.code_line.dir_words.relocatable_2 = 1;
    mycode.code_line.dir_words.absolute_2 = 0;
    sprintf(output, "%x %x",mycode.code_line.twoWordsbytes.lsbBytes, mycode.code_line.twoWordsbytes.msbBytes);
    if (!strcmp(output,expected)) {
        printf("%s succeded !\n",__func__);
        return 0;
    } else {
        printf("%s failed: expected %s found %s !\n",__func__,expected,output);
        return 1;
    }
}

int testStructIndexWord(){
    char output[20];
    char expected[20]="40020 10003";
    code mycode;
    mycode.code_line.inx_words.base_address = 32;
    mycode.code_line.inx_words.external = 0;
    mycode.code_line.inx_words.relocatable = 0;
    mycode.code_line.inx_words.absolute = 1;
    mycode.code_line.inx_words.offset = 3;
    mycode.code_line.inx_words.external_2 = 1;
    mycode.code_line.inx_words.relocatable_2 = 0;
    mycode.code_line.inx_words.absolute_2 = 0;
    sprintf(output, "%x %x",mycode.code_line.twoWordsbytes.lsbBytes, mycode.code_line.twoWordsbytes.msbBytes);
    if (!strcmp(output,expected)) {
        printf("%s succeded !\n",__func__);
        return 0;
    } else {
        printf("%s failed: expected %s found %s !\n",__func__,expected,output);
        return 1;
    }
}

int testStructData() {
    char output[10];
    char expected[10]="10064";
    data mydata;
    mydata.data_line.item = 100;
    mydata.data_line.external = 1;
    mydata.data_line.relocatable = 0;
    mydata.data_line.absolute = 0;
    unsigned int *bytes = (unsigned int *)&mydata.data_line;
    sprintf(output, "%x",bytes*);
    if (!strcmp(output,expected)) {
        printf("%s succeded !\n",__func__);
        return 0;
    } else {
        printf("%s failed: expected %s found %s !\n",__func__,expected,output);
        return 1;
    }
}

int main(){
    printf("---------------test code---------------\n");
    testStructCodeCommand();
    testStructCodeWord();
    testStructCodeImmediate_word();
    testStructDirectWord();
    testStructIndexWord();
    printf("---------------test data---------------\n");
    testStructData();
}