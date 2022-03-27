#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "constant.h"
#include "structs.h"
int isNameInTable(char symbolName[MAX_LINE_LEN], symbol *head);
int split(char* str, char *arr[], int lineNum);
int printAndReturn(char *str, int num, int lineNum);
int isCommand(char commandName[MAX_LINE_LEN]);
int isLegalNumber(char *number);
int isLegalSymName(char symbolName[MAX_LINE_LEN]);
int isRegister(char *str);
void print_code(code* p_code);
void print_symbol(symbol* p_symbol);
void print_data(data* p_data, int icf);
void fprint_data(data *p_data, int icf, FILE *stream);
void fprint_code(code* p_code, FILE *stream);
command* getCommandByName(char *cmdName);
symbol* getSymbolByName(symbol *sym_head, char *symName);