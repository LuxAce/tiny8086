

#import "emu.h"


void emu_init(struct emuctx *ctx)
{
    ctx->mem = (unsigned char*) malloc(MEMORY_SIZE * sizeof(unsigned char)) ;
    ctx->ip.w = 0x0 ;
    ctx->cs.w = 0x0 ;
    ctx->ds.w = 0x0 ;
    ctx->ss.w = 0x0 ;
    ctx->es.w = 0x0 ;
    ctx->sp.w = 0xFFF ;
    ctx->flags.w = 0 ;
    ctx->is_seg_ovr = 0 ; 
}

void emu_reset()
{
    
}

void emu_sent_intr()
{
    
}

int exec(int cycles)
{
    return 0 ;
}

void emu_write_bytes(struct emuctx *ctx, char array[], byte size)
{
    int i = 0 ;
    for (i=0; i < size; i++)
    {
        ctx->mem[(ctx->cs.w << 4) + ctx->ip.w + i] = array[i] ;
    }
}

void emu_cleanup(struct emuctx *ctx)
{
    free(ctx->mem) ;
}

void byte_af_sub(struct emuctx *ctx, byte val1, byte val2)
{
    /*
    byte two_comp = (~val2)+1 ;
    if (((val1 & 0x0F) + (two_comp & 0x0F)) & 0x10) set_af(ctx, NO);
    else set_af(ctx, YES) ;
    */
    if (((val1 & 0x0F) - (val2 & 0x0F)) & 0x10) set_af(ctx, YES);
    else set_af(ctx, NO) ;
}

void byte_cf_sub(struct emuctx *ctx, byte val1, byte val2)
{
    /*
    byte two_comp = (~val2)+1 ;
    unsigned int result = val1 + two_comp ;
    if (result & 0x100) set_cf(ctx, NO) ;
    else set_cf(ctx, YES) ;
    */
    unsigned int result = val1 - val2 ;
    if (result & 0x100) set_cf(ctx, YES) ;
    else set_cf(ctx, NO) ;
}

void word_cf_sub(struct emuctx *ctx, word val1, word val2)
{
    /*
    word two_comp ;
    two_comp.w = (~val2.w) + 1 ;
    unsigned int result = val1.w + two_comp.w ;
    if (result & 0x10000) set_cf(ctx, NO) ;
    else set_cf(ctx, YES) ; 
     */
    unsigned int result = val1.w - val2.w ;
    if (result & 0x10000) set_cf(ctx, YES) ;
    else set_cf(ctx, NO) ;
}

void word_af_adc(struct emuctx *ctx, word left_reg, word right_reg)
{
    if (((left_reg.w & 0x0F) + (right_reg.w & 0x0F) + get_cf(ctx)) & 0x10) set_af(ctx, YES) ;
    else set_af(ctx, NO) ;
}

void word_af_sub(struct emuctx *ctx, word val1, word val2)
{
    byte result = ((val1.w & 0xF) - (val2.w & 0xF));
    if (result & 0x10 ) set_af(ctx, YES) ;
    else { set_af(ctx, NO) ;}
}

void sbb_af_word(struct emuctx *ctx, word val1, word val2)
{
    /*
    word two_comp ;
    two_comp.w = (~val2.w) + 1 ;
    if (get_cf(ctx))
    {
        if (((val1.w & 0x0F) + (two_comp.w & 0x0F) + 0x0F) & 0x10) set_af(ctx, YES) ;
        else set_af(ctx, NO) ;
    }
    else
    {
        if (((val1.w & 0x0F) + (two_comp.w & 0x0F)) & 0x10) set_af(ctx, NO) ;
        else set_af(ctx, YES) ;
    }
    */
    if (get_cf(ctx))
    {
        if (((val1.w & 0x0F) - (val2.w & 0x0F) - 1) & 0x10) set_af(ctx, YES) ;
        else set_af(ctx, NO) ;
    }
    else
    {
        if (((val1.w & 0x0F) - (val2.w & 0x0F)) & 0x10) set_af(ctx, YES) ;
        else set_af(ctx, NO) ;
    }

}

