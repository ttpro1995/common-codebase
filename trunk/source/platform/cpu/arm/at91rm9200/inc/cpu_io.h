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

#ifndef __CPU_IO_H
#define __CPU_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>
#include <inc/reg.h>

/* debug led define, PC3 */
#define CPU_DEBUG_LED_BIT           (3)
#define CPU_DEBUG_LED_BASE_ADDR     (PIOC_BASE_REG)
#define CPU_IO_WRITE_DEBUG_LED(value)     \
    CPU_PIO_WriteBit(CPU_DEBUG_LED_BASE_ADDR, CPU_DEBUG_LED_BIT, value)
#define CPU_IO_READ_DEBUG_LED()           \
    CPU_PIO_ReadBit(CPU_DEBUG_LED_BASE_ADDR, CPU_DEBUG_LED_BIT)


/******************************************************************************
 * FUNCTION NAME:
 *      None
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
GT_UI8 CPU_IO_ReadByte(IN GT_UI8 *addr);

/******************************************************************************
 * FUNCTION NAME:
 *      None
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
void CPU_IO_WriteByte(IN GT_UI8 *addr, IN GT_UI8 value);

/******************************************************************************
 * FUNCTION NAME:
 *      None
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
GT_UI16 CPU_IO_ReadHalfWord(IN GT_UI16 *addr);

/******************************************************************************
 * FUNCTION NAME:
 *      None
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
void CPU_IO_WriteHalfWord(IN GT_UI16 *addr, IN GT_UI16 value);

/******************************************************************************
 * FUNCTION NAME:
 *      None
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
GT_UI32 CPU_IO_ReadWord(IN GT_UI32 *addr);

/******************************************************************************
 * FUNCTION NAME:
 *      None
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
void CPU_IO_WriteWord(IN GT_UI32 *addr, IN GT_UI32 value);

/******************************************************************************
 * FUNCTION NAME:
 *      CPU_PIO_WriteBit
 * DESCRIPTION:
 *      Write PIO bit
 * INPUT:
 *      pio_base_addr : PIO base address;
 *      pio_no        : PIO bit number;
 *      value         : =0, clear bit; else, set bit.
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
void CPU_PIO_WriteBit
(
    IN const GT_UI32 pio_base_addr,
    IN const GT_UI32 pio_no,
    IN const GT_UI32 value
);

/******************************************************************************
 * FUNCTION NAME:
 *      CPU_PIO_ReadBit
 * DESCRIPTION:
 *      Read PIO bit
 * INPUT:
 *      pio_base_addr : PIO base address;
 *      pio_no        : =~0, all PIO bits; else, PIO bit number.
 * OUTPUT:
 *      None
 * RETURN:
 *      if pio_no = ~0, return all bit value;
 *      else, return 0/1.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_UI32 CPU_PIO_ReadBit
(
    IN const GT_UI32 pio_base_addr,
    IN const GT_UI32 pio_no
);

#ifdef __cplusplus
}
#endif


#endif  /* __CPU_IO_H */

