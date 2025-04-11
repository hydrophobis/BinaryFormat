#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_LINE_LEN 100

#include "global.c"

int reg_index(char* r) {
    return r[1] - '0';
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

        
        if (sscanf(line, "ld %[^,], %d", r1, &val) == 2) {
            code = (OP_LD << 24) | (reg_index(r1) << 20) | (val & 0xFFFF);
        }
        
        else if (sscanf(line, "add %[^,], %[^,], %s", r1, r2, r3) == 3) {
            code = (OP_ADD << 24) |
                   (reg_index(r1) << 20) |
                   (reg_index(r2) << 16) |
                   (reg_index(r3) << 12);
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

        // printf("Encoding instruction: %s -> 0x%08X\n", line, code);
        fwrite(&code, sizeof(uint32_t), 1, fout);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