void sbb_af_byte(struct emuctx *ctx, byte val1, byte val2)
{
    /*
    byte two_comp = (~val2)+1 ;
    if (get_cf(ctx))
    {
        if (((val1 & 0x0F) + (two_comp & 0x0F) + 0x0F) & 0x10) set_af(ctx, YES);
        else set_af(ctx, NO) ;
    }
    else
    {
        if (((val1 & 0x0F) + (two_comp & 0x0F)) & 0x10) set_af(ctx, NO);
        else set_af(ctx, YES) ;
    }
     */
    if (get_cf(ctx))
    {
        if (((val1 & 0x0F) - (val1 & 0x0F) - 1) & 0x10) set_af(ctx, YES);
        else set_af(ctx, NO) ;
    }
    else
    {
        if (((val1 & 0x0F) - (val2 & 0x0F)) & 0x10) set_af(ctx, YES);
        else set_af(ctx, NO) ;
    }

}

void sbb_cf_byte(struct emuctx *ctx, byte val1, byte val2)
{
    /*
    byte two_comp = (~val2)+1 ;
    if (get_cf(ctx))
    {
        if ((val1 + two_comp + 0xff) & 0x100) set_cf(ctx, YES) ;
        else set_cf(ctx, NO) ;
    }
    else
    {
        if ((val1 + two_comp) & 0x100) set_cf(ctx, NO) ;
        else set_cf(ctx, YES) ;
    }
    */
    if (get_cf(ctx))
    {
        if ((val1 - val2 - 1) & 0x100) set_cf(ctx, YES) ;
        else set_cf(ctx, NO) ;
    }
    else
    {
        if ((val1 - val2) & 0x100) set_cf(ctx, YES) ;
        else set_cf(ctx, NO) ;
    }
}


void sbb_cf_word(struct emuctx *ctx, word val1, word val2)
{
    /*
    word two_comp ;
    two_comp.w = (~val2.w) + 1 ;
    if (get_cf(ctx) == YES)
    {
        if ((val1.w + two_comp.w + 0xFFFF) & 0x10000) set_cf(ctx, YES) ;
        else set_cf(ctx, NO) ;
    }
    else
    {
        if ((val1.w + two_comp.w) & 0x10000) set_cf(ctx, NO) ;
        else set_cf(ctx, YES) ; 
    }
    */
    if (get_cf(ctx) == YES)
    {
        if ((val1.w - val2.w - 1) & 0x10000) set_cf(ctx, YES) ;
        else set_cf(ctx, NO) ;
    }
    else
    {
        if ((val1.w - val2.w) & 0x10000) set_cf(ctx, YES) ;
        else set_cf(ctx, NO) ;
    }
}

void sbb_of_word(struct emuctx *ctx, word val1, word val2)
{
    word two_comp ;
    two_comp.w = (~val2.w) + 1 ;
    if (get_cf(ctx) == YES)
    {
        word result ;
        result.w = val1.w + two_comp.w + 0xFFFF ;
        two_comp.w = two_comp.w + 0xFFFF ;
        if (!signed_word(val1) && !signed_word(two_comp) && signed_word(result)) set_of(ctx, YES) ;
        else if (signed_word(val1) && signed_word(two_comp) && !signed_word(result)) set_of(ctx,YES) ;
        else set_of(ctx, NO) ;
    }
    else
    {
        word result;
        result.w = val1.w + two_comp.w ;
        if (!signed_word(val1) && !signed_word(two_comp) && signed_word(result)) set_of(ctx, YES) ;
        else if (signed_word(val1) && signed_word(two_comp) && !signed_word(result)) set_of(ctx,YES) ;
        else set_of(ctx, NO) ;
    }
}

