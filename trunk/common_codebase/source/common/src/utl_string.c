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

static GT_I32 hextoi(IN const GT_UI8 *str)
{
	GT_I32 		i, len;
	GT_UI8		ch;
	GT_UI32 	val = 0;

	len = strlen(str);
	for (i=0; i<len; i++)
	{
		val <<= 4;

		ch = str[i];
		if ((ch >='0') && (ch <='9'))
		{
			val |= (ch - '0');
		}
		else
		{
			if ((ch >='a') && (ch <='f'))
			{
				val |= (ch - 'a' + 10);
			}
			else if ((ch >='A') && (ch <='F'))
			{
				val |= ((ch - 'A') + 10);
			}
			else
			{
				break;
			}
		}
	}

	return val;
}

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
GT_I32 ATOI(IN const GT_UI8 *str)
{
    GT_I32 n = 0;

    if (str == NULL)
    {
        return 0;
    }

    if ((str[0]=='0')
            && ((str[1]=='x') || (str[1]=='X')))
    {
        return hextoi(str + 2);
    }
    else
    {
        while ((*str >= '0') && (*str <= '9'))
        {
            n = (n * 10) + *str - '0';
            str++;
        }

        return n;
    }
}


/******************************************************************************
FUNCTION NAME:
    MEMSET
DESCRIPTION:
    Set memory to a const value.
INPUT:
    c   : const value
    n   : memory length to be set
OUTPUT:
    m   : memory start address to be set
RETURN:
    Memory start address to be set
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void *MEMSET(OUT void *m, IN GT_UI8 c, IN GT_UI32 n)
{
    const GT_UI8 *end;
    GT_UI8   *p;

    if (m == NULL)
    {
        return m;
    }

    end = (GT_UI8 *)m + n;
    p   = (GT_UI8 *)m;

    for (; p < end; p++)
    {
        *p = (GT_UI8)c;
    }

    return m;
}


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
void *MEMCPY(OUT void *dst, IN const void *src, IN GT_UI32 len)
{
    const GT_UI8 *end;
    GT_UI8       *to;
    GT_UI8       *from;

    if ((dst == NULL) || (src == NULL))
    {
        return dst;
    }

    end  = (GT_UI8 *)dst + len;
    to   = (GT_UI8 *)dst;
    from = (GT_UI8 *)src;

    for (; to < end; to++, from++)
    {
        *to = *from;
    }

    return dst;
}


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
GT_I8 MEMCMP(IN const void *s1v, IN const void *s2v, IN GT_UI32 size)
{
    GT_UI8 *s1 = (GT_UI8 *)s1v;
    GT_UI8 *s2 = (GT_UI8 *)s2v;
    GT_I8   a, b;

    if ((s1v == NULL) && (s2v == NULL))
    {
        return 0;
    }
    else if ((s1v != NULL) && (s2v == NULL))
    {
        return 1;
    }
    else if ((s1v == NULL) && (s2v != NULL))
    {
        return -1;
    }
    else
    {
        while (size-- != 0)
        {
            if ((a = *s1++) != (b = *s2++))
            {
                return (a - b);
            }
        }

        return 0;
    }
}


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
GT_UI32 STRLEN(IN const GT_UI8 *string)
{
    const GT_UI8 *ret = string;

    if (string == NULL)
    {
        return 0;
    }

    while (*string++ != '\0')
    {}

    return (GT_UI32)((string - 1) - ret);
}


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
GT_UI8 *STRCPY(OUT GT_UI8 *to, IN const GT_UI8 *from)
{
    GT_UI8 *ret = to;

    if ((to == NULL) || (from == NULL))
    {
        return ret;
    }

    while ((*to++ = *from++) != '\0')
    {}

    return ret;
}


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
GT_UI8 *STRNCPY(OUT GT_UI8 *to, IN const GT_UI8 *from, IN GT_UI32 count)
{
    GT_UI8 *ret = to;

    if ((to == NULL) || (from == NULL))
    {
        return ret;
    }

    while (count > 0)
    {
        count--;
        if ((*to++ = *from++) == '\0')
        {
            break;
        }
    }

    if (count == 0)
    {
        *to = '\0';
    }
    else
    {
        while (count-- > 0)
        {
            *to++ = '\0';
        }
    }

    return ret;
}


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
GT_I8 STRCMP(IN const GT_UI8 *_s1, IN const GT_UI8 *_s2)
{
    GT_UI8 *s1 = (GT_UI8 *)_s1;
    GT_UI8 *s2 = (GT_UI8 *)_s2;
    GT_I8   a, b;

    if ((s1 == NULL) && (s2 == NULL))
    {
        return 0;
    }
    else if ((s1 != NULL) && (s2 == NULL))
    {
        return 1;
    }
    else if ((s1 == NULL) && (s2 != NULL))
    {
        return -1;
    }
    else
    {
        while ((a = *s1++), (b = *s2++), a && b)
        {
            if (a != b)
            {
                return (a - b);
            }
        }

        return (a - b);
    }
}

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
GT_UI8 *STRCHR(IN const GT_UI8 *s, IN GT_UI8 c)
{
	while (1)
	{
		if (*s == c)
		{
			return (GT_UI8 *)s;
		}
		else if (*s == '\0')
		{
			return NULL;
		}
        else
        {
    		s++;
        }
	}
}


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
GT_UI8 *STRSTR(IN const GT_UI8 *haystack, IN const GT_UI8 *needle)
{
	GT_UI32   hlen = STRLEN(haystack);
	GT_UI32   nlen = STRLEN(needle);

	while (hlen >= nlen)
	{
		if (!MEMCMP(haystack, needle, nlen))
		{
			return (GT_UI8 *)haystack;
		}

		haystack++;
		hlen--;
	}

	return NULL;
}

