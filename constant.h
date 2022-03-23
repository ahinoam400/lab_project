
#define MAX_LINE_LEN 80
#define MAX_CMD_NUM 16

typedef struct symbol{ /*struct for symbols table*/
    char symbol[MAX_LINE_LEN];
    int value;
    int baseAddress;
    int offset;
    char attributes[MAX_LINE_LEN];
    struct symbol* next;
}symbol;

typedef struct{/*struct for the commands*/
    char cmdName[5];
    int cmd_opcode;
    int cmd_funct;
}command;

extern command cmd_arr[MAX_CMD_NUM];