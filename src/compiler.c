#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "global.c"

#define MAX_LINE_LEN 100
#define MAX_VARIABLES 128

int* variablePointers;
char** variableNames;
char* variableTypes;
int variableCounter = 0;

int add_variable(const char* variable_name, char type) {
    if (variableCounter >= MAX_VARIABLES) {
        fprintf(stderr, "Error: Maximum number of variables exceeded\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < variableCounter; i++) {
        if (strcmp(variableNames[i], variable_name) == 0) {
            return variablePointers[i];
        }
    }

    variablePointers[variableCounter] = variableCounter;
    variableNames[variableCounter] = strdup(variable_name);
    variableTypes[variableCounter] = type;
    return variableCounter++;
}

int get_register_index(const char* var_name) {
    for (int i = 0; i < variableCounter; i++) {
        if (strcmp(variableNames[i], var_name) == 0) {
            return variablePointers[i];
        }
    }

    fprintf(stderr, "Warning: Variable '%s' is used without declaration. It will be declared automatically\n", var_name);
    variablePointers[variableCounter] = variableCounter;
    variableNames[variableCounter] = strdup(var_name);
    variableTypes[variableCounter] = 'i';
    return variablePointers[variableCounter++];
}

uint32_t encode_instruction(const char* instr_line) {
    char reg1[16], reg2[16], reg3[16];
    int immediate;
    uint32_t instruction = 0;

    if (sscanf(instr_line, "%s = %d", reg1, &immediate) == 2) {
        instruction |= ((uint32_t)OP_LD) << 24;
        instruction |= ((uint32_t)add_variable(reg1, 'i')) << 20;
        instruction |= (immediate & 0xFFFF);
    }
    else if (sscanf(instr_line, "%s + %s", reg1, reg2) == 2) {
        instruction |= ((uint32_t)OP_ADD) << 24;
        instruction |= ((uint32_t)get_register_index(reg1)) << 20;
        instruction |= ((uint32_t)get_register_index(reg2)) << 16;
    }
    else if (sscanf(instr_line, "%s ^ %s", reg1, reg2) == 2) {
        instruction |= ((uint32_t)OP_XOR) << 24;
        instruction |= ((uint32_t)get_register_index(reg1)) << 20;
        instruction |= ((uint32_t)get_register_index(reg2)) << 16;
    }
    else if (sscanf(instr_line, "%s & %s", reg1, reg2) == 2) {
        instruction |= ((uint32_t)OP_AND) << 24;
        instruction |= ((uint32_t)get_register_index(reg1)) << 20;
        instruction |= ((uint32_t)get_register_index(reg2)) << 16;
    }
    else if (sscanf(instr_line, "%s / %s", reg1, reg2) == 2) {
        instruction |= ((uint32_t)OP_DIV) << 24;
        instruction |= ((uint32_t)get_register_index(reg1)) << 20;
        instruction |= ((uint32_t)get_register_index(reg2)) << 16;
    }
    else if (sscanf(instr_line, "%s * %s", reg1, reg2) == 2) {
        instruction |= ((uint32_t)OP_MUL) << 24;
        instruction |= ((uint32_t)get_register_index(reg1)) << 20;
        instruction |= ((uint32_t)get_register_index(reg2)) << 16;
    }
    else if (sscanf(instr_line, "%s \\ %s", reg1, reg2) == 2) {
        instruction |= ((uint32_t)OP_MOD) << 24;
        instruction |= ((uint32_t)get_register_index(reg1)) << 20;
        instruction |= ((uint32_t)get_register_index(reg2)) << 16;
    }
    else if (sscanf(instr_line, "%s - %s", reg1, reg2) == 2) {
        instruction |= ((uint32_t)OP_SUB) << 24;
        instruction |= ((uint32_t)get_register_index(reg1)) << 20;
        instruction |= ((uint32_t)get_register_index(reg2)) << 16;
    }
    else if (sscanf(instr_line, "print %s", reg1) == 1) {
        instruction |= ((uint32_t)OP_PRT) << 24;
        instruction |= ((uint32_t)get_register_index(reg1)) << 20;
    }
    else if (sscanf(instr_line, "input %s", reg1) == 1) {
        instruction |= ((uint32_t)OP_CIN) << 24;
        instruction |= ((uint32_t)add_variable(reg1, 'i')) << 20;
    }
    else if (sscanf(instr_line, "char %s", reg1) == 1) {
        instruction |= ((uint32_t)OP_CHR) << 24;
        instruction |= ((uint32_t)add_variable(reg1, 'c')) << 20;
    }

    return instruction;
}

int main(int argc, char** argv) {
    variablePointers = malloc(MAX_VARIABLES * sizeof(int));
    variableNames = malloc(MAX_VARIABLES * sizeof(char*));
    variableTypes = malloc(MAX_VARIABLES * sizeof(char));

    if (argc < 3) {
        fprintf(stderr, "Usage: %s input.asm output.bin\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "wb");
    if (!input_file || !output_file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), input_file)) {
        uint32_t instruction = encode_instruction(line);
        fwrite(&instruction, sizeof(uint32_t), 1, output_file);
    }

    for (int i = 0; i < variableCounter; i++) {
        free(variableNames[i]);
    }

    free(variablePointers);
    free(variableNames);
    free(variableTypes);

    fclose(input_file);
    fclose(output_file);
    return EXIT_SUCCESS;
}