void word_of_sub(struct emuctx *ctx, word val1, word val2)
{
    word two_comp ;
    two_comp.w = (~val2.w) + 1 ;
    word result ;
    result.w = val1.w + two_comp.w;
    if (!signed_word(val1) && !signed_word(two_comp) && signed_word(result)) set_of(ctx, YES) ;
    else if (signed_word(val1) && signed_word(two_comp) && !signed_word(result)) set_of(ctx,YES) ;
    else set_of(ctx, NO) ;
}

void byte_of_sub(struct emuctx *ctx, byte val1, byte val2)
{
    byte two_comp = (~val2)+1 ;
    byte result = val1 + two_comp;
    if (!signed_byte(val1) && !signed_byte(two_comp) && signed_byte(result)) set_of(ctx, YES) ;
    else if (signed_byte(val1) && signed_byte(two_comp) && !signed_byte(result)) set_of(ctx, YES) ;
    else set_of(ctx, NO) ;
}

void byte_of_and(struct emuctx *ctx, byte val1, byte val2)
{
    byte result = val1 & val2;
    if (!signed_byte(val1) && !signed_byte(val2) && signed_byte(result)) set_of(ctx, YES) ;
    else if (signed_byte(val1) && signed_byte(val2) && !signed_byte(result)) set_of(ctx, YES) ;
    else set_of(ctx, NO) ;
}

void sbb_of_byte(struct emuctx *ctx, byte val1, byte val2)
{
    byte two_comp = (~val2)+1 ;
    if (get_cf(ctx))
    {
        byte result = val1 + two_comp + 0xFF ;
        two_comp = two_comp + 0xFF ;
        if (!signed_byte(val1) && !signed_byte(two_comp) && signed_byte(result)) set_of(ctx, YES) ;
        else if (signed_byte(val1) && signed_byte(two_comp) && !signed_byte(result)) set_of(ctx, YES) ;
        else set_of(ctx, NO) ;
                 
    }
    else
    {
        byte result = val1 + two_comp + 0xFF ;
        if (!signed_byte(val1) && !signed_byte(two_comp) && signed_byte(result)) set_of(ctx, YES) ;
        else if (signed_byte(val1) && signed_byte(two_comp) && !signed_byte(result)) set_of(ctx, YES) ;
        else set_of(ctx, NO) ;
    }
}

bool signed_byte(byte b)
{
    if (b & 0x80) return YES ;
    else return NO ;
}

bool signed_word(word w)
{
    if (w.w & 0x8000) return YES ;
    else return NO ;
}

bool is_reg(struct emuctx *ctx)
{
    return ctx->is_reg ? YES : NO ;
}

byte second_op(struct emuctx *ctx)
{
    return ctx->second_op ;
}

byte get_regRM(struct emuctx *ctx)
{
    byte reg = ((ctx->second_op & 0x38) >> 0x03) ;
    return reg ;
}

void set_reg16(struct emuctx *ctx, byte reg, word value)
{
    switch (reg)
    {
        case 0x0:
        { // ax
            ctx->ax.w = value.w ;
        } break ;
        case 0x01:
        { // cx
            ctx->cx.w = value.w ;
        } break ;
        case 0x02:
        { // dx
            ctx->dx.w = value.w ;
        } break ;
        case 0x03:
        { // bx
            ctx->bx.w = value.w ;
        } break ;
        case 0x04:
        { // sp
            ctx->sp.w = value.w ;
        } break ;
        case 0x05:
        { // bp
            ctx->bp.w = value.w ;
        } break ;
        case 0x06:
        { // si
            ctx->si.w = value.w ;
        } break ;
        case 0x07:
        { // di
            ctx->di.w = value.w ;
        } break ;
    }
}

