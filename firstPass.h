#include "commonFunctions.h"
int isLegalSymName(char symbolName[MAX_LINE_LEN]);
int isLegalNumber(char *number);
int isRegister(char *str);
int firstPass(const char *filename, struct images *images);

data* addDataNode(data *tail, int lineNum);
code* addCodeNode(code *tail, int lineNum);
symbol *addSymbolNode(symbol *tail, int lineNum);
