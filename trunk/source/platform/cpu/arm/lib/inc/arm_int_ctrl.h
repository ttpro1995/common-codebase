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

#ifndef __ARM_INT_CTRL_H
#define __ARM_INT_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif


/* ARM globally enable interrupt, support IRQ and FIQ */
#define ARM_INT_GLOBAL_ENABLE(bit)                        \
({                                                        \
    GT_UI32 val;                                          \
                                                          \
    if (((GT_UI32)bit & IRQ_BIT)                          \
         && ((GT_UI32)bit & FIQ_BIT))                     \
    {                                                     \
        __asm__ __volatile__ (                            \
            "mrs     %0, cpsr ;"                          \
            "bic     %0, %0, #((1<<6) | (1<<7)) ;"        \
            "msr     cpsr_c, %0"                          \
            : "=&r" (val)                                 \
        );                                                \
    }                                                     \
    else if ((GT_UI32)bit & IRQ_BIT)                      \
    {                                                     \
        __asm__ __volatile__ (                            \
            "mrs     %0, cpsr ;"                          \
            "bic     %0, %0, #(1<<7) ;"                   \
            "msr     cpsr_c, %0"                          \
            : "=&r" (val)                                 \
        );                                                \
    }                                                     \
    else if ((GT_UI32)bit & FIQ_BIT)                      \
    {                                                     \
        __asm__ __volatile__ (                            \
            "mrs     %0, cpsr ;"                          \
            "bic     %0, %0, #(1<<6) ;"                   \
            "msr     cpsr_c, %0"                          \
            : "=&r" (val)                                 \
        );                                                \
    }                                                     \
})


/* ARM globally disable interrupt, support IRQ and FIQ */
#define ARM_INT_GLOBAL_DISABLE(bit)                       \
({                                                        \
    GT_UI32 val;                                          \
                                                          \
    if (((GT_UI32)bit & IRQ_BIT)                          \
         && ((GT_UI32)bit & FIQ_BIT))                     \
    {                                                     \
        __asm__ __volatile__ (                            \
            "mrs     %0, cpsr ;"                          \
            "orr     %0, %0, #((1<<6) | (1<<7)) ;"        \
            "msr     cpsr_c, %0"                          \
            : "=&r" (val)                                 \
        );                                                \
    }                                                     \
    else if ((GT_UI32)bit & IRQ_BIT)                      \
    {                                                     \
        __asm__ __volatile__ (                            \
            "mrs     %0, cpsr ;"                          \
            "orr     %0, %0, #(1<<7) ;"                   \
            "msr     cpsr_c, %0"                          \
            : "=&r" (val)                                 \
        );                                                \
    }                                                     \
    else if ((GT_UI32)bit & FIQ_BIT)                      \
    {                                                     \
        __asm__ __volatile__ (                            \
            "mrs     %0, cpsr ;"                          \
            "orr     %0, %0, #(1<<6) ;"                   \
            "msr     cpsr_c, %0"                          \
            : "=&r" (val)                                 \
        );                                                \
    }                                                     \
})


#ifdef __cplusplus
}
#endif

#endif /* __ARM_INT_CTRL_H */

