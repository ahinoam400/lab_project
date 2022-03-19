
#include "firstPassFunctions.h"
/*#include "functions.h"*/
int firstPass(char *filename);
extern command cmd_arr[];
int ICF, DCF;
int main()
{
    head_code = (code *)malloc(sizeof(code));
    tail_code = head_code;
    firstPass("test");
    /*addSymbol("TEST", 100, ".data");
    printf("%s, %d, %d, %d, %s\n", head->symbol, head->value, head->baseAddress, head->offset, head->attributes);
    addSymbol("EXT", 105, ".string");
    printf("%s, %d, %d, %d, %s\n", tail->symbol, tail->value, tail->baseAddress, tail->offset, tail->attributes);
    addSymbol("SYM", 127, ".extern");
    printf("%s, %d, %d, %d, %s\n", tail->symbol, tail->value, tail->baseAddress, tail->offset, tail->attributes);*/
}

int firstPass(char *filename)
{
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
    {
        printf("ERORR OPENING FILE\n");
        return -1;
    }
    int IC = 100, DC = 0, L = 0, lineLength = 0;
    int errFlag = 0, symbolFlag = 0;
    int num, i, command;
    char line[MAX_LINE_LEN];
    char *token, name2[MAX_LINE_LEN], *name, firstChar = ' ';
    bool isEmptyLine = true;
    while (fgets(line, MAX_LINE_LEN, assembly))
    {
        char *arr[MAX_LINE_LEN];
        char test[MAX_LINE_LEN] = line;
        lineLength = strlen(line);
        line[lineLength] = '\0';
        for (i = 0; i < lineLength; i++)
        {
            if (!isspace(line[i]))
            {
                isEmptyLine = false;
                firstChar = line[i];
                break;
            }
        }
        if (firstChar != ';' && isEmptyLine == false)
        {
            token = strtok(line, "  ");
            if (token[strlen(token) - 1] == ':')
            {
                strcpy(name2, token);
                name = name2;
                name = strtok(name, ":");
                if (!isLegalSymName(name))
                {
                    printf("ERROR: ILLEGAL SYMBOL NAME");
                    errFlag = 1;
                    continue;
                }
                if (isNameInTable(name))
                {
                    printf("ERROR: SYMBOL NAME ALREADY EXISTS");
                    errFlag = 1;
                    continue;
                }
                symbolFlag = 1;
            }
            if (!strcmp(token, ".string"))
            {
                if (symbolFlag)
                    addSymbol(name, IC, token);
                token = strtok(NULL, " ");
                for (i = 0; token[i] != '\0'; i++)
                {
                    if (!addDataNode())
                    {
                        errFlag = 1;
                        continue;
                    }
                    tail_code->code_line.string_word.str = token[i]; /*not working well*/
                    tail_code->code_line.string_word.class.absolute = 1;
                    tail_code->code_line.string_word.class.relocatable = 0;
                    tail_code->code_line.string_word.class.external = 0;

                    DC++;
                }
            }
            else if (!strcmp(token, ".data"))
            {
                if (symbolFlag)
                    addSymbol(name, IC, token);
                token = strtok(NULL, " ");
                if (!(num = isLegalNumber(token)))
                {
                    printf("ERROR : ILLEGAL DATA");
                    return 0;
                }
                if (!addDataNode())
                {
                    errFlag = 1;
                    continue;
                }
                tail_code->code_line.data_word.data_num = decimalToBinary(num); /*not working well*/
                tail_code->code_line.data_word.class.absolute = 1;
                tail_code->code_line.string_word.class.relocatable = 0;
                tail_code->code_line.string_word.class.external = 0;
                DC++;
            }
            else if (!strcmp(token, ".entry"))
                continue;
            else if (!strcmp(token, ".extern"))
            {
                if (!symbolFlag)
                {
                    printf("ERROR : THERE IS NO SYMBOL");
                    errFlag = 1;
                    continue;
                }
                addSymbol(name, 0, token);
                continue;
            }
            else
            {
                if (symbolFlag)
                {
                    addSymbol(name, IC, ".code");
                    token = strtok(NULL, " ");
                }
                if (command = isCommand(token) == -1)
                {
                    printf("ERROR: COMMAND NAME");
                    errFlag = 1;
                    continue;
                }
            }

            /*enter steps 13-15 here, i couldn't understand them and i got stuck because of them... 0_0*/
            IC += lineLength;
            L += lineLength;
            lineLength = 0;
        }
    }
    ICF = IC;
    DCF = DC;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    sym = head;
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

int secondPass(char *filename){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
    {
        printf("ERORR OPENING FILE\n");
        return -1;
    }
    char line[MAX_LINE_LEN];
    char *token;
    int errFlag = 0;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    while(fgets(line, MAX_LINE_LEN, assembly)){
        token = strtok(line, " ");
        if(!isLegalSymName(token))
            continue;
        if(!(strcmp(token, ".data"))||!(strcmp(token, ".string"))||!(strcmp(token, ".extern")))
            continue;
        if(!strcmp(token, ".entry")){
            token = strtok(NULL, " ");
            if(!isNameInTable(token)){
                errFlag = 1;
                printf("ERROR: NAME IS NOT IN TABLE");
                continue;
            }
            sym = head;
            while(sym != NULL){
                if(!strcmp(sym->symbol, token)){
                    strcat(sym->attributes, ", entry");
                    break;
                }
                sym = sym->next;
            }
        }
    }
}
