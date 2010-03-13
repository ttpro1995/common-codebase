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

#include <inc/utl_tran.h>


#define ISXDIGIT(x) ((((x)>='0') && ((x)<='9'))         \
                        || (((x)>='a') && ((x)<='f'))   \
                        || (((x)>='A') && ((x)<='F')))
#define TOUPPER(x)  ((((x)>='a') && ((x)<='z')) ? ((char)(((x)-'a')+'A')) : (x))


/* local vars */
static const GT_UI8 TRAN_Htob[16] =
{ '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


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
    OUT GT_UI8  *obuf)
{
    GT_UI32      div;
    GT_UI8       j;                  /* index of the mappig table */
    GT_UI32      max_unsigned_int;
    GT_UI32      max;
    GT_I32       tempValue;          /* temp value */
    GT_UI8      *tempString;

    /* clear output string */
    obuf[0] = 0;

    /* just support 1-16 */
    if ((radix > 16) || (radix < 1))
    {
        return 0;
    }

    tempValue  = val;
    tempString = obuf;

    max_unsigned_int  = (GT_UI32)(-1);
    max_unsigned_int /= (GT_UI32)radix;
    max = (GT_UI32)max_unsigned_int;

    if (val < 0)
    {
        /* '-' sign */
        *tempString++ = '-';
        tempValue = val * (-1);
    }

    div = radix;

    while (div <= max)
    {
        div *= radix;
    }

    while (div)
    {
        j = (GT_UI8)((GT_UI32)tempValue / div);

        if ( j > 0)
        {
            /* 48 = ASCII 0 */
            *tempString++ = TRAN_Htob[j];
            tempValue    -= ((int)j * (int)div);
        }
        else
        {
            if (tempString != obuf)
            {
                *tempString++ = TRAN_Htob[0];
            }
            else
            {
                if (div == 1)
                {
                    *tempString++ = TRAN_Htob[0];
                }
            }
        }

        div /= radix;
    }

    *tempString++ = '\0';

    return (GT_UI32)((tempString - obuf) - 1);
}


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
    OUT GT_UI8   *str)
{
    GT_UI8       j;              /* index of the mappig table */
    GT_UI32      tempValue;      /* temp value */
    GT_UI8      *tempString;
    GT_UI32      div;
    GT_UI32      max;

    /* just support 1-16 */
    if ((radix > 16) || (radix < 1))
    {
        return 0;
    }

    max  = (GT_UI32)(~0);
    max /= (GT_UI32)radix;
    tempValue  = (GT_UI32)val;
    tempString = str;

    div = (GT_UI32)radix;
    while (div <= max)
    {
        div *= (GT_UI32)radix;
    }

    while (div)
    {
        j = (GT_UI8)(tempValue / div);

        if (j)
        {
            *tempString++ = TRAN_Htob[j];  /* 48 = ASCII 0 */
            tempValue    -= ((GT_UI32)j * div);
        }
        else
        {
            if (tempString != str)
            {
                *tempString++ = TRAN_Htob[j];
            }
            else
            {
                if (div == 1)
                {
                    *tempString++ = TRAN_Htob[j];
                }
            }
        }

        div /= (GT_UI32)radix;
    }

    *tempString++ = '\0';

    return (GT_UI32)((tempString - str) - 1);
}


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
GT_I32 UTL_TRAN_AsciiToI32(IN const GT_I8 *str, IN const GT_UI32 radix)
{
    GT_I8    ch;
    GT_I32   value = 0;

    switch (radix)
    {
        case 16:
            while (ch = *str++, ISXDIGIT(ch))
            {
                /* convert char symbol to upper case */
                ch = TOUPPER(ch);
                ch -= '0';

                /* 'A' ~ 'F' */
                if (ch > 9)
                {
                    /* convert the hex symbol to hex number */
                    ch -= ('A' - '9' - 1);
                }

                value = 16*value + (GT_UI32)ch;
            }
            break;

        case 10:
        default:
            while (1)
            {
                ch = *str++;
                if ((ch >= '0') && (ch <= '9'))
                {
                    value = value*10 + ch - '0';
                }
                else
                {
                    break;
                }
            }
            break;
    }

    return value;
}

