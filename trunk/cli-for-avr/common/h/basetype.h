/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    This file define the base types
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#ifndef __BASETYPE_H__
#define __BASETYPE_H__

typedef unsigned char   UI8_T;
typedef unsigned short  UI16_T;
typedef unsigned long   UI32_T;
typedef long long       I64_T;

typedef signed char     I8_T;
typedef signed short    I16_T;
typedef signed long     I32_T;
typedef unsigned long long  UI64_T;

#ifdef __CPU_AVR__
  typedef UI8_T         BOOLEAN_T;
#else
  typedef int           BOOLEAN_T;
#endif

typedef char            STR4_T[4+4];
typedef char            STR8_T[8+4];
typedef char            STR16_T[16+4];
typedef char            STR32_T[32+4];
typedef char            STR64_T[64+4];
typedef char            STR80_T[80+4];
typedef char            STR128_T[128+4];
typedef char            STR256_T[256+4];
typedef char            STR512_T[512+4];
typedef char            STR1024_T[1024+4];

/* volatile address access */
#define VP32            *(volatile UI32_T *)
#define VP16            *(volatile UI16_T *)
#define VP8             *(volatile UI8_T  *)

/* parameter type of a function(INPUT/OUTPUT),
 * have no meanings */
#define IN              /* input parameter  */
#define OUT             /* output parameter */

#define FALSE           0
#define TRUE            1

#ifndef EOF
 #define EOF            -1
#endif

#ifndef NULL
  #define NULL          ((void *)0)
#endif

#define KB(x)           ((x)*1024)
#define MB(x)           ((x)*1024*1024)

/* function start macro define, for debug use */
#define FUNCTION_BEGIN(x)

/* check pointer is NULL or not */
#define POINTER_CHECK(p)    ((p == NULL) ? TRUE : FALSE)

#ifndef NO_WARNING
  #define NO_WARNING(x) if(x){}
#endif

#define INLINE          __inline
#define REENTRANT       reentrant
#define CONST           const

/* ROM control */
#ifdef __CPU_AVR__

  #include <avr/pgmspace.h>
  #define IN_ROM        PROGMEM

  #define ROM_VOID_T    PGM_VOID_P
  #define ROM_UI8_T     prog_uchar
  #define ROM_I8_T      prog_char

  #define ROM_READ_BYTE pgm_read_byte

#else

  #define IN_ROM

  #define ROM_VOID_T    void
  #define ROM_UI8_T     UI8_T
  #define ROM_I8_T      I8_T

  #define ROM_READ_BYTE

#endif

#endif

