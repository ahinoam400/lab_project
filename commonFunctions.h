#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "constant.h"
#include "structs.h"
int isNameInTable(char symbolName[MAX_LINE_LEN], symbol *head);
int split(char* str, char *arr[], code *head, code *tail);
code* addDataNode(code *tail);
int printAndReturn(char *str, int num);
int isCommand(char commandName[MAX_LINE_LEN], code *current_tail);