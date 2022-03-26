
typedef struct{
    unsigned int item : 16;
    unsigned int external : 1;
    unsigned int relocatable : 1;
    unsigned int absolute : 1;
    unsigned int empty_bit : 1;
    unsigned int buffer:12;
}data_line;

struct data{/*hold the code for a file*/
    data_line data_line;
    struct data *next;
};
typedef struct data data;

typedef struct{/*the first word*/
    unsigned int opcode : 16;
    unsigned int external : 1;
    unsigned int relocatable : 1;
    unsigned int absolute : 1;
    unsigned int empty_bit : 1;
}cmd_word;

 
typedef struct{/*for commands with operands*/
    unsigned int dest_address : 2;
    unsigned int dest_register : 4;
    unsigned int src_address : 2;
    unsigned int src_register : 4;
    unsigned int funct : 4;
    unsigned int external : 1;
    unsigned int relocatable : 1;
    unsigned int absolute : 1;
    unsigned int empty_bit : 1;
}second_word;

typedef struct{/*word for immediate addressing mode*/
    unsigned int word : 16;
    unsigned int external : 1;
    unsigned int relocatable : 1;
    unsigned int absolute : 1;
    unsigned int empty_bit : 1;
}immediate_word;

typedef struct{/*two words for direct addressing mode*/
    /*the first word:*/
    unsigned int base_address : 16;
    unsigned int external : 1;
    unsigned int relocatable : 1;
    unsigned int absolute : 1;
    unsigned int empty_bit : 1;
}direct_word_1;

typedef struct{
    /*the second word:*/
    unsigned int offset :16;
    unsigned int external_2 : 1;
    unsigned int relocatable_2 : 1;
    unsigned int absolute_2 : 1;
    unsigned int empty_bit_2 : 1;
}direct_word_2;

typedef struct{/*two words for index addressing mode*/
    /*the first word:*/
    unsigned int base_address : 16;
    unsigned int external : 1;
    unsigned int relocatable : 1;
    unsigned int absolute : 1;
    unsigned int empty_bit : 1;
}index_word_1;
typedef struct{
    /*the second word:*/
    unsigned int offset :16;
    unsigned int external_2 : 1;
    unsigned int relocatable_2 : 1;
    unsigned int absolute_2 : 1;
    int empty_bit_3 : 1;
}index_word_2;



struct twoWordsbytes{
    unsigned int lsbBytes : 20;
    unsigned int spare :12;
    unsigned int msbBytes : 20;
};

typedef union{/*hold the code for a line*/
    cmd_word command;
    second_word word; /*for comands with operands*/
    immediate_word imm_word; /*for immediate addressing mode*/
    direct_word_1 dir_word_1; /*for direct addressing mode*/
    direct_word_2 dir_word_2; /*for direct addressing mode*/
    index_word_1 inx_word_1; /*for index addressing mode*/
    index_word_2 inx_word_2; /*for index addressing mode*/
    unsigned int bytes:20;
    struct twoWordsbytes twoWordsbytes;
}code_line;

struct code{/*hold the code for a file*/
    code_line code_line;
    struct code *next;
};
typedef struct code code;


struct images {
    symbol *symbol_head, *symbol_tail;
    code *code_head, *code_tail;
    data *data_head, *data_tail;
    int ICF;
    int DCF;
};