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
#include <inc/cpu_int.h>
#include <inc/utl_prt.h>


/******************************************************************************
FUNCTION NAME:
    vector_undef
DESCRIPTION:
    process undefined exception
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void vector_undef(void)
{
    UTL_PRT_Printf("\n\r--Undef exception--");
    while (1)
    {}
}


/******************************************************************************
FUNCTION NAME:
    vector_swi
DESCRIPTION:
    process swi exception
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void vector_swi(void)
{
    UTL_PRT_Printf("\n\r--Swi exception--");
    while (1)
    {}
}


/******************************************************************************
FUNCTION NAME:
    vector_pabt
DESCRIPTION:
    process prefetch abort exception
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void vector_pabt(void)
{
    UTL_PRT_Printf("\n\r--Prefetch abort exception--");
    while (1)
    {}
}


/******************************************************************************
FUNCTION NAME:
    vector_dabt
DESCRIPTION:
    process data abort exception
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void vector_dabt(void)
{
    UTL_PRT_Printf("\n\r--Data abort exception--");
    while (1)
    {}
}


/******************************************************************************
FUNCTION NAME:
    vector_irq
DESCRIPTION:
    process irq exception
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void vector_irq(void)
{
    UTL_PRT_Printf("\n\r--IRQ exception--");
    while (1)
    {}
}


/******************************************************************************
FUNCTION NAME:
    vector_fiq
DESCRIPTION:
    process fiq exception
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void vector_fiq(void)
{
    UTL_PRT_Printf("\n\r--FIQ exception--");
    while (1)
    {}
}

