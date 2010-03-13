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


//#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* these parameters are defined only for easy code reading */
#define IN                      /* nothing */
#define OUT                     /* nothing */

typedef unsigned int            GT_BOOL;    /* Boolean: TRUE/FALSE */
typedef unsigned char           GT_U8;      /* 8-bit */
typedef          char           GT_S8;
typedef unsigned short          GT_U16;     /* 16-bit */
typedef          short          GT_S16;
typedef unsigned int            GT_U32;     /* 32-bit */
typedef          int            GT_S32;

#define GT_TRUE                 1
#define GT_FALSE                0

/* NULL pointer */
#undef NULL
#define NULL                    ((void *)0)


#include <base_macro.h>


#endif	/* __TYPEDEF_H */

