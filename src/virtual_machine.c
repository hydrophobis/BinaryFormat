#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define REGISTER_COUNT 128

#include "global.c"

int registers[REGISTER_COUNT];

void execute_program(uint32_t* program, int length) {
    for (int i = 0; i < length; i++) {
        uint32_t instr = program[i];

        int opcode = (instr >> 24) & 0xFF;
        int rd     = (instr >> 20) & 0x0F;   
        int rs1    = (instr >> 16) & 0x0F;   
        int rs2    = (instr >> 12) & 0x0F;   
        int imm    = instr & 0xFFFF;         

        if (imm & 0x8000)
            imm |= 0xFFFF0000;

        //printf("Processing instruction at index %d, opcode: %d\n", i, opcode);
        switch (opcode) {
            case OP_LD: {  
                if (rd < REGISTER_COUNT)
                    registers[rd] = imm;
                break;
            }
            case OP_ADD: {  
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT)
                    registers[rd] = registers[rd] + registers[rs1];
                break;
            }
            case OP_CIN: {  
                if (rd < REGISTER_COUNT)
                    registers[rd] = getchar();
                break;
            }            
            case OP_PRT: {  
                if (rd < REGISTER_COUNT)
                    printf("%d\n", registers[rd]);
                break;
            }
            case OP_XOR: {  
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT)
                    registers[rd] = registers[rd] ^ registers[rs1];  
                break;
            }
            case OP_AND: {  
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT)
                    registers[rd] = registers[rd] & registers[rs1];  
                break;
            }
            case OP_SUB: {
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT)
                    registers[rd] = registers[rd] - registers[rs1];
                break;
            }
            case OP_MUL: {
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT)
                    registers[rd] = registers[rd] * registers[rs1];
                break;
            }
            case OP_DIV: {
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT)
                    registers[rd] = registers[rd] / registers[rs1];
                break;
            }
            case OP_MOD: {
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT)
                    registers[rd] = registers[rd] % registers[rs1];
                break;
            }
            case OP_CHR: {
                if (rd < REGISTER_COUNT)
                    printf("%c\n", (char)registers[rd]);
                break;
            }  
            default: {
                if(opcode != 0){
                    printf("Unknown opcode %d\n", opcode);                    
                }
                break;
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s program.bin\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("Failed to open binary file");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);

    uint32_t* buffer = malloc(file_size);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(f);
        return 1;
    }

    fread(buffer, 1, file_size, f);
    fclose(f);

    int total_words = file_size / sizeof(uint32_t);

    uint32_t* program = malloc(total_words * sizeof(uint32_t));
    memcpy(program, buffer, total_words * sizeof(uint32_t));

    execute_program(program, total_words);

    free(program);
    free(buffer);
    return 0;
}