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

#ifndef __UTL_LZMA_H
#define __UTL_LZMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>

typedef GT_UI32     LZMA_UI32_T;
typedef GT_UI16     LZMA_UI16_T;
typedef GT_UI8      LZMA_UI8_T;

/******************************************************************************
 * FUNCTION NAME:
 *      UTL_LZMA_GetDecodeBufSize
 * DESCRIPTION:
 *      Get LZMA decode buffer size
 * INPUT:
 *      flash_base : kernel image base address in flash, exclude the header.
 * OUTPUT:
 *      None
 * RETURN:
 *      >0  : Buffer Size
 *      =0  : Fail
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00   2007.02.14    Panda Xiong     Porting from 7-zip's LZMA SDK.
******************************************************************************/
LZMA_UI32_T UTL_LZMA_GetDecodeBufSize(LZMA_UI8_T *flash_base);

/******************************************************************************
 * FUNCTION NAME:
 *      UTL_LZMA_Decompress
 * DESCRIPTION:
 *      Decompress kernel image use LZMA for bootloader.
 * INPUT:
 *      flash_base        : kernel image base address in flash, exclude the header.
 *      img_size          : kernel compressed image size.
 *      ram_base          : kernel base address in RAM.
 *      limit_size        : MAX kernel image size.
 *      decode_buf_base   : LZMA decode buffer base address.
 *      callback          : progress callback.
 *                          if NULL do nothing, else call it by decompress.
 *                          return 0: cancel; 1:continue.
 * OUTPUT:
 *      None
 * RETURN:
 *      >0   : Decompressed Size
 *      =0   : Fail
 *      =-1  : User Cancel (by progressCallback return 0)
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00   2007.02.14    Panda Xiong     Porting from 7-zip's LZMA SDK.
******************************************************************************/
int UTL_LZMA_Decompress
(
    LZMA_UI8_T   *flash_base,
    LZMA_UI32_T  img_size,
    LZMA_UI8_T   *ram_base,
    LZMA_UI32_T  limit_size,
    LZMA_UI8_T   *decode_buf_base,
    int          (*callback)(LZMA_UI32_T total_size, LZMA_UI32_T current_size)
);

#ifdef __cplusplus
}
#endif

#endif /* __UTL_LZMA_H */