void set_reg8(struct emuctx *ctx, byte reg, byte value)
{
    switch (reg)
    {
        case 0x0:
        { // al
            ctx->ax.b.l = value ;
        } break ;
        case 0x01:
        { // cl
            ctx->cx.b.l = value ;
        } break ;
        case 0x02:
        { // dl
            ctx->dx.b.l = value ;
        } break ;
        case 0x03:
        { // bl
            ctx->bx.b.l = value ;
        } break ;
        case 0x04:
        { // ah
            ctx->ax.b.h = value ;
        } break ;
        case 0x05:
        { // ch
            ctx->cx.b.h = value ;
        } break ;
        case 0x06:
        { // dh
            ctx->dx.b.h = value ;
        } break ;
        case 0x07:
        { // bh
            ctx->bx.b.h = value ;
        } break ;
    }
}

word get_reg16(struct emuctx *ctx, byte reg)
{
    switch (reg)
    {
        case 0x0:
        { // ax
            return ctx->ax ;
        } break ;
        case 0x01:
        { // cx
            return ctx->cx ;
        } break ;
        case 0x02:
        { // dx
            return ctx->dx ;
        } break ;
        case 0x03:
        { // bx
            return ctx->bx ;
        } break ;
        case 0x04:
        { // sp
            return ctx->sp;
        } break ;
        case 0x05:
        { // bp
            return ctx->bp ;
        } break ;
        case 0x06:
        { // si
            return ctx->si ;
        } break ;
        case 0x07:
        { // di
            return ctx->di;
        } break ;
    }
    word return_val;
    return_val.w = 0 ;
    return  return_val;
}

byte get_reg8(struct emuctx *ctx, byte reg)
{
    switch (reg)
    {
        case 0x0:
        { // al
            return ctx->ax.b.l ;
        } break ;
        case 0x01:
        { // cl
            return ctx->cx.b.l ;
        } break ;
        case 0x02:
        { // dl
            return ctx->dx.b.l ;
        } break ;
        case 0x03:
        { // bl
            return ctx->bx.b.l ;
        } break ;
        case 0x04:
        { // ah
            return ctx->ax.b.h ;
        } break ;
        case 0x05:
        { // ch
            return ctx->cx.b.h ;
        } break ;
        case 0x06:
        { // dh
            return ctx->dx.b.h ;
        } break ;
        case 0x07:
        { // bh
            return ctx->bx.b.h ;
        } break ;
    }
    return 0 ; 
}

