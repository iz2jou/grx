/**
 ** vd_mem.c ---- driver for creating image in memory for later exporting
 **
 ** Author:  Andris Pavenis
 ** [e-mail: pavenis@acad.latnet.lv]
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

#include <stdio.h>

#include "libgrx.h"
#include "grdriver.h"
#include "allocate.h"
#include "arith.h"
#include "memfill.h"


static  char * MemBuf = NULL;
static  unsigned long MemBufSze = 0;

static void FreeMemBuf(void) {
  if (MemBuf) free(MemBuf);
  MemBuf = NULL;
  MemBufSze = 0;
}

static int AllocMemBuf(unsigned long sze) {
  int clear = 1;
  if (!MemBuf) {
    MemBuf = calloc(1,(size_t)sze);
    if (!MemBuf) return 0;
    MemBufSze = sze;
    clear = 0;
  }
  if (MemBufSze < sze) {
    MemBuf = realloc(MemBuf,(size_t)sze);
    if (!MemBuf) return 0;
    MemBufSze = sze;
  }
  if (clear) memzero(MemBuf,sze);
  return 1;
}

static int mem_setmode (GrxVideoMode *mp,int noclear);


static GrVideoModeExt gr1ext = {
    GRX_FRAME_MODE_RAM_1BPP,            /* frame driver */
    NULL,                               /* frame driver override */
    0,                                  /* frame buffer address */
    { 1, 1, 1 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoModeExt gr4ext = {
    GRX_FRAME_MODE_RAM_4BPP,            /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 8, 8, 8 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoModeExt gr8ext = {
    GRX_FRAME_MODE_RAM_8BPP,            /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 8, 8, 8 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoModeExt gr24ext = {
#ifdef GRX_USE_RAM3x8
    GRX_FRAME_MODE_RAM_3X8BPP,          /* frame driver */
#else
    GRX_FRAME_MODE_RAM_24BPP,           /* frame driver */
#endif
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 8, 8, 8 },                        /* color precisions */
#ifdef GRX_USE_RAM3x8
    { 0, 0, 0 },                        /* color component bit positions */
#else
    { 0, 8, 16 },                        /* color component bit positions */
#endif
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};


static int dummymode (GrxVideoMode * mp , int noclear )
{
    FreeMemBuf();
    return TRUE;
}


GrVideoModeExt   dummyExt = {
    GRX_FRAME_MODE_TEXT,                /* frame driver */
    NULL,                               /* frame driver override */
    MK_FP(0xb800,0),                    /* frame buffer address */
    { 0, 0, 0 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    0,                                  /* mode flag bits */
    dummymode,                          /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};




static GrxVideoMode modes[] = {
    /* pres.  bpp wdt   hgt   BIOS   scan  priv. &ext                             */
    {  TRUE,  1,  640,  480,  0x00,   80,    0,  &gr1ext                          },
    {  TRUE,  4,  640,  480,  0x00,  320,    0,  &gr4ext                          },
    {  TRUE,  8,  640,  480,  0x00,  640,    0,  &gr8ext                          },
    {  TRUE, 24,  640,  480,  0x00, 1920,    0,  &gr24ext                         },
    {  TRUE,  1,   80,   25,  0x00,  160,    0,  &dummyExt                        }
};



static int mem_setmode (GrxVideoMode *mp,int noclear)
{
     return MemBuf ? TRUE : FALSE;
}



static GrxVideoMode * mem_selectmode ( GrxVideoDriver * drv, int w, int h,
                                      int bpp, int txt, unsigned int * ep )
{
    int  index;
    unsigned long  size;
    int  LineOffset;

    if (txt) return _gr_select_mode (drv,w,h,bpp,txt,ep);

    if (w < 1 || h < 1) return NULL;

    switch (bpp)
      {
         case 1:   index = 0;
                   LineOffset = (w + 7) >> 3;
                   size = h;
                   break;
         case 4:   index = 1;
                   LineOffset = (w + 7) >> 3;
                   size = 4*h;
                   break;
         case 8:   index = 2;
                   LineOffset = w;
                   size = h;
                   break;
         case 24:  index = 3;
#ifdef GRX_USE_RAM3x8
                   LineOffset = w;
                   size = 3*h;
#else
                   LineOffset = 3*w;
                   size = h;
#endif
                   break;
         default:  return NULL;
      }

    LineOffset = (LineOffset+7) & (~7); /* align rows to 64bit boundary */
    size *= LineOffset;

    if (((size_t)size) != size) return NULL;

                               /* why ???       */
    modes[index].width       = /* w<320 ? 320 : */ w;
    modes[index].height      = /* h<200 ? 200 : */ h;
    modes[index].bpp         = bpp;
    modes[index].line_offset  = LineOffset;

    if ( AllocMemBuf(size) ) {
        modes[index].extended_info->frame = MemBuf;
        return _gr_select_mode (drv,w,h,bpp,txt,ep);
    }
    return FALSE;
}


/*
static int detect (void)
{
        return TRUE;
}
*/

static void mem_reset (void)
{
    if(DRVINFO->moderestore) {
      FreeMemBuf();
    }
}


GrxVideoDriver _GrDriverMEM = {
    "memory",                           /* name */
    GRX_VIDEO_ADAPTER_MEMORY,           /* adapter type */
    NULL,                               /* inherit modes from this driver */
    modes,                              /* mode table */
    itemsof(modes),                     /* # of modes */
    NULL, /* detect, */                 /* detection routine */
    NULL,                               /* initialization routine */
    mem_reset,                          /* reset routine */
    mem_selectmode,                     /* special mode select routine */
    GRX_VIDEO_DRIVER_FLAG_USER_RESOLUTION /* arbitrary resolution possible */
};

