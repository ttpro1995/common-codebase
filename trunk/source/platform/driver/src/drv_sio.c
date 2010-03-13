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

#include <inc/drv_sio.h>

typedef struct
{
	unsigned char	buffer[SIO_BUFFER_SIZE];	/* data buffer             */
	int				count;						/* current count in buffer */
	int				write;						/* write pointer           */
	int				read;						/* read pointer            */
} SIO_BUFFER_T;

static SIO_BUFFER_T	sioRxBuffer;


static void	sio_bufferInit(SIO_BUFFER_T *buf)
{
	buf->read  = 0;
	buf->write = 0;
	buf->count = 0;
}

static int sio_bufferWrite(SIO_BUFFER_T *p, unsigned char data)
{
	if (p->count == SIO_BUFFER_SIZE)
	{
		return GT_FALSE;	/* ring buffer full state */
	}

	p->buffer[p->write] = data;
	p->write++;
	p->count++;

	if (p->write == SIO_BUFFER_SIZE)
	{
		p->write = 0;
	}

	return GT_TRUE;
}

static int sio_bufferRead(SIO_BUFFER_T *p)
{
	int		data;

	if (p->count == 0)
	{
		return -1;	/* ring buffer empty state */
	}

	data = (int)(p->buffer[p->read]);

	p->read++;
	p->count--;

	if (p->read == SIO_BUFFER_SIZE)
	{
		p->read = 0;
	}

	return data;
}


/* get char from SIO */
int DRV_SIO_GetChar(void)
{
	int 	rxChar;

	/* first, check that there is any data in Rx buffer,
	 * if does, read and save it into sio buffer.
	 */
	while ((rxChar = UTL_VT_GetChar(1)) != -1)
	{
		sio_bufferWrite(&sioRxBuffer, (unsigned char)rxChar);
	}

	/* second, read data from Rx buffer */
	return sio_bufferRead(&sioRxBuffer);
}


/* send char from SIO.
 * if there is some input data, save it into buffer,
 * to make sure the SIO input data will not loss.
 */
void DRV_SIO_PutChar(unsigned char c)
{
	int		rxChar;

	/* first, check that there is any data in Rx buffer,
	 * if does, read and save it into sio buffer.
	 */
	while ((rxChar = UTL_VT_GetChar(1)) != -1)
	{
		sio_bufferWrite(&sioRxBuffer, (unsigned char)rxChar);
	}

	/* second, wait until this char has been sent out */
	while (!UTL_VT_PutChar(c))
	{}
}


/* here, we use polling uart Rx to delay, to make there is no data loss. */
void DRV_SIO_Delay(unsigned int msec)
{
    int     rxChar;

    while (msec-- > 0)
    {
    	while ((rxChar = UTL_VT_GetChar(1)) != -1)
    	{
    		sio_bufferWrite(&sioRxBuffer, (unsigned char)rxChar);
    	}
	}
}


/* update SIO buffer, only used by interrupt vector */
void DRV_SIO_UpdateBuffer(void)
{
	int 	rxChar;

	/* check that there is any data in Rx buffer,
	 * if does, read and save it into sio buffer.
	 */
	while ((rxChar = UTL_VT_GetChar(1)) != -1)
	{
		sio_bufferWrite(&sioRxBuffer, (unsigned char)rxChar);
	}
}


/* init SIO buffer */
void DRV_SIO_Init(void)
{
	sio_bufferInit(&sioRxBuffer);
}

