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

#include <inc/xmodem_port.h>
#include <h/xmodem.h>
#include <inc/drv_sio.h>


/* get char from SIO */
int xModem_GetChar(unsigned long ms)
{
    int     ch;

    while (ms-- > 0)
    {
        if ((ch = DRV_SIO_GetChar()) != -1)
        {
            return ch;
        }

        DRV_SIO_Delay(500);
    }

    return -1;
}

/* send char to SIO */
void xModem_SendChar(unsigned char ch)
{
	DRV_SIO_PutChar(ch);
}


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
GT_BOOL xModem_Upload(unsigned char *file_buf, int file_len)
{
    int		status;

	/* init SIO for xModem */
    DRV_SIO_Init();

    status = xmodem_upload(file_buf, file_len);

	return (status == CON_CST_XMODEM_UPLOAD_SUCCESS) ? GT_TRUE : GT_FALSE;
}

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
int xModem_Download(unsigned char *file_buf, int file_buf_len)
{
    int             status, download_file_len;
    unsigned long   block_no;

	/* init SIO for xModem */
    DRV_SIO_Init();

    status = xmodem_download(&block_no, file_buf, file_buf_len);

    if (status == CON_CST_XMODEM_DOWNLOAD_SUCCESS)
    {
        download_file_len = block_no * XMODEM_BLOCK_SIZE;
    }
    else
    {
        download_file_len = 0;
    }

    return download_file_len;
}

#endif

