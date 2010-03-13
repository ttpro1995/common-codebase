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

#include <inc/drv_image.h>


/******************************************************************************
FUNCTION NAME:
    drv_image_CheckImageValid
DESCRIPTION:
    Check image is valid or not
INPUT:
    image_base : image base address
OUTPUT:
    NONE
RETURN:
    GT_TRUE  : Image is valid;
    GT_FALSE : Image is invalid.
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
static GT_BOOL drv_image_CheckImageValid(IN const GT_UI8 *image_base)
{
    GT_IMAGE_HEADER  *image_sig_header;
    GT_UI16          *addr;
    GT_UI32           len;
    GT_UI16           checksum;

    if (image_base == NULL)
    {
        return GT_FALSE;
    }

    image_sig_header = (GT_IMAGE_HEADER *)(image_base + IMAGE_SIG_HEADER_OFFSET);

    if (image_sig_header->product_code != IMAGE_PRODUCT_CODE)
    {
        return GT_FALSE;
    }

    if ((image_sig_header->signature != IMAGE_LOADER_SIGNATURE)
                && (image_sig_header->signature != IMAGE_KERNEL_SIGNATURE))
    {
        return GT_FALSE;
    }

    checksum = 0;
    len      = (image_sig_header->size + 1) & (~0x1);   /* align to 2 bytes */
    addr     = (GT_UI16 *)image_base;

    while (len > 0)
    {
        checksum += (GT_UI16)(VP16(addr));
        addr++;
        len -= 2;
    }

    if (checksum != 0)
    {
        return GT_FALSE;
    }

    return GT_TRUE;
}


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
DRV_IMAGE_TYPE DRV_Image_GetImageType(IN const GT_UI8 *image_base)
{
    GT_IMAGE_HEADER  *image_sig_header;

    if (image_base == NULL)
    {
        return DRV_IMAGE_UNKNOWN;
    }

    if (drv_image_CheckImageValid(image_base) != GT_TRUE)
    {
        /* image is not valid */
        return DRV_IMAGE_UNKNOWN;
    }

    image_sig_header = (GT_IMAGE_HEADER *)(image_base + IMAGE_SIG_HEADER_OFFSET);

    switch (image_sig_header->signature)
    {
        case IMAGE_LOADER_SIGNATURE:
            return DRV_IMAGE_LOADER;

        case IMAGE_KERNEL_SIGNATURE:
            return DRV_IMAGE_KERNEL;

        default:
            return DRV_IMAGE_UNKNOWN;
    }
}


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
GT_UI8 *DRV_IMAGE_GetImageVersion(IN const GT_UI8 *image_base)
{
    GT_IMAGE_HEADER  *image_sig_header;

    if (image_base == NULL)
    {
        return NULL;
    }

    if (drv_image_CheckImageValid(image_base) != GT_TRUE)
    {
        /* image is not valid */
        return NULL;
    }

    image_sig_header = (GT_IMAGE_HEADER *)(image_base + IMAGE_SIG_HEADER_OFFSET);

    return (GT_UI8 *)(image_sig_header->version);
}


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
GT_UI32 DRV_Image_GetImageSize(IN const GT_UI8 *image_base)
{
    GT_IMAGE_HEADER  *image_sig_header;

    if (image_base == NULL)
    {
        return 0;
    }

    if (drv_image_CheckImageValid(image_base) != GT_TRUE)
    {
        /* image is not valid */
        return 0;
    }

    image_sig_header = (GT_IMAGE_HEADER *)(image_base + IMAGE_SIG_HEADER_OFFSET);

    return image_sig_header->size;
}


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
DRV_IMAGE_COMPRESS_MODE DRV_Image_GetCompressMode(IN const GT_UI8 *image_base)
{
    GT_IMAGE_HEADER  *image_sig_header;

    if (image_base == NULL)
    {
        return 0;
    }

    if (drv_image_CheckImageValid(image_base) != GT_TRUE)
    {
        /* image is not valid */
        return 0;
    }

    image_sig_header = (GT_IMAGE_HEADER *)(image_base + IMAGE_SIG_HEADER_OFFSET);

    switch (image_sig_header->compress_method)
    {
        case 0x0:
            return DRV_IMAGE_COMPRESS_NONE;
        case 0x1:
            return DRV_IMAGE_COMPRESS_LZMA;
        default:
            return DRV_IMAGE_COMPRESS_UNKNOWN;
    }
}

