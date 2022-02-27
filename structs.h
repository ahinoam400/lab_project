

typedef struct{/*the first word*/
    int opcode : 16;
    int coding_class : 3;/*A=Absolute, R=Relocatable or E=External*/
    int empty_bit : 1;
}cmd_word;

typedef struct{/*for comands with operands*/
    int dest_address : 2;
    int dest_register : 4;
    int src_address : 2;
    int src_register : 4;
    int funct : 4;
    int coding_class : 3;/*A=Absolute, R=Relocatable or E=External*/
    int empty_bit : 1;
}second_word;

typedef struct{/*word for immediate addressing mode*/
    int word : 16;
    int coding_class : 3;/*always A=Absolute*/
    int empty_bit : 1;
}immediate_word;

typedef struct{/*two words for direct addressing mode*/
    /*the first word:*/
    unsigned int base_address : 16;
    int coding_class : 3;/*R=Relocatable or E=External*/
    int empty_bit : 1;
    /*the second word:*/
    unsigned int offset :16;
    int coding_class_2 : 3;/*R=Relocatable or E=External*/
    int empty_bit_2 : 1;
}direct_words;

typedef struct{/*two words for index addressing mode*/
    /*the first word:*/
    unsigned int base_address : 16;
    int coding_class : 3;/*R=Relocatable or E=External*/
    int empty_bit : 1;
    /*the second word:*/
    unsigned int offset :16;
    int coding_class_3 : 3;/*R=Relocatable or E=External*/
    int empty_bit_3 : 1;
}index_words;

typedef union{/*hold the code for a line*/
    cmd_word command;
    second_word word; /*for comands with operands*/
    immediate_word imm_word; /*for immediate addressing mode*/
    direct_words dir_words; /*for direct addressing mode*/
    index_words inx_word; /*for index addressing mode*/
}code_line;

typedef struct{/*hold the code for a file*/
    code_line code_line;
    struct code *next;
}code;
