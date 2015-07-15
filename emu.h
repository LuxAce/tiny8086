#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 1048576
#define NO 0
#define YES 1

typedef int bool ;
typedef int BOOL ; 

enum flags
{
    CF = 0,
    PF = 2,
    AF = 4,
    ZF = 6,
    SF = 7,
    TF = 8,
    IF = 9,
    DF = 10,
    OF = 11
} ;

enum registers8
{
    AL = 0,
    CL = 1,
    DL = 2,
    BL = 3,
    AH = 4,
    CH = 5,
    DH = 6,
    BH = 7
} ;

enum registers16
{
    AX = 0,
    CX = 1,
    DX = 2,
    BX = 3,
    SP = 4,
    BP = 5,
    SI = 6,
    DI = 6
} ;

enum seg_registers
{
    ES = 0,
    CS = 1,
    SS = 2,
    DS = 3
};

typedef union
{
    unsigned short w;
    struct
    {
        unsigned char l,h;
    } b;
} word;

typedef unsigned char byte ;

struct emuctx
{
    word ip ;
    word cs ;
    word ds ;
    word ss ;
    word es ;
    word ax ;
    word bx ;
    word cx ;
    word dx ;
    word sp ;
    word si ;
    word di ;
    word bp ;
    word flags ;
    BOOL intr ;
    int cycles ;
    BOOL is_reg ;
    BOOL is_seg_ovr ; 
    unsigned char *mem ;
    byte second_op ;
    BOOL is_mem_ea ;
    signed char seg_ovr_reg ;
};

void emu_init(struct emuctx *ctx) ;

void emu_reset() ;
void emu_sent_intr() ;
void emu_step(struct emuctx *ctx) ;
void emu_write_bytes(struct emuctx *ctx, char array[], byte size) ;
void emu_cleanup(struct emuctx *ctx) ; 
int emu_exec(int cycles) ;

void debug(char *s) ;
int enabled_seg(struct emuctx *ctx);
void disable_seg_ovr(struct emuctx *ctx) ;
void set_seg_reg(struct emuctx *ctx, int reg) ;
int  get_seg_reg(struct emuctx *ctx) ;
void set_df(struct emuctx *ctx, bool value) ;
bool get_df(struct emuctx *ctx) ;
void movsb(struct emuctx *ctx) ;
void movsw(struct emuctx *ctx) ;
void cmpsb(struct emuctx *ctx) ;
word get_addrw_string(struct emuctx *ctx) ;
byte get_addrb_string(struct emuctx *ctx) ;
void set_cycles(struct emuctx *ctx, int value) ;
bool is_reg(struct emuctx *ctx) ;
bool signed_word(word w) ;
bool signed_byte(byte b) ; 
void word_af_adc(struct emuctx *ctx, word left_reg, word right_reg) ; 
void byte_af_sub(struct emuctx *ctx, byte val1, byte val2) ;
void word_af_sub(struct emuctx *ctx, word val1, word val2) ;
void word_cf_sub(struct emuctx *ctx, word val1, word val2) ;
void byte_cf_sub(struct emuctx *ctx, byte val1, byte val2) ;
void word_of_sub(struct emuctx *ctx, word val1, word val2) ;
void byte_of_sub(struct emuctx *ctx, byte val1, byte val2) ;
void byte_of_and(struct emuctx *ctx, byte val1, byte val2) ;

void inc_flags(struct emuctx *ctx, word value) ; 
void dec_flags(struct emuctx *ctx, word value) ;

void sbb_of_word(struct emuctx *ctx, word val1, word val2) ;
void sbb_af_word(struct emuctx *ctx, word val1, word val2) ;
void sbb_cf_word(struct emuctx *ctx, word val1, word val2) ; 
void sbb_af_byte(struct emuctx *ctx, byte val1, byte val2) ;
void sbb_cf_byte(struct emuctx *ctx, byte val1, byte val2) ;
void sbb_of_byte(struct emuctx *ctx, byte val1, byte val2) ;

byte second_op(struct emuctx *ctx) ;
byte get_regRM(struct emuctx *ctx) ;

void set_reg8(struct emuctx *ctx, byte reg, byte value) ;
byte get_reg8(struct emuctx *ctx, byte reg) ;
void set_reg16(struct emuctx *ctx, byte reg, word value) ;
word get_reg16(struct emuctx *ctx, byte reg) ;

void write_mem_byte(struct emuctx *ctx, int addr, byte value) ; 
byte read_mem_byte(struct emuctx *ctx, int addr) ;
word read_mem_word(struct emuctx *ctx, int addr) ;
void write_mem_word(struct emuctx *ctx, int addr, word value) ; 

byte read_byte(struct emuctx *ctx) ;
word read_word(struct emuctx *ctx) ;
byte next_byte(struct emuctx *ctx) ;
void next_ip(struct emuctx *ctx) ;
word next_word(struct emuctx *ctx) ;

int decodeEA(struct emuctx *ctx) ;

void aaa(struct emuctx *ctx) ;
void aad(struct emuctx *ctx) ;
void aam(struct emuctx *ctx) ;
void aas(struct emuctx *ctx) ;

bool get_af(struct emuctx *ctx) ;
void set_af(struct emuctx *ctx, bool value) ;
void set_af_byte(struct emuctx *ctx, byte v1, byte v2, byte result) ;
void set_af_word(struct emuctx *ctx, word v1, word v2, word result) ;

bool get_cf(struct emuctx *ctx) ;
void set_cf(struct emuctx *ctx, bool value) ;
void set_cf_byte(struct emuctx *ctx, int result) ;
void set_cf_word(struct emuctx *ctx, int result) ;

bool get_zf(struct emuctx *ctx) ;
void set_zf(struct emuctx *ctx, bool value) ;

bool get_sf(struct emuctx *ctx) ;
void set_sf(struct emuctx *ctx, bool value) ;

bool get_pf(struct emuctx *ctx) ;
void set_pf(struct emuctx *ctx, bool value) ;

bool get_of(struct emuctx *ctx) ;
void set_of(struct emuctx *ctx, bool value) ;
void set_of_byte(struct emuctx *ctx, byte v1, byte v2, byte result) ;
void set_of_word(struct emuctx *ctx, word v1, word v2, word result) ;

void is_zero_byte(struct emuctx *ctx, byte value) ;
void is_sign_byte(struct emuctx *ctx, byte value) ;
void is_parity_byte(struct emuctx *ctx, byte value) ;

void is_zero_word(struct emuctx *ctx, word value) ;
void is_sign_word(struct emuctx *ctx, word value) ;
void is_parity_word(struct emuctx *ctx, word value) ;
