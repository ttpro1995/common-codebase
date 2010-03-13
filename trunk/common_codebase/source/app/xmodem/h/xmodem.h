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

#ifndef __XMODEM_H
#define __XMODEM_H

#ifdef __cplusplus
extern "C" {
#endif

#define CON_CST_XMODEM_DOWNLOAD_SUCCESS 	1
#define CON_CST_XMODEM_UPLOAD_SUCCESS		CON_CST_XMODEM_DOWNLOAD_SUCCESS
#define CON_CST_USER_ABORT					2
#define CON_CST_TIME_OUT					3
#define CON_CST_BAD_FILE					4

/* xmodem bock size, don't try to modify it.
 * and, the xmodem-1024 is not tested yet.
 */
#define XMODEM_BLOCK_SIZE   		    128

/* xmodem max retry times, is about 20s.
 * every 1s will send NCK command, if data is not received.
 */
#define XMODEM_MAX_RETRY_NO		        20

int xModem_GetChar(unsigned long ms);
void xModem_SendChar(unsigned char ch);

#if XMODEM_UPLOAD_ENABLE

/*******************************************************************************
 * FUNCTION NAME:
 *      xmodem_upload
 * DESCRIPTION:
 *      Upload file use X modem protocol.
 * INPUT:
 *      curdata     :  buffer base address to upload file.
 *      retlen      :  upload file length.
 * OUTPUT:
 *      None
 * RETURN:
 *      CON_CST_XMODEM_UPLOAD_SUCCESS  :  Upload Success
 *	    else                           :  Upload Fail
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
int xmodem_upload
(
	unsigned char *curdata,
	unsigned long  retlen
);

#endif

#if XMODEM_DOWNLOAD_ENABLE

/*******************************************************************************
 * FUNCTION NAME:
 *      xmodem_download
 * DESCRIPTION:
 *      Download file use X modem protocol.
 * INPUT:
 *      buf     :  buffer base address to save file.
 *      buf_len :  max buffer len.
 * OUTPUT:
 *      blkno   :  the total blkno that received on XMODEM.
 * RETURN:
 *      CON_CST_XMODEM_DOWNLOAD_SUCCESS :  Downlod success.
 *      else                            :  Downlod fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
int xmodem_download
(
	unsigned long       *blkno,
	unsigned char       *buf,
	unsigned long        buf_len
);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __XMODEM_H */