int decodeEA(struct emuctx *ctx)
{
    byte second_byte = next_byte(ctx) ;
    ctx->second_op = second_byte ;
    byte mod = (second_byte & 0xE0) >> 6 ;
    byte rm =  (second_byte & 0x07) ;
    int addr ;
    word disp ;
    disp.w = 0 ;
    ctx->is_reg = NO ;
    BOOL no_disp = NO ;
    BOOL is_disp = NO ;
    switch (mod)
    {
        case 0x00:
        {
            disp.w = 0 ;
            no_disp = YES  ;
            is_disp = NO ;
        } break ;
        case 0x01:
        {
            signed char value = next_byte(ctx) ;
            disp.w = (signed short) value ;
            is_disp = YES ;
        } break  ;
        case 0x02:
        {
            disp = next_word(ctx) ;
            is_disp = YES ;
        } break ;
        case 0x03:
        {
            ctx->is_reg = YES ;
            // assign reg value to return value
            disp.w = rm ;
            return disp.w ;
        } break ;
    }
    int sgmt_over_addr = 0 ;
    switch (rm)
    {
        case 0x00:
        {
            if (ctx->is_seg_ovr) addr = sgmt_over_addr + ctx->bx.w + ctx->si.w + disp.w ;
            else addr = (ctx->ds.w << 4) + ctx->bx.w + ctx->si.w + disp.w ;
            if (is_disp == YES) set_cycles(ctx, 11) ;
            else set_cycles(ctx, 7) ;
        } break ;
        case 0x01:
        {
            if (ctx->is_seg_ovr) addr = sgmt_over_addr + ctx->bx.w + ctx->di.w + disp.w ;
            else addr = (ctx->ds.w << 4) + ctx->bx.w + ctx->di.w + disp.w ;
            if (is_disp == YES) set_cycles(ctx, 12) ;
            else set_cycles(ctx, 8) ;
        } break ;
        case 0x02:
        {
            if (ctx->is_seg_ovr) addr = sgmt_over_addr + ctx->bp.w + ctx->si.w + disp.w ;
            else addr = (ctx->ss.w << 4) + ctx->bp.w + ctx->si.w + disp.w ;
            if (is_disp == YES) set_cycles(ctx, 12) ;
            else set_cycles(ctx, 8) ;
        } break ;
        case 0x03:
        {
            if (ctx->is_seg_ovr) addr = sgmt_over_addr + ctx->bp.w + ctx->di.w + disp.w ;
            else addr = (ctx->ss.w << 4) + ctx->bp.w + ctx->di.w + disp.w ;
            if (is_disp == YES) set_cycles(ctx, 11) ;
            else set_cycles(ctx, 7) ;
        } break ;
        case 0x04:
        {
            if (ctx->is_seg_ovr) addr = sgmt_over_addr + ctx->si.w + disp.w ;
            else addr = (ctx->ds.w << 4) + ctx->si.w + disp.w ;
            if (is_disp == YES) set_cycles(ctx, 9) ;
            else set_cycles(ctx, 5) ;
        } break ;
        case 0x05:
        {
            if (ctx->is_seg_ovr) addr = sgmt_over_addr + ctx->di.w + disp.w ;
            else addr = (ctx->ds.w << 4) + ctx->di.w + disp.w ;
            if (is_disp == YES) set_cycles(ctx, 9) ;
            else set_cycles(ctx, 5) ;
        } break ;
        case 0x06:
        {
            if (no_disp == YES)
            {
                addr = next_word(ctx).w ;
                set_cycles(ctx, 6) ;
            }
            else
            {
                if (ctx->is_seg_ovr) addr = sgmt_over_addr + ctx->bp.w + disp.w ;
                else addr = (ctx->ss.w << 4) + ctx->bp.w + disp.w ;
                if (is_disp == YES) set_cycles(ctx, 9) ;
                else set_cycles(ctx, 5) ;
            }
        } break ;
        case 0x07:
        {
            if (ctx->is_seg_ovr) addr = sgmt_over_addr + ctx->bx.w + disp.w ;
            else addr = (ctx->ds.w << 4) + ctx->bx.w + disp.w ;
            if (is_disp == YES) set_cycles(ctx, 9) ;
            else set_cycles(ctx, 5) ;
        } break ;
    }
    return addr ;
}

void debug(char *s)
{
    //NSLog(@"%s",s) ;
}

byte read_mem_byte(struct emuctx *ctx, int addr)
{
    return ctx->mem[addr] ;
}

void write_mem_byte(struct emuctx *ctx, int addr, byte value)
{
    ctx->mem[addr] = value ;
}

word read_mem_word(struct emuctx *ctx, int addr)
{
    byte low = ctx->mem[addr] ;
    byte high = ctx->mem[addr+1] ;
    word result ;
    result.w = ((high << 8) + low) ;
    return result ;
}

void write_mem_word(struct emuctx *ctx, int addr, word value)
{
    word w = value ;
    byte low = (w.w & 0x00FF) ;
    byte high = (w.w & 0xFF00) >> 8 ;
    ctx->mem[addr] = low ;
    ctx->mem[addr+1] = high ;
}

byte read_byte(struct emuctx *ctx)
{
    int addr = (ctx->cs.w << 4) + ctx->ip.w ;
    return ctx->mem[addr] ;
}

word read_word(struct emuctx *ctx)
{
    int addr = (ctx->cs.w << 4) + ctx->ip.w ;
    byte low = ctx->mem[addr] ;
    byte high = ctx->mem[addr + 1] ;
    word value;
    value.w = (high << 8) + low ;
    return value ;
}

byte next_byte(struct emuctx *ctx)
{
    int addr = (ctx->cs.w << 4) + ctx->ip.w ;
    ctx->ip.w++ ;
    return ctx->mem[addr] ;
}

