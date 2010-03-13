/*
 * This file is define the macro of bit control.
 */

#ifndef __BIT_H
#define __BIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>

#define SET_BIT(port, i)    {(port) |= (1 << (i));}
#define CLR_BIT(port, i)    {(port) &= (~(1 << (i)));}

/* if this bit is set, return GT_TRUE,
 * else retunrn GT_FALSE */
#define TEST_BIT(port, i)                           \
            ((((port) & (1 << (i))) != 0) ? (GT_TRUE) : (GT_FALSE))

#ifdef __cplusplus
}
#endif

#endif /* __BIT_H */
