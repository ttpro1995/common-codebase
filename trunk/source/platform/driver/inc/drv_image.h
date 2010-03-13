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

#ifndef __DRV_IMAGE_H
#define __DRV_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>
#include <image_define.h>
#include <inc/image_cfg.h>


typedef enum
{
    DRV_IMAGE_UNKNOWN = 0,
    DRV_IMAGE_LOADER,
    DRV_IMAGE_KERNEL
} DRV_IMAGE_TYPE;

typedef enum
{
    DRV_IMAGE_COMPRESS_UNKNOWN = 0,
    DRV_IMAGE_COMPRESS_NONE,
    DRV_IMAGE_COMPRESS_LZMA
} DRV_IMAGE_COMPRESS_MODE;

/******************************************************************************
FUNCTION NAME:
    DRV_Image_GetImageType
DESCRIPTION:
    Get image type.
INPUT:
    image_base : image base address
OUTPUT:
    NONE
RETURN:
    DRV_IMAGE_UNKNOWN / DRV_IMAGE_LOADER / DRV_IMAGE_KERNEL
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
DRV_IMAGE_TYPE DRV_Image_GetImageType(IN const GT_UI8 *image_base);

/******************************************************************************
FUNCTION NAME:
    DRV_IMAGE_GetImageVersion
DESCRIPTION:
    Get image version base address.
INPUT:
    image_base : image base address
OUTPUT:
    NONE
RETURN:
    image version base address.
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_UI8 *DRV_IMAGE_GetImageVersion(IN const GT_UI8 *image_base);

/******************************************************************************
FUNCTION NAME:
    DRV_Image_GetImageSize
DESCRIPTION:
    Get image size.
INPUT:
    image_base : image base address
OUTPUT:
    NONE
RETURN:
    =0   : invalid image.
    else : image size.

NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_UI32 DRV_Image_GetImageSize(IN const GT_UI8 *image_base);

/******************************************************************************
FUNCTION NAME:
    DRV_Image_GetCompressMode
DESCRIPTION:
    Get image compress mode.
INPUT:
    image_base : image base address
OUTPUT:
    NONE
RETURN:
    Compress mode
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
DRV_IMAGE_COMPRESS_MODE DRV_Image_GetCompressMode(IN const GT_UI8 *image_base);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_IMAGE_H */