word next_word(struct emuctx *ctx)
{
    int addr = (ctx->cs.w << 4) + ctx->ip.w ;
    byte low = ctx->mem[addr] ;
    ctx->ip.w++ ;
    addr = (ctx->cs.w << 4) + ctx->ip.w ;
    byte high = ctx->mem[addr] ;
    ctx->ip.w++ ;
    word value;
    value.w = (high << 8) + low ;
    return value ;
}

void next_ip(struct emuctx *ctx)
{
    ctx->ip.w++;
}

void set_cycles(struct emuctx *ctx, int value)
{
    ctx->cycles = ctx->cycles - value ; 
}

bool get_af(struct emuctx *ctx)
{
    word flags = ctx->flags ;
    return (flags.w & (1 << AF)) ? YES : NO ;
}

void set_af(struct emuctx *ctx, bool value)
{
    if (value) ctx->flags.w |= (1 << AF) ;
    else ctx->flags.w &= ~(1 << AF) ;
}

bool get_cf(struct emuctx *ctx)
{
    word flags = ctx->flags ;
    return (flags.w & (1 << CF)) ? YES : NO ;
}

void set_cf(struct emuctx *ctx, bool value)
{
    if (value) ctx->flags.w |= (1 << CF) ;
    else ctx->flags.w &= ~(1 << CF) ;
}

void set_cf_byte(struct emuctx *ctx, int result)
{
    if (result & 0x100) set_cf(ctx, YES) ;
    else set_cf(ctx, NO) ;
}

void set_cf_word(struct emuctx *ctx, int result)
{
    if (result & 0x10000) set_cf(ctx, YES) ;
    else set_cf(ctx, NO) ;
}

bool get_zf(struct emuctx *ctx)
{
    word flags = ctx->flags ;
    return (flags.w & (1 << ZF)) ? YES : NO ;
}
void set_zf(struct emuctx *ctx, bool value)
{
    if (value) ctx->flags.w |= (1 << ZF) ;
    else ctx->flags.w &= ~(1 << ZF) ;
}

bool get_sf(struct emuctx *ctx)
{
    word flags = ctx->flags ;
    return (flags.w & (1 << SF)) ? YES : NO ;
}

void set_sf(struct emuctx *ctx, bool value)
{
    if (value) ctx->flags.w |= (1 << SF) ;
    else ctx->flags.w &= ~(1 << SF) ;
}

bool get_pf(struct emuctx *ctx)
{
    word flags = ctx->flags ;
    return (flags.w & (1 << PF)) ? YES : NO ;
}

void set_pf(struct emuctx *ctx, bool value)
{
    if (value) ctx->flags.w |= (1 << PF) ;
    else ctx->flags.w &= ~(1 << PF) ;
}

bool get_of(struct emuctx *ctx)
{
    word flags = ctx->flags ;
    return (flags.w & (1 << OF)) ? YES : NO ;
}

void set_of(struct emuctx *ctx, bool value)
{
    if (value) ctx->flags.w |= (1 << OF) ;
    else ctx->flags.w &= ~(1 << OF) ;
}

void set_of_byte(struct emuctx *ctx, byte v1, byte v2, byte result)
{
    if ((v1 & 0x80) && (v2 & 0x80) && !(result & 0x80)) set_of(ctx, YES) ;
    else if  (!(v1 & 0x80) && !(v2 & 0x80) && (result & 0x80)) set_of(ctx, YES) ;
    else set_of(ctx, NO) ;
}

void set_of_word(struct emuctx *ctx, word v1, word v2, word result)
{
    if ((v1.w & 0x8000) && (v2.w & 0x8000) && !(result.w & 0x8000)) set_of(ctx, YES) ;
    else if  (!(v1.w & 0x8000) && !(v2.w & 0x8000) && (result.w & 0x8000)) set_of(ctx, YES) ;
    else set_of(ctx, NO) ;
}

