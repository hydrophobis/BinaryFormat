#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define REGISTER_COUNT 128

#include "global.c"

int registers[REGISTER_COUNT];
char** string_table;

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
            case OP_STR: {
                int str_index = rd;
                if (str_index >= 0 && str_index < MAX_STRINGS && string_table[str_index]) {
                    printf("%s\n", string_table[str_index]);
                } else {
                    printf("(invalid string index %d)\n", str_index);
                }
                break;
            }       
            case STRING_MARKER: {
                return;
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
        //     printf("rd: %d, rs1: %d\n", rd, rs1);
        // } else if (opcode == OP_PRT) {
        //     printf("\n");
        // } else {
        //     printf("rd: %d, rs1: %d, Imm: %d (raw)\n", rd, rs1, imm);
        // }
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
    int split_index = -1;

    for (int i = 0; i < total_words; i++) {
        if (buffer[i] == STRING_MARKER) {
            split_index = i;
            break;
        }
    }

    if (split_index == -1) {
        fprintf(stderr, "STRING_MARKER not found\n");
        free(buffer);
        return 1;
    }

    // Separate instructions
    int instruction_count = split_index;
    uint32_t* program = malloc(instruction_count * sizeof(uint32_t));
    memcpy(program, buffer, instruction_count * sizeof(uint32_t));

    // Initialize string table
    string_table = malloc(sizeof(char*) * MAX_STRINGS);
    int str_count = 0;

    // Start reading strings after the marker
    uint8_t* string_data = (uint8_t*)&buffer[split_index + 1];
    uint8_t* end = (uint8_t*)buffer + file_size;

    while (string_data + sizeof(uint32_t) < end && str_count < MAX_STRINGS) {
        uint32_t len;
        memcpy(&len, string_data, sizeof(uint32_t));
        string_data += sizeof(uint32_t);

        if (len == 0 || len > MAX_STRING_LENGTH || string_data + len > end)
            break;

        string_table[str_count] = malloc(len + 1);
        memcpy(string_table[str_count], string_data, len);
        string_table[str_count][len] = '\0';
        string_data += len;
        str_count++;
    }

    // Execute
    execute_program(program, instruction_count);

    // Cleanup
    for (int i = 0; i < str_count; i++)
        free(string_table[i]);
    free(string_table);
    free(program);
    free(buffer);
    return 0;
}
