#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

        switch (opcode) {
            case OP_LD: {  
                if (rd < REGISTER_COUNT)
                    registers[rd] = imm;
                break;
            }
            case OP_ADD: {  
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT && rs2 < REGISTER_COUNT)
                    registers[rd] = registers[rs1] + registers[rs2];
                break;
            }
            case OP_PRT: {  
                if (rd < REGISTER_COUNT)
                    printf("R%d = %d\n", rd, registers[rd]);
                break;
            }
            case OP_XOR: {  
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT && rs2 < REGISTER_COUNT)
                    registers[rd] = registers[rs1] ^ registers[rs2];  
                break;
            }
            case OP_AND: {  
                if (rd < REGISTER_COUNT && rs1 < REGISTER_COUNT && rs2 < REGISTER_COUNT)
                    registers[rd] = registers[rs1] & registers[rs2];  
                break;
            }
            default: {
                if(opcode != 0){
                    printf("Unknown opcode %d\n", opcode);                    
                }
                break;
            }
        }
        // printf("Opcode: %d, ", opcode);
        // printf("RD: %d, ", rd);
        // if (opcode == OP_LD) {
        //     printf("Imm: %d\n", imm);
        // } else if (opcode == OP_ADD || opcode == OP_XOR || opcode == OP_AND) {
        //     printf("RS1: %d, RS2: %d\n", rs1, rs2);
        // } else if (opcode == OP_PRT) {
        //     printf("\n");
        // } else {
        //     printf("RS1: %d, RS2: %d, Imm: %d (raw)\n", rs1, rs2, imm);
        // }
        // printf("%i\n", registers[rs1] ^ registers[rs2]);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s program.bin\n", argv[0]);
        return 1;
    }

    
    for (int i = 0; i < REGISTER_COUNT; i++) {
        registers[i] = 0;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("Failed to open binary file");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);

    int instruction_count = file_size / sizeof(uint32_t);
    uint32_t* program = malloc(file_size);
    if (!program) {
        perror("Memory allocation failed");
        fclose(f);
        return 1;
    }

    fread(program, sizeof(uint32_t), instruction_count, f);
    fclose(f);

    execute_program(program, instruction_count);
    free(program);
    return 0;
}
