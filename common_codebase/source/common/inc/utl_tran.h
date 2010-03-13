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

#ifndef __UTL_TRAN_H
#define __UTL_TRAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>


/******************************************************************************
Function:
    UTL_TRAN_I32ToAscii
Description:
    This function converts the digits of the given value argument to a
    null-terminated character string and sotre the result(up to 10 bytes)
    in string. The radix argument specifies the base of value; it must be 10
    or 16. if radix equal and value is negative, the first character of the
    stored string is the minus sign(-).
Input:
    val = decimal value
    radix = radix 16 or 10
Output:
    obuf = return string
Return:
    the length of out string
Note:

******************************************************************************/
GT_UI32 UTL_TRAN_I32ToAscii(
    IN  GT_I32   val,
    IN  GT_UI32  radix,
    OUT GT_UI8  *obuf);


/******************************************************************************
Function:
    UTL_TRAN_UI32ToAscii
Description:
    Translate unsigned long Data type into ASCII Characters
Input:
    val = decimal value
    radix = radix 16 or 10.
Output:
    str = return string.
Return:

Note:

******************************************************************************/
GT_UI32 UTL_TRAN_UI32ToAscii(
    IN  GT_UI32   val,
    IN  int      radix,
    OUT GT_UI8   *str);

/******************************************************************************
Function:
    UTL_TRAN_AsciiToI32
Description:
    Translate ASCII Character data type into Integer; radix is 10 or 16
Input:
    str = Input String.
    radix = 16 or 10. other treat like 10
Output:

Return:
    The Integer value
Note:

******************************************************************************/
GT_I32 UTL_TRAN_AsciiToI32(IN const GT_I8 *str, IN const GT_UI32 radix);

#ifdef __cplusplus
}
#endif

#endif /* __UTL_TRAN_H */

