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

#ifndef __UTL_PRT_H
#define __UTL_PRT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>
#include <h/stdarg.h>
#include <inc/utl_vt.h>

/* UTL_PRT_Printf max printf buffer define */
#define UTL_PRINTF_MAX_BUFFER    KB(1)


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
);

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
) __attribute__ ((format(printf,2,3)));

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
) __attribute__ ((format(printf,1,2)));

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
void UTL_PRT_Display_Callback(IN const GT_UI32 percent);


#ifdef __cplusplus
}
#endif

#endif /* __UTL_PRT_H */

