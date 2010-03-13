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
 * Description:
 *    This config file is used to signature image.
 *    1. in this config file, all the comments must strictly follow
 *       ANSI C, else, the image signaturing will fail.
 *    2. in this config file, all the macro must only contain ONE line,
 *       else, the image signaturing will fail.
 *
 * History:
 *    2007.03.27    Panda Xiong       Create
 *
******************************************************************************/

#ifndef __IMAGE_CFG_H
#define __IMAGE_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* image signature */
#define IMAGE_SIGNATURE                 0x00000001

/* image product code */
#define IMAGE_PRODUCT_CODE              0x00000001

/* image compress method */
#define IMAGE_COMPRESS_METHOD           0x00000000

/* image version */
#define IMAGE_VERSION                   "v1.00.001"

/* image description */
#define IMAGE_DESCRIPTION               "Panda Xiong Create"

/* image signature header offset */
#define IMAGE_SIG_HEADER_OFFSET         32


#ifdef __cplusplus
}
#endif

#endif /* __IMAGE_CFG_H */

