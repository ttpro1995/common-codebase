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

#include <h/basetype.h>
#include <inc/utl_tran.h>
#include <inc/utl_prt.h>


/******************************************************************************
 * FUNCTION NAME:
 *      cli_AlignString
 * DESCRIPTION:
 *      format and align string
 * INPUT:
 *      left_align  : is left align.
 *      zero_head   : is zero head.
 *      format_bits : format bits length.
 *      str_from    : string input.
 * OUTPUT:
 *      str_to      : formatted string out.
 * RETURN:
 *      return the formatted string length.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_UI32 utl_prt_AlignString
(
    IN const GT_BOOL   left_align,
    IN const GT_BOOL   zero_head,
    IN const GT_UI32   format_bits,
    IN const GT_UI8   *str_from,
    OUT      GT_UI8   *str_to
)
{
    GT_UI32   str_len;

    if ((str_from == NULL) || (str_to == NULL))
    {
        return 0;
    }

    str_len = strlen(str_from);

    if (format_bits <= str_len)
    {
        memcpy(str_to, str_from, str_len);
        return str_len;
    }

    if (left_align == GT_TRUE)
    {
        memcpy(str_to, str_from, str_len);
        str_to += str_len;

        /* append spaces after the string */
        str_len = format_bits - str_len;
        while (str_len-- > 0)
        {
            *str_to++ = ' ';
        }
    }
    else
    {
        str_len = format_bits - str_len;
        if (zero_head == GT_TRUE)
        {
            while (str_len-- > 0)
            {
                *str_to++ = '0';
            }
        }
        else
        {
            while (str_len-- > 0)
            {
                *str_to++ = ' ';
            }
        }

        memcpy(str_to, str_from, strlen(str_from));
    }

    return format_bits;
}


/******************************************************************************
 * FUNCTION NAME:
 *      UTL_PRT_vsPrintf
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      Support ramdom compound of :
 *       %d / %i / %u / %x / %X / %c / %s / %- / %. / %0 / %*
 *      Such as : %-.3d / %.*s and so on.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_I32 UTL_PRT_vsPrintf
(
    OUT      GT_UI8   *buffer,
    IN const GT_I8    *format,
    IN       va_list   arg_prt
)
{
    GT_I8        fch;            /* get format value */
    GT_UI8       format_bits;    /* store how many bits to format align  */
    GT_BOOL      left_align;     /* = GT_TRUE: left align */
    GT_BOOL      zero_head;      /* = GT_TRUE: format with 0 ahead; or not. */
    GT_UI32      length = 0;     /* record string's length */
    GT_UI8      *p_str;
    GT_UI32      str_len;
    GT_I32       value;
    GT_UI8       str_buf[15];

    if ((buffer == NULL) || (format == NULL))
    {
        return -1;
    }

    while (1)
    {
        while ((fch = *format++) != '%')
        {
            if (fch == '\0')
            {
                goto vsprintf_exit;
            }

            *buffer++ = fch;
            length++;
        }

        format_bits = 0;
        zero_head   = GT_FALSE;
        left_align  = GT_FALSE;

        fch = *format++;
        if (fch == '%')  /* only a '%' */
        {
            *buffer++ = fch;
            length++;
            continue;  /* go to next circle */
        }

        if (fch == '-')
        {
            left_align = GT_TRUE;
            fch = *format++;  /* go to next charactor */
        }

        if ((fch == '.') || (fch == '0')) /* format with 0 ahead */
        {
            zero_head = GT_TRUE;

            /* skip redundant '0', and go to next valid charactor */
            while ((fch = *format++) == '0')
            {}
        }

        if (fch == '*')  /* dynamic width */
        {
            format_bits = (GT_UI8)(va_arg(arg_prt, int));
            fch = *format++;  /* go to next charactor */
        }
        else if ((fch >= '0') && (fch <= '9'))
        {
            do
            {
                format_bits = format_bits * 10 + fch - '0';
            } while (fch = *format++, ((fch >= '0') && (fch <= '9')));
        }

        switch (fch)
        {
            case 'd':
            case 'i':
                memset(str_buf, 0x0, sizeof(str_buf));
                value   = (GT_I32)(va_arg(arg_prt, int));
                UTL_TRAN_I32ToAscii(value, 10, str_buf);
                str_len = utl_prt_AlignString(left_align, zero_head,
                                              format_bits, str_buf, buffer);
                buffer += str_len;
                length += str_len;
                break;

            case 'u':
            case 'x':
            case 'X':
                memset(str_buf, 0x0, sizeof(str_buf));
                value   = (GT_I32)(va_arg(arg_prt, int));
                UTL_TRAN_UI32ToAscii((GT_UI32)value,
                                     ((fch == 'u')? 10 : 16),
                                     str_buf);
                str_len = utl_prt_AlignString(left_align, zero_head,
                                              format_bits, str_buf, buffer);
                buffer += str_len;
                length += str_len;
                break;

            case 'c':
                memset(str_buf, 0x0, sizeof(str_buf));
                str_buf[0] = (GT_UI8)(va_arg(arg_prt, int));
                str_len = utl_prt_AlignString(left_align, zero_head,
                                              format_bits, str_buf, buffer);
                buffer += str_len;
                length += str_len;
                break;

            case 's':
                p_str = (GT_UI8 *)(va_arg(arg_prt, GT_UI8 *));
                if (p_str == NULL)
                {
                    goto vsprintf_exit;
                }
                str_len = utl_prt_AlignString(left_align, zero_head,
                                              format_bits, p_str, buffer);
                buffer += str_len;
                length += str_len;
                break;

            default:
                /* print empty charactors */
                while (format_bits-- > 0)
                {
                    *buffer++ = ' ';
                    length++;
                }
                break;
        }
    }

