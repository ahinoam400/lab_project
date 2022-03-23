#include "commonFunctions.h"
int isLegalSymName(char symbolName[MAX_LINE_LEN]);
int isLegalNumber(char *number);
int isRegister(char *str);
int firstPass(char *filename, struct images images);

data* addDataNode(data *tail);
code* addCodeNode(code *tail);