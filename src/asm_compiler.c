#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "global.c"

#define MAX_LINE_LEN 100

int get_register_index(const char* reg) {
    if (reg[0] != 'r') {
        fprintf(stderr, "Invalid register name: %s\n", reg);
        exit(EXIT_FAILURE);
    }

    int reg_num = atoi(reg + 1);  // Skip the 'r' character
    if (reg_num < 0 || reg_num >= 128) {
        fprintf(stderr, "Register index out of bounds: %s\n", reg);
        exit(EXIT_FAILURE);
    }

    return reg_num;
}


int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s input.asm output.bin\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "wb");
    if (!fin || !fout) {
        perror("File error");
        return 1;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fin)) {
        char instr[10], r1[4], r2[4], r3[4];
        int val;
        uint32_t code = 0;

        // Parsing instructions and encoding them
        if (sscanf(line, "ld %[^,], %d", r1, &val) == 2) {
            code = (OP_LD << 24) | (reg_index(r1) << 20) | (val & 0xFFFF);
        }
        
        else if (sscanf(line, "add %[^,], %s", r1, r2) == 2) {
            code = (OP_ADD << 24) |
                   (reg_index(r1) << 20) |
                   (reg_index(r1) << 16) |
                   (reg_index(r2) << 12);
        }
        
        else if (sscanf(line, "prt %s", r1) == 1) {
            code = (OP_PRT << 24) | (reg_index(r1) << 20);
        } 
        
        else if (sscanf(line, "xor %[^,], %s", r1, r2) == 2) {
            code = (OP_XOR << 24) |
                   (reg_index(r1) << 20) |
                   (reg_index(r1) << 16) | 
                   (reg_index(r2) << 12);  
        }
        else if (sscanf(line, "and %[^,], %[^,]", r1, r2) == 2) {
            code = (OP_AND << 24) | (reg_index(r1) << 20) | (reg_index(r2) << 16);
        }
        else if (sscanf(line, "nor %[^,], %[^,]", r1, r2) == 2) {
            code = (OP_NOR << 24) | (reg_index(r1) << 20) | (reg_index(r2) << 16);
        }
        else if (sscanf(line, "div %[^,], %[^,]", r1, r2) == 2) {
            code = (OP_DIV << 24) | (reg_index(r1) << 20) | (reg_index(r2) << 16);
        }
        else if (sscanf(line, "mul %[^,], %[^,]", r1, r2) == 2) {
            code = (OP_MUL << 24) | (reg_index(r1) << 20) | (reg_index(r2) << 16);
        }
        else if (sscanf(line, "mod %[^,], %[^,]", r1, r2) == 2) {
            code = (OP_MOD << 24) | (reg_index(r1) << 20) | (reg_index(r2) << 16);
        }
        else if (sscanf(line, "sub %[^,], %[^,]", r1, r2) == 2) {
            code = (OP_SUB << 24) | (reg_index(r1) << 20) | (reg_index(r2) << 16);
        }
        else if (sscanf(line, "cin %s", r1) == 1) {
            code = (OP_CIN << 24) | (reg_index(r1) << 20);
        }            
        else if (sscanf(line, "chr %s", r1) == 1) {
            code = (OP_CHR << 24) | (reg_index(r1) << 20);
        }
        
        printf("Encoding instruction: %s -> 0x%08X\n", line, code);
        fwrite(&code, sizeof(uint32_t), 1, fout);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
