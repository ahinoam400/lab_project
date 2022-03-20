#include "firstPassFunctions.h"
int firstPass(char *filename);
extern command cmd_arr[];
int ICF, DCF;
int IC = 100, DC = 0, L = 0;
symbol *symbol_head = NULL, *symbol_tail = NULL;
code *code_head = NULL, *code_tail = NULL;
int main()
{
    code_head = (code *)malloc(sizeof(code));
    code_tail = code_head;
    firstPass("test");
}

int firstPass(char *filename)
{
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
        return(printAndReturn("ERROR OPENING FILE\n", -1));
    int lineLength = 0;
    int errFlag = 0, symbolFlag = 0;
    int num, i, command, j = 0, addresing_mode;
    char line[MAX_LINE_LEN], *name, *ws = " \t";
    char firstChar = ' ';
    bool isEmptyLine = true;
    while (fgets(line, MAX_LINE_LEN, assembly))
    {
        char *arr[MAX_LINE_LEN];
        L = 0;
        split(line, arr, code_head, code_tail);
        lineLength = strlen(line);
        line[lineLength] = '\0';
        for (i = 0; i < lineLength; i++)
        {
            if (!strchr(ws, line[i]))
            {
                isEmptyLine = false;
                firstChar = line[i];
                break;
            }
        }
        if (firstChar != ';' && isEmptyLine == false)
        {
            if (arr[j][strlen(arr[j]) - 1] == ':')
            {
                if (j != 0)
                {
                    printf("ERROR: ILLEGAL SYMBOL\n");
                    errFlag = 1;
                    continue;
                }
                name = arr[j];
                name[strlen(name) - 1] = '\0';
                if (!isLegalSymName(name, code_tail))
                {
                    printf("ERROR: ILLEGAL SYMBOL NAME\n");
                    errFlag = 1;
                    continue;
                }
                if (isNameInTable(name, symbol_head))
                {
                    printf("ERROR: SYMBOL NAME ALREADY EXISTS\n");
                    errFlag = 1;
                    continue;
                }
                symbolFlag = 1;
                j++;
            }
            if (!strcmp(arr[j], ".string"))
            {
                if (symbolFlag)
                    addSymbol(name, IC, arr[j], symbol_head, symbol_tail);
                j++;
                for (i = 0; arr[j][i] != '\0'; i++)
                {
                    if (!addDataNode(code_tail))
                    {
                        errFlag = 1;
                        continue;
                    }
                    code_tail->code_line.string_word.str = arr[j][i];
                    code_tail->code_line.string_word.class.absolute = 1;
                    code_tail->code_line.string_word.class.relocatable = 0;
                    code_tail->code_line.string_word.class.external = 0;
                    DC++;
                }
            }
            else if (!strcmp(arr[j], ".data")){
                if (symbolFlag)
                    addSymbol(name, IC, arr[j], symbol_head, symbol_tail);
                j++;
                if (!(num = isLegalNumber(arr[j])))
                    return (printAndReturn("ERROR : ILLEGAL DATA", 0));
                if (!addDataNode(code_tail)){
                    errFlag = 1;
                    continue;
                }
                code_tail->code_line.data_word.data_num = num;
                code_tail->code_line.data_word.class.absolute = 1;
                code_tail->code_line.string_word.class.relocatable = 0;
                code_tail->code_line.string_word.class.external = 0;
                DC++;
            }
            else if (!strcmp(arr[j], ".entry"))
                continue;
            else if (!strcmp(arr[j], ".extern")){
                if (!symbolFlag){
                    printf("ERROR: THERE IS NO SYMBOL\n");
                    errFlag = 1;
                    continue;
                }
                addSymbol(name, 0, arr[j], symbol_head, symbol_tail);
                continue;
            }else{
                if (symbolFlag){
                    addSymbol(name, IC, ".code", symbol_head, symbol_tail);
                    j++;
                }
                if (command = isCommand(arr[j]) == -1){
                    printf("ERROR: COMMAND NAME\n");
                    errFlag = 1;
                    continue;
                }
                L += command;
                if (command < 5){
                    addresing_mode = findAddressingMode(arr[j++], 0,symbol_head, code_tail);
                    if (addresing_mode == 0)
                        L++;
                    if (addresing_mode == 1 || addresing_mode == 2)
                        L += 2;
                    if (addresing_mode == -1){
                        printf("ERROR : ILLEGAL ADDRESING MODE");
                        errFlag = 1;
                        continue;
                    }
                    addresing_mode = findAddressingMode(arr[j++], 1,symbol_head, code_tail);
                    if (addresing_mode == 0)
                        L++;
                    if (addresing_mode == 1 || addresing_mode == 2)
                        L += 2;
                    if (addresing_mode == -1){
                        printf("ERROR : ILLEGAL ADDRESING MODE");
                        errFlag = 1;
                        continue;
                    }
                }
                if (command > 5 && command < 14){
                    findAddressingMode(arr[j++], 0, symbol_head, code_tail);
                    if (addresing_mode == 0)
                        L++;
                    if (addresing_mode == 1 || addresing_mode == 2)
                        L += 2;
                    if (addresing_mode == -1){
                        printf("ERROR : ILLEGAL ADDRESING MODE");
                        errFlag = 1;
                        continue;
                    }
                }
            }
            IC += L;
            lineLength = 0;
        }
    }
    ICF = IC;
    DCF = DC;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    sym = symbol_head;
    while (sym != NULL)
    {
        if (!strcmp(sym->attributes, ".data"))
        {
            sym->value = ICF;
            sym->baseAddress = (ICF / 32) * 32;
            sym->offset = ICF - sym->baseAddress;
        }
        sym = sym->next;
    }
    free(sym);
    return !errFlag;
}
