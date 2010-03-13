/******************************************************************************
 *
 * COPYRIGHT:
 *	 Copyright (c)	2005-2010	Source Photonics Inc.	 All rights reserved.
 *
 *	 This is unpublished proprietary source code of Source Photonics Inc.
 *	 The copyright notice above does not evidence any actual or intended
 *	 publication of such source code.
 *
 * FILE NAME:
 *	 base_macro.h
 * NODULE NAME:
 *	 N/A
 * DESCRIPTION:
 *	 N/A
 * HISTORY:
 *	 2009.4.13		  Panda.Xiong		  Create/Update
 *
*****************************************************************************/

#ifndef __BASE_MACRO_H
#define __BASE_MACRO_H

//#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define Delay_Us(x)         do { volatile int n = (x); while(n--) {} } while (0)

/* for CVI */
#ifndef __GNUC__

 #include <cvidef.h>
 #include <utility.h>

 #define __attribute__(x)	/* nothing */
 #define Delay_Ms(x)         Delay((float)(x) * 0.001)

#else

 #include <windows.h>

 /* the Sleep() API only can be minimum to delay 30ms */
 #define Delay_Ms(x)         Sleep(((x) + 30 -1) / 30)

#endif


/* volatile address access */
#define VP32                *(volatile GT_U32 *)
#define VP16                *(volatile GT_U16 *)
#define VP8                 *(volatile GT_U8  *)

#define KB(x)				((x)*1024)
#define MB(x)				((x)*1024*1024)

/* calculate the total element of an array */
#define COUNT_OF(n)			(GT_S32)(sizeof(n) / sizeof((n)[0]))

/* remove warning */
#define NO_WARNING(x)		do { if (x) {} } while (0)


#ifndef MIN
 #define	MIN(a, b)		(((a)<(b))?(a):(b))
#endif

#ifndef MAX
 #define	MAX(a, b)		(((a)>(b))?(a):(b))
#endif


/* bit operation */
#define SET_BIT(n, b)		do { (n) |= (1 << (b));  } while (0)
#define CLR_BIT(n, b)		do { (n) &= ~(1 << (b)); } while (0)
#define READ_BIT(n, b)		(((n) >> (b)) & 0x1)


/* get the absolute value */
#define ABS(x)				((x) >= 0 ? (x) : -(x))


#endif /* __BASE_MACRO_H */

