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

#ifndef __IMAGE_DEFINE_H
#define __IMAGE_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>

typedef struct
{
    /* image signature: bootloader, kernel image, or others */
    GT_UI32      signature;

    /* product code: to distinguish this product with others */
    GT_UI32      product_code;

    /* image size, or image compress size if compressed,
     * including this image header */
    GT_UI32      size;

    /* compress method: =1, LZMA; or others */
    GT_UI16      compress_method;

    /* checksum of image, including this image header */
    GT_UI16      checksum;

    /* image version, must end with character '\0' */
    GT_UI8       version[16];

    /* image description, must end with character '\0' */
    GT_UI8       description[32];

} GT_IMAGE_HEADER;

/* these values must be the same with the corresponding value in image_cfg.h */
#define IMAGE_LOADER_SIGNATURE      0x00000001
#define IMAGE_KERNEL_SIGNATURE      0x00000002


/*
 * Image store define in flash:
 *  ----------------
 * |   BootLoader   |  KB(128)
 * |----------------|
 * |                |
 * |     Kernel     |  (Other spaces)
 * |                |
 *  ----------------
 */
#define IMAGE_LOADER_BASE           (FLASH_BASE_ADDR)
#define IMAGE_LOADER_SIZE           KB(128)

#define IMAGE_KERNEL_BASE           (IMAGE_LOADER_BASE + IMAGE_LOADER_SIZE)
#define IMAGE_KERNEL_SIZE           (FLASH_SIZE - IMAGE_LOADER_SIZE)


/* all image will download to this area */
#define IMAGE_DOWNLOAD_BASE         (SDRAM_BASE_ADDR)
#define IMAGE_DOWNLOAD_MAX_SIZE     (IMAGE_KERNEL_SIZE)


/* text base address */
#define LOADER_TEXT_BASE            (__LOADER_TEXT_BASE__)
#define KERNEL_TEXT_BASE            (__KERNEL_TEXT_BASE__)


#ifdef __cplusplus
}
#endif

#endif /* __IMAGE_DEFINE_H */

