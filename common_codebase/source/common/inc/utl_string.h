/******************************************************************************
 *
 * (C) Copyright 2007
 *    Panda Xiong, yaxi1984@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * History:
 *    2007.03.27    Panda Xiong       Create
 *
******************************************************************************/

#ifndef __UTL_STRING_H
#define __UTL_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>


/* instead of the standard string API */
#define atoi(str)					ATOI((const GT_UI8 *)(str))
#define memset(m, c, n)             MEMSET((void *)(m), (GT_UI8)(c), (GT_UI32)(n))
#define memcpy(dst, src, len)       MEMCPY((void *)(dst), (const void *)(src), (GT_UI32)(len))
#define memcmp(s1v, s2v, size)      MEMCMP((const void *)(s1v), (const void *)(s2v), (GT_UI32)(size))
#define strcpy(to, from)            STRCPY((GT_UI8 *)(to), (const GT_UI8 *)(from))
#define strcmp(s1, s2)              STRCMP((const GT_UI8 *)(s1), (const GT_UI8 *)(s2))
#define strncpy(to, from, count)    STRNCPY((GT_UI8 *)(to), (const GT_UI8 *)(from), (GT_UI32)(count))
#define strlen(str)                 STRLEN((const GT_UI8 *)(str))
#define strchr(str, c)              STRCHR((const GT_UI8 *)(str), (GT_UI8)(c))
#define strstr(str, str_2)          STRSTR((const GT_UI8 *)(str), (const GT_UI8 *)(str_2))


/******************************************************************************
FUNCTION NAME:
    ATOI
DESCRIPTION:
    Translate Ascii to int
INPUT:
    str   : string input
OUTPUT:
    NONE
RETURN:
    The result int 32
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_I32 ATOI(IN const GT_UI8 *str);

/******************************************************************************
FUNCTION NAME:
    MEMSET
DESCRIPTION:
    Set memory to a const value.
INPUT:
    c   : const value
    n   : memory length to be set
OUTPUT:
    m   : memory to be set
RETURN:
    Memory start address to be set
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void *MEMSET(OUT void *m, IN GT_UI8 c, IN GT_UI32 n);

/******************************************************************************
FUNCTION NAME:
    MEMCPY
DESCRIPTION:
    copy memory block
INPUT:
    src : source memory address
    len : memory copy length
OUTPUT:
    dst : destination memory start address
RETURN:
    destination memory start address
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void *MEMCPY(OUT void *dst, IN const void *src, IN GT_UI32 len);

/******************************************************************************
FUNCTION NAME:
    MEMCMP
DESCRIPTION:
    compare memory block
INPUT:
    s1v  : first  source memory address to be compared
    s2v  : second source memory address to be compared
    size : compare length
OUTPUT:
    NONE
RETURN:
    =0   : equal
    >0   : first memory block is larger than second memory block
    <0   : first memory block is smaller than second memory block
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_I8 MEMCMP(IN const void *s1v, IN const void *s2v, IN GT_UI32 size);

/******************************************************************************
FUNCTION NAME:
    STRLEN
DESCRIPTION:
    get string length
INPUT:
    string : string to be calculated
OUTPUT:
    NONE
RETURN:
    string length
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_UI32 STRLEN(IN const GT_UI8 *string);

/******************************************************************************
FUNCTION NAME:
    STRCPY
DESCRIPTION:
    copy string
INPUT:
    from : source string start address
OUTPUT:
    to   : destination string start address
RETURN:
    destination string start address
NOTES:
    the source string must be endded with '\0'
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_UI8 *STRCPY(OUT GT_UI8 *to, IN const GT_UI8 *from);

/******************************************************************************
FUNCTION NAME:
    STRNCPY
DESCRIPTION:
    copy a const length string
INPUT:
    from : source string start address
    count: copy string length
OUTPUT:
    to   : destination string start address
RETURN:
    destination string start address
NOTES:
    1. the source string must be endded with '\0'
    2. if the source string length is less than "count", will auto return.
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_UI8 *STRNCPY(OUT GT_UI8 *to, IN const GT_UI8 *from, IN GT_UI32 count);

/******************************************************************************
FUNCTION NAME:
    STRCMP
DESCRIPTION:
    compare string
INPUT:
    _s1  : first string start address
    _s2  : second string start address
OUTPUT:
    NONE
RETURN:
    =0   : equal
    >0   : first string is larger than second string
    <0   : first string is smaller than second string
NOTES:
    the first string and the second string must be endded with '\0'.
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_I8 STRCMP(IN const GT_UI8 *_s1, IN const GT_UI8 *_s2);

/******************************************************************************
FUNCTION NAME:
    STRCHR
DESCRIPTION:
    Find the first place, where the input charactor exists.
INPUT:
    s  : string start address, to search
    c  : charactor, search key to find
OUTPUT:
    NONE
RETURN:
    The first place in string, where the input charactor exists.
    If not found, return NULL.
NOTES:
    All input string must be endded with '\0'.
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_UI8 *STRCHR(IN const GT_UI8 *s, IN GT_UI8 c);

/******************************************************************************
FUNCTION NAME:
    STRSTR
DESCRIPTION:
    Find the first place, where the input string exists.
INPUT:
    haystack  : string start address, to search
    needle    : string, search key to find
OUTPUT:
    NONE
RETURN:
    The first place in string, where the input string exists.
    If not found, return NULL.
NOTES:
    All input string must be endded with '\0'.
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_UI8 *STRSTR(IN const GT_UI8 *haystack, IN const GT_UI8 *needle);

#ifdef __cplusplus
}
#endif

#endif /* __UTL_STRING_H */