void inc_flags(struct emuctx *ctx, word value)
{
    word result ;
    result.w = value.w + 1 ;
    if (((value.w & 0x0F) + 1) > 0x0F) set_af(ctx, YES) ;
    else set_af(ctx, NO) ;
    word one ;
    one.w = 1;
    set_of_word(ctx, value, one, result) ;
    is_parity_word(ctx, value) ;
    is_sign_word(ctx, value) ;
    is_zero_word(ctx, value) ;
}

void dec_flags(struct emuctx *ctx, word value)
{
    word result ;
    result.w = value.w - 1 ;
    word one ;
    one.w = 1;
    word_af_sub(ctx, value, one) ;
    set_of_word(ctx, value, one, result) ;
    is_parity_word(ctx, result) ;
    is_sign_word(ctx, result) ;
    is_zero_word(ctx, result) ;
}

void aaa(struct emuctx *ctx)
{
    if ((ctx->ax.b.l & 0x0f) > 9 || get_af(ctx))
    {
        ctx->ax.b.l = ctx->ax.b.l + 6 ;
        ctx->ax.b.h = ctx->ax.b.h + 1 ;
        set_af(ctx,  YES) ;
    }
    set_cf(ctx, get_af(ctx)) ;
    ctx->ax.b.l = ctx->ax.b.l & 0x0F ;
    set_cycles(ctx, 8) ;
}

void aad(struct emuctx *ctx)
{
    ctx->ax.b.l = ctx->ax.b.h * 0x0A + ctx->ax.b.l ;
    is_zero_word(ctx, ctx->ax) ;
    is_sign_word(ctx, ctx->ax) ;
    is_parity_word(ctx, ctx->ax) ;
    set_cycles(ctx, 60) ;
}

void aam(struct emuctx *ctx)
{
    ctx->ax.b.h = ctx->ax.b.l / 0x0A ;
    ctx->ax.b.l = ctx->ax.b.l % 0x0A ;
    is_zero_word(ctx, ctx->ax) ;
    is_sign_word(ctx, ctx->ax) ;
    is_parity_word(ctx, ctx->ax) ;
    set_cycles(ctx, 83) ;
}

void aas(struct emuctx *ctx)
{
    if ((ctx->ax.b.l & 0x0F) > 9 || get_af(ctx))
    {
        ctx->ax.b.l = ctx->ax.b.l - 6 ;
        ctx->ax.b.h = ctx->ax.b.h - 1 ;
        set_af(ctx, YES) ;
        set_af(ctx, get_af(ctx)) ;
    }
    ctx->ax.b.l = ctx->ax.b.l & 0x0F ;
    set_cycles(ctx, 8) ;
}

void is_zero_byte(struct emuctx *ctx, byte value)
{
    if (value == 0) set_zf(ctx, YES) ;
    else set_zf(ctx, NO) ;
}

void is_sign_byte(struct emuctx *ctx, byte value)
{
    if (value & 0x80) set_sf(ctx, YES) ;
    else set_sf(ctx, NO) ;
}

void is_parity_byte(struct emuctx *ctx, byte value)
{
    int i = 0 ;
    int v = 1 ;
    int p = 0 ;
    for (i=0; i < 8; i++)
    {
        if (value & v) p++ ;
        v = v << 1 ;
    }
    if ((p & 0x1)) set_pf(ctx, YES) ;
    else set_pf(ctx, NO) ;
}

void is_zero_word(struct emuctx *ctx, word value)
{
    if (value.w == 0) set_zf(ctx, YES) ;
    else set_zf(ctx, NO) ;
}

void is_sign_word(struct emuctx *ctx, word value)
{
    if (value.w & 0x8000) set_sf(ctx, YES) ;
    else set_sf(ctx, NO) ;
}

void is_parity_word(struct emuctx *ctx, word value)
{
    int i = 0 ;
    int v = 1 ;
    int p = 0 ;
    for (i=0; i < 8; i++)
    {
        if (value.w & v) p++ ;
        v = v << 1 ;
    }
    if ((p & 0x1)) set_pf(ctx, YES) ;
    else set_pf(ctx, NO) ;
}

