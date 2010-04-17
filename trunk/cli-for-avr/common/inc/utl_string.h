/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    None
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#ifndef __UTL_STRING_H__
#define __UTL_STRING_H__

#include <basetype.h>

#if 0
I8_T ATOI8(const UI8_T *str);

void *MEMSET(void *m, const I8_T c, const UI8_T n);

void *MEMCPY(void *dst, const void *src, const UI8_T len);

I8_T MEMCMP(const void *s1v, const void *s2v, UI8_T size);

UI8_T STRLEN(const UI8_T *string);

UI8_T *STRCPY(UI8_T *to, const UI8_T *from);

UI8_T *STRNCPY(UI8_T *to, const UI8_T *from, UI8_T count);

I8_T STRCMP(const UI8_T *_s1, const UI8_T *_s2);

#else

#include <string.h>

#define MEMSET  memset
#define MEMCPY  memcpy
#define MEMCMP  memcmp
#define STRLEN  strlen
#define STRCPY  strcpy
#define STRNCPY strncpy
#define STRCMP  strcmp

#endif

#ifdef __CPU_AVR__

void *P_MEMCPY(void *dst, ROM_VOID_T src, const UI8_T len);

#endif

#endif /* __UTL_STRING_H__ */

