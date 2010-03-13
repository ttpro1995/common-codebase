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

#ifndef __XMODEM_PORT_H
#define __XMODEM_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>
#include <inc/utl_vt.h>
#include <inc/delay.h>


#define XMODEM_UPLOAD_ENABLE				1
#define XMODEM_DOWNLOAD_ENABLE				1

#if (!XMODEM_UPLOAD_ENABLE && !XMODEM_DOWNLOAD_ENABLE)
  #error "XMODEM Upload and Download should not be disabled together!"
#endif


#if XMODEM_UPLOAD_ENABLE

/*******************************************************************************
 * FUNCTION NAME:
 *      xModem_Upload
 * DESCRIPTION:
 *      Upload file use X modem protocol.
 * INPUT:
 *      file_buf     :  buffer base address to upload file.
 *      file_len     :  upload file length.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE   :  Upload Success.
 *      GT_FALSE  :  Upload Fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
GT_BOOL xModem_Upload(unsigned char *file_buf, int file_len);

#endif


#if XMODEM_DOWNLOAD_ENABLE

/*******************************************************************************
 * FUNCTION NAME:
 *      xModem_Download
 * DESCRIPTION:
 *      Download file use X modem protocol.
 * INPUT:
 *      file_buf     :  buffer base address to save file.
 *      file_buf_len :  max buffer len.
 * OUTPUT:
 *      None
 * RETURN:
 *      =0   :  Downlod fail.
 *      else :  Actual file size.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
int xModem_Download(unsigned char *file_buf, int file_buf_len);

#endif


#ifdef __cplusplus
}
#endif

#endif /* __XMODEM_PORT_H */

