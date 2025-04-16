#define OP_LD  0x01
#define OP_PRT 0x02
#define OP_CHR 0x03
#define OP_CIN 0x04

#define OP_XOR 0x07
#define OP_AND 0x08
#define OP_NOR 0x09

#define OP_ADD 0x0A
#define OP_DIV 0x0B
#define OP_MUL 0x0C
#define OP_MOD 0x0D
#define OP_SUB 0x0E

#define ENCODE(op, rd, rs1, rs2_or_imm) (((op)  << 22) | ((rd) << 15) | ((rs1) << 8) | ((rs2_or_imm) & 0xFF))