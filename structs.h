typedef union{
    unsigned int absolute : 1;
    unsigned int relocatable: 1;
    unsigned int external : 1;
}coding_class;

typedef struct{/*the first word*/
    coding_class class;
    unsigned int opcode : 16;
    unsigned int empty_bit : 1;
}cmd_word;

 
typedef struct{/*for comands with operands*/
    unsigned int dest_address : 2;
    unsigned int dest_register : 4;
    unsigned int src_address : 2;
    unsigned int src_register : 4;
    unsigned int funct : 4;
    coding_class class;
    unsigned int empty_bit : 1;
}second_word;

typedef struct{/*word for immediate addressing mode*/
    unsigned int word : 16;
    coding_class class;
    unsigned int empty_bit : 1;
}immediate_word;

typedef struct{/*two words for direct addressing mode*/
    /*the first word:*/
    unsigned int base_address : 16;
    coding_class class;
    unsigned int empty_bit : 1;
    /*the second word:*/
    unsigned int offset :16;
    coding_class class_2;
    unsigned int empty_bit_2 : 1;
}direct_words;

typedef struct{/*two words for index addressing mode*/
    /*the first word:*/
    unsigned int base_address : 16;
    coding_class class;
    unsigned int empty_bit : 1;
    /*the second word:*/
    unsigned int offset :16;
    coding_class class_2;
    int empty_bit_3 : 1;
}index_words;

typedef struct{
    int data_num : 16;
    coding_class class;
    unsigned int empty_bit_4 : 1;
}data;

typedef struct{
    unsigned int str : 16;
    coding_class class;
    unsigned int empty_bit_5 : 1;
}string;

typedef union{/*hold the code for a line*/
    cmd_word command;
    second_word word; /*for comands with operands*/
    immediate_word imm_word; /*for immediate addressing mode*/
    direct_words dir_words; /*for direct addressing mode*/
    index_words inx_words; /*for index addressing mode*/
    data data_word;
    string string_word;
}code_line;

struct code{/*hold the code for a file*/
    code_line code_line;
    struct code *next;
};
typedef struct code code;
