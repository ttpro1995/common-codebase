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
#include <inc/cpu_io.h>


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
GT_UI8 CPU_IO_ReadByte(IN GT_UI8 *addr)
{
    if (addr == NULL)
    {
        return 0;
    }

    return (GT_UI8)(VP8(addr));
}


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
void CPU_IO_WriteByte(IN GT_UI8 *addr, IN GT_UI8 value)
{
    if (addr == NULL)
    {
        return;
    }

    (VP8(addr)) = value;
}


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
GT_UI16 CPU_IO_ReadHalfWord(IN GT_UI16 *addr)
{
    if (addr == NULL)
    {
        return 0;
    }

    return (GT_UI16)(VP16((GT_UI32)addr & (~0x1)));
}


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
void CPU_IO_WriteHalfWord(IN GT_UI16 *addr, IN GT_UI16 value)
{
    if (addr == NULL)
    {
        return;
    }

    (VP16((GT_UI32)addr & (~0x1))) = value;
}


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
GT_UI32 CPU_IO_ReadWord(IN GT_UI32 *addr)
{
    if (addr == NULL)
    {
        return 0;
    }

    return (GT_UI32)(VP32((GT_UI32)addr & (~0x3)));
}


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
void CPU_IO_WriteWord(IN GT_UI32 *addr, IN GT_UI32 value)
{
    if (addr == NULL)
    {
        return;
    }

    (VP32((GT_UI32)addr & (~0x3))) = value;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_PIO_WriteBit
 * DESCRIPTION:
 *      Write PIO bit
 * INPUT:
 *      pio_base_addr : PIO base address;
 *      pio_no        : =~0, all PIO bits; else, PIO bit number.
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
)
{
    if (pio_no == ~0)
    {
        (VP32(PIO_PER(pio_base_addr))) = ~0;    /* PIO control enable */
        (VP32(PIO_OER(pio_base_addr))) = ~0;    /* PIO output enable  */

        if (value == 0)
        {
            (VP32(PIO_CODR(pio_base_addr))) = ~0;
        }
        else
        {
            (VP32(PIO_SODR(pio_base_addr))) = ~0;
        }
    }
    else
    {
        (VP32(PIO_PER(pio_base_addr))) |= (1 << pio_no);    /* PIO control enable */
        (VP32(PIO_OER(pio_base_addr))) |= (1 << pio_no);    /* PIO output enable  */

        if (value == 0)
        {
            (VP32(PIO_CODR(pio_base_addr))) |= (1 << pio_no);
        }
        else
        {
            (VP32(PIO_SODR(pio_base_addr))) |= (1 << pio_no);
        }
    }

    return;
}


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
)
{
    GT_UI32 value;

    value = (VP32(PIO_PDSR(pio_base_addr)));

    if (pio_no == ~0)
    {
        return value;
    }
    else
    {
        return ((value & (1 << pio_no))? 1 : 0);
    }
}


