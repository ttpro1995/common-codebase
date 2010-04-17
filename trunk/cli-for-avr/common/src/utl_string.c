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

#include <basetype.h>
#include <utl_string.h>

#if 0
I8_T ATOI8(const UI8_T *str)
{
	I8_T n = 0;

	while ((*str >= '0') && (*str <= '9'))
	{
		n = (n * 10) + *str - '0';
		str++;
	}

	return n;
}


void *MEMSET(void *m, const I8_T c, const UI8_T n)
{
    const UI8_T *end = (UI8_T *)m + n;
    UI8_T   *p  = (UI8_T *)m;

    for (; p < end; p++)
    {
        *p = (UI8_T)c;
    }

    return m;
}


void *MEMCPY(void *dst, const void *src, const UI8_T len)
{
    const UI8_T *end  = (UI8_T *)dst + len;
    UI8_T       *to   = (UI8_T *)dst;
    UI8_T       *from = (UI8_T *)src;

    for (; to < end; to++, from++)
    {
        *to = *from;
    }

    return dst;
}


I8_T MEMCMP(const void *s1v, const void *s2v, UI8_T size)
{
    UI8_T *s1 = (UI8_T *)s1v;
    UI8_T *s2 = (UI8_T *)s2v;
    I8_T   a, b;

    while (size-- != 0)
    {
        if ((a = *s1++) != (b = *s2++))
        {
            return (a - b);
        }
    }

    return 0;
}


UI8_T STRLEN(const UI8_T *string)
{
    const UI8_T *ret = string;

	while (*string++ != '\0')
	{}

	return (UI8_T)((string - 1) - ret);
}


UI8_T *STRCPY(UI8_T *to, const UI8_T *from)
{
    UI8_T *ret = to;

	while ((*to++ = *from++) != '\0')
	{}

	return ret;
}


UI8_T *STRNCPY(UI8_T *to, const UI8_T *from, UI8_T count)
{
    UI8_T *ret = to;

	while (count > 0)
	{
		count--;
		if ((*to++ = *from++) == '\0')
		{
			break;
		}
	}

	while (count > 0)
	{
		count--;
		*to++ = '\0';
	}

	return ret;
}


I8_T STRCMP(const UI8_T *_s1, const UI8_T *_s2)
{
    UI8_T *s1 = (UI8_T *)_s1;
	UI8_T *s2 = (UI8_T *)_s2;
	I8_T   a, b;

	while ((a = *s1++), (b = *s2++), a && b)
	{
		if (a != b)
		{
			return (a - b);
		}
	}

	return (a - b);
}
#endif

#ifdef __CPU_AVR__

void *P_MEMCPY(void *dst, ROM_VOID_T src, const UI8_T len)
{
    UI8_T  loop = 0;
    UI8_T *to = (UI8_T *)dst;

    while (loop != len)
    {
        *to = ROM_READ_BYTE((const ROM_UI8_T *)src + loop);
        to++;
        loop++;
    }

    return dst;
}

#endif

