/**
 ** int86.h ---- common ground for DOS/BIOS interrupts under TCC and DJGPP
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#ifndef __INT86_H_INCLUDED__
#define __INT86_H_INCLUDED__

#ifndef __MEMMODE_H_INCLUDED__
#include "memmode.h"
#endif

#if defined(MSDOS) && defined(_MSC_VER)

#include <dos.h>

typedef union _REGS Int86Regs;
#define int10(iregp)    _int86(0x10,(iregp),(iregp))
#define int11(iregp)    _int86(0x11,(iregp),(iregp))
#define int16(iregp)    _int86(0x16,(iregp),(iregp))
#define int33(iregp)    _int86(0x33,(iregp),(iregp))

#define IREG_AX(iregs)  ((iregs).x.ax)
#define IREG_BX(iregs)  ((iregs).x.bx)
#define IREG_CX(iregs)  ((iregs).x.cx)
#define IREG_DX(iregs)  ((iregs).x.dx)
#define IREG_SI(iregs)  ((iregs).x.si)
#define IREG_DI(iregs)  ((iregs).x.di)
/* ==== IREG_BP() not implemented ==== */
/* ==== IREG_DS() not implemented ==== */
/* ==== IREG_ES() not implemented ==== */


#define IREG_AL(iregs)  ((iregs).h.al)
#define IREG_AH(iregs)  ((iregs).h.ah)
#define IREG_BL(iregs)  ((iregs).h.bl)
#define IREG_BH(iregs)  ((iregs).h.bh)
#define IREG_CL(iregs)  ((iregs).h.cl)
#define IREG_CH(iregs)  ((iregs).h.ch)
#define IREG_DL(iregs)  ((iregs).h.dl)
#define IREG_DH(iregs)  ((iregs).h.dh)

#endif  /* _MSC_VER */

#ifdef  __MSDOS__
#ifdef  __TURBOC__

typedef struct REGPACK  Int86Regs;
#define int10(iregp)    intr(0x10,(iregp))
#define int11(iregp)    intr(0x11,(iregp))
#define int16(iregp)    intr(0x16,(iregp))
#define int33(iregp)    intr(0x33,(iregp))

#define IREG_AX(iregs)  ((iregs).r_ax)
#define IREG_BX(iregs)  ((iregs).r_bx)
#define IREG_CX(iregs)  ((iregs).r_cx)
#define IREG_DX(iregs)  ((iregs).r_dx)
#define IREG_SI(iregs)  ((iregs).r_si)
#define IREG_DI(iregs)  ((iregs).r_di)
#define IREG_BP(iregs)  ((iregs).r_bp)
#define IREG_DS(iregs)  ((iregs).r_ds)
#define IREG_ES(iregs)  ((iregs).r_es)

#define IREG_AL(iregs)  (((unsigned char *)(&(iregs).r_ax))[0])
#define IREG_AH(iregs)  (((unsigned char *)(&(iregs).r_ax))[1])
#define IREG_BL(iregs)  (((unsigned char *)(&(iregs).r_bx))[0])
#define IREG_BH(iregs)  (((unsigned char *)(&(iregs).r_bx))[1])
#define IREG_CL(iregs)  (((unsigned char *)(&(iregs).r_cx))[0])
#define IREG_CH(iregs)  (((unsigned char *)(&(iregs).r_cx))[1])
#define IREG_DL(iregs)  (((unsigned char *)(&(iregs).r_dx))[0])
#define IREG_DH(iregs)  (((unsigned char *)(&(iregs).r_dx))[1])

#endif  /* __TURBOC__ */

#ifdef  __GNUC__
#ifdef  I386_GCC_FAR_MEMORY

#include <dpmi.h>

typedef _go32_dpmi_registers Int86Regs;
#define int10(iregp)    _go32_dpmi_simulate_int(0x10,(iregp))
#define int11(iregp)    _go32_dpmi_simulate_int(0x11,(iregp))
#define int16(iregp)    _go32_dpmi_simulate_int(0x16,(iregp))
#define int33(iregp)    _go32_dpmi_simulate_int(0x33,(iregp))

#define IREG_AX(iregs)  ((iregs).x.ax)
#define IREG_BX(iregs)  ((iregs).x.bx)
#define IREG_CX(iregs)  ((iregs).x.cx)
#define IREG_DX(iregs)  ((iregs).x.dx)
#define IREG_SI(iregs)  ((iregs).x.si)
#define IREG_DI(iregs)  ((iregs).x.di)
#define IREG_BP(iregs)  ((iregs).x.bp)
#define IREG_DS(iregs)  ((iregs).x.ds)
#define IREG_ES(iregs)  ((iregs).x.es)

#define IREG_AL(iregs)  ((iregs).h.al)
#define IREG_AH(iregs)  ((iregs).h.ah)
#define IREG_BL(iregs)  ((iregs).h.bl)
#define IREG_BH(iregs)  ((iregs).h.bh)
#define IREG_CL(iregs)  ((iregs).h.cl)
#define IREG_CH(iregs)  ((iregs).h.ch)
#define IREG_DL(iregs)  ((iregs).h.dl)
#define IREG_DH(iregs)  ((iregs).h.dh)

#else   /* I386_GCC_FAR_MEMORY */

#include <dos.h>

typedef union REGS      Int86Regs;
#define int10(iregp)    int86(0x10,(iregp),(iregp))
#define int11(iregp)    int86(0x11,(iregp),(iregp))
#define int16(iregp)    int86(0x16,(iregp),(iregp))
#define int33(iregp)    int86(0x33,(iregp),(iregp))

#define IREG_AX(iregs)  ((iregs).x.ax)
#define IREG_BX(iregs)  ((iregs).x.bx)
#define IREG_CX(iregs)  ((iregs).x.cx)
#define IREG_DX(iregs)  ((iregs).x.dx)
#define IREG_SI(iregs)  ((iregs).x.si)
#define IREG_DI(iregs)  ((iregs).x.di)
/* ==== IREG_BP() not implemented ==== */
/* ==== IREG_DS() not implemented ==== */
/* ==== IREG_ES() not implemented ==== */

#define IREG_AL(iregs)  ((iregs).h.al)
#define IREG_AH(iregs)  ((iregs).h.ah)
#define IREG_BL(iregs)  ((iregs).h.bl)
#define IREG_BH(iregs)  ((iregs).h.bh)
#define IREG_CL(iregs)  ((iregs).h.cl)
#define IREG_CH(iregs)  ((iregs).h.ch)
#define IREG_DL(iregs)  ((iregs).h.dl)
#define IREG_DH(iregs)  ((iregs).h.dh)

#endif  /* I386_GCC_FAR_MEMORY */
#endif  /* __GNUC__ */
#endif  /* __MSDOS__ */

/* all int10 calls using the XFER_BUFFER _must_ use int10x ! */
#ifndef int10x
#define int10x(r) int10(r)
#endif

#endif  /* whole file */

