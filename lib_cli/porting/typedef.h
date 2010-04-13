/******************************************************************************
 *
 * COPYRIGHT:
 *   Copyright (c) 2005-2010  SourcePhotonics Corporation  All rights reserved
 *
 *   This is unpublished proprietary source code of SourcePhotonics Corporation.
 *   The copyright notice above does not evidence any actual or intended
 *   publication of such source code.
 *
 * MODULE NAME:
 *   typedef.h
 * DESCRIPTION:
 *   Base type definition.
 * HISTORY:
 *   V1.00      2009.4.8     Panda Xiong       Update
 *
******************************************************************************/

#ifndef __TYPEDEF_H
#define __TYPEDEF_H


#include <windows.h>


/* these parameters are defined only for easy code reading */
#define IN                      /* nothing */
#define OUT                     /* nothing */

/*typedef unsigned int          BOOL;*/     /* Boolean: TRUE/FALSE */
typedef unsigned char           UINT8;      /* 8-bit */
typedef          char           SINT8;
typedef unsigned short          UINT16;     /* 16-bit */
typedef          short          SINT16;
typedef unsigned int            UINT32;     /* 32-bit */
typedef          int            SINT32;

#define TRUE                    1
#define FALSE                   0

/* NULL pointer */
#undef  NULL
#define NULL                    ((void *)0)


#include "base_macro.h"


#endif	/* __TYPEDEF_H */

