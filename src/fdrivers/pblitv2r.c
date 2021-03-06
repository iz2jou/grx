/*
 * pblitv2r.c ---- video to RAM bitblt routine for packed (8, 16, 24, 32 bpp) modes
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu].
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <grx/context.h>

#include "banking.h"
#include "colors.h"
#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "mempeek.h"
#include "memcopy.h"
#include "pblit.h"


/* frame offset address calculation */
#define FOFS(x,y,lo) umuladd32((y),(lo),(x))

void _GrFrDrvPackedBitBltV2R(GrxFrame *dst,int dx,int dy,
                             GrxFrame *src,int sx,int sy,
                             int w,int h,GrxColor op)
{
        unsigned char *dptr, *sptr;
        GR_int32u soff;
        unsigned dskip, sskip;
        int oper;
        GR_int8u cval;

        GRX_ENTER();
        dptr  = &dst->base_address.plane0[FOFS(dx,dy,dst->line_offset)];
        soff  = FOFS(sx,sy,src->line_offset);
        dskip = dst->line_offset - w;
        sskip = src->line_offset - w;
        oper  = C_OPER(op);
        cval  = (GR_int8u)op;

#       define DOICPY()   DOIMGCOPY(FW,_n,_f,w1)

        setup_far_selector(src->selector);
        do {
            unsigned w1 = BANKLFT(soff);
            unsigned w2 = w - (w1 = umin(w,w1));
            do {
                sptr = &src->base_address.plane0[BANKPOS(soff)];
                CHKBANK(BANKNUM(soff));
                soff += w1;
                if (w1) switch(oper) {
                    case C_IMAGE: DOICPY();                         break;
                    case C_XOR:   fwdcopy_xor_f(sptr,dptr,sptr,w1); break;
                    case C_OR:    fwdcopy_or_f( sptr,dptr,sptr,w1); break;
                    case C_AND:   fwdcopy_and_f(sptr,dptr,sptr,w1); break;
                    default:      fwdcopy_set_f(sptr,dptr,sptr,w1); break;
                }
                w1 = w2;
                w2 = 0;
            } while(w1 != 0);
            dptr += dskip;
            soff += sskip;
        } while(--h != 0);
        GRX_LEAVE();
}
