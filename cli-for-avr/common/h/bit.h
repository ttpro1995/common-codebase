/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    This file is define the macro of bit control
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#ifndef __BIT_H__
#define __BIT_H__

#include <basetype.h>

#define SET_BIT(port, i)    {(port) |= (1 << (i));}
#define CLR_BIT(port, i)    {(port) &= (~(1 << (i)));}

/* if this bit is set, return TRUE,
 * else retunrn FALSE */
#define TEST_BIT(port, i)                           \
            ((((port) & (1 << (i))) != 0) ? (TRUE) : (FALSE))

#endif /* __BIT_H__ */