vsprintf_exit:

    *buffer = '\0';  /* end with '\0' */
    return length;
}


/******************************************************************************
 * FUNCTION NAME:
 *      UTL_PRT_sPrintf
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_I32 UTL_PRT_sPrintf
(
    OUT      GT_UI8 *buffer,
    IN const GT_I8  *fmt,
    ...
)
{
    va_list      ptr;
    GT_I32       length;

    if ((buffer == NULL) || (fmt == NULL))
    {
        return -1;
    }

    va_start(ptr, fmt);
    length = UTL_PRT_vsPrintf(buffer, fmt, ptr);
    va_end(ptr);

    return length;
}


/******************************************************************************
 * FUNCTION NAME:
 *      UTL_PRT_Printf
 * DESCRIPTION:
 *      Print string to output device.
 *      The sizeof string is limited by UTL_PRINT_MAX_BUFFER.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      Support ramdom compound of :
 *       %d / %i / %u / %x / %X / %c / %s / %- / %. / %0 / %*
 *      Such as : %-.3d / %.*s and so on.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_I32 UTL_PRT_Printf
(
    IN const GT_I8 *fmt,
    ...
)
{
    GT_UI8       temp_buffer[UTL_PRINTF_MAX_BUFFER + 1];
    va_list      ptr;
    GT_I32       length;

    if (fmt == NULL)
    {
        return -1;
    }

    va_start(ptr, fmt);
    length = UTL_PRT_vsPrintf(temp_buffer, fmt, ptr);
    va_end(ptr);

    UTL_VT_PutString(temp_buffer, length);

    return length;
}


/******************************************************************************
 * FUNCTION NAME:
 *      UTL_PRT_Display_Callback
 * DESCRIPTION:
 *      display callback function.
 * INPUT:
 *      percent : percent to display.
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      This API will be used when program flash, or other places.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
void UTL_PRT_Display_Callback(IN const GT_UI32 percent)
{
    UTL_PRT_Printf("\b\b\b\b\b%3u %%", percent);
}

