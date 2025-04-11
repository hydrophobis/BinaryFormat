#define OP_LD  0x01
#define OP_PRT 0x04
#define OP_CIN 0x10
#define OP_STR 0x11

#define OP_XOR 0x03
#define OP_AND 0x05

#define OP_ADD 0x02
#define OP_DIV 0x06
#define OP_MUL 0x07
#define OP_MOD 0x08
#define OP_SUB 0x09

#define ENCODE(op, rd, rs1, rs2_or_imm) \
    (((op)  << 22) | ((rd) << 15) | ((rs1) << 8) | ((rs2_or_imm) & 0xFF))
#define MAX_STRINGS 128
#define MAX_STRING_LENGTH 1024

#define STRING_MARKER 0xDEADBEEF