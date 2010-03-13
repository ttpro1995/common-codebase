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

#define SOH					0x1		/* the start data of xModem packet */
#define CTL_C				0x3		/* terminate trasmission command   */
#define EOT					0x4		/* trasmission success command     */
#define ACK					0x6		/* ack command                     */
#define NAK					0x15	/* re-trasmit command              */
#define CAN					0x18	/* cancel trasmission command      */

#define DLY_TICKS			1
#define BACKUP_DELAY_TIME	1
#define TOTAL_FRAME_LEN 	(1+1+1+XMODEM_BLOCK_SIZE+1)

#define E_RCVOK 			0
#define E_SNDOK 			1

#define E_PAKNUM			2
#define E_NOACK 			3
#define E_NODATA			4
#define E_RCVCAN			5
#define E_SENCAN			6


#if XMODEM_UPLOAD_ENABLE

/* wait 1sec  */
#define XMODEM_UPLOAD_WAIT_TICK		(1000)


/*
 * WaitAnswer(MaxWaitTick)
 *
 * This subroutine will wait for the answer of the xmodem
 *
 * On Entry:
 *	MaxWaitTick : max tick to be wait
 *
 * On Exit:
 *	0 -> fail
 *	else
 *	   return value
 */
unsigned short _wait_answer(int MaxWaitTick)
{
	int waittick;
	int ch;

	waittick = 0;

	while (waittick++ < MaxWaitTick)
	{
		ch = xModem_GetChar(XMODEM_UPLOAD_WAIT_TICK);

		if (ch == -1)
		{
			continue;
		}

		switch (ch)
		{
			case NAK:
			case CAN:
			case ACK:
			case EOT:
			case SOH:
			case CTL_C:
				return ch;
		}
	}

	return 0;
}

/*
 * Calculate, intelligently, the CRC of a dataset incrementally given a
 * buffer full at a time.
 * Initialize crc to 0 for XMODEM, -1 for CCITT.
 *
 * Usage:
 *   newcrc = updcrc( oldcrc, bufadr, buflen )
 *	  unsigned int oldcrc, buflen;
 *	  char *bufadr;
 */

	/* the CRC polynomial. This is used by XMODEM (almost CCITT).
	 * If you change P, you must change crctab[]'s initial value to what is
	 * printed by initcrctab()
	 */
#define P    		0x1021

    /* number of bits in CRC: don't change it. */
#define W 			16

    /* this the number of bits per char: don't change it. */
#define B 			8

static unsigned short crctab[1<<B] = { /* as calculated by initcrctab() */
    0x0000,  0x1021,  0x2042,  0x3063,	0x4084,  0x50a5,  0x60c6,  0x70e7,
    0x8108,  0x9129,  0xa14a,  0xb16b,	0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
    0x1231,  0x0210,  0x3273,  0x2252,	0x52b5,  0x4294,  0x72f7,  0x62d6,
    0x9339,  0x8318,  0xb37b,  0xa35a,	0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
    0x2462,  0x3443,  0x0420,  0x1401,	0x64e6,  0x74c7,  0x44a4,  0x5485,
    0xa56a,  0xb54b,  0x8528,  0x9509,	0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
    0x3653,  0x2672,  0x1611,  0x0630,	0x76d7,  0x66f6,  0x5695,  0x46b4,
    0xb75b,  0xa77a,  0x9719,  0x8738,	0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
    0x48c4,  0x58e5,  0x6886,  0x78a7,	0x0840,  0x1861,  0x2802,  0x3823,
    0xc9cc,  0xd9ed,  0xe98e,  0xf9af,	0x8948,  0x9969,  0xa90a,  0xb92b,
    0x5af5,  0x4ad4,  0x7ab7,  0x6a96,	0x1a71,  0x0a50,  0x3a33,  0x2a12,
    0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,	0x9b79,  0x8b58,  0xbb3b,  0xab1a,
    0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,	0x2c22,  0x3c03,  0x0c60,  0x1c41,
    0xedae,  0xfd8f,  0xcdec,  0xddcd,	0xad2a,  0xbd0b,  0x8d68,  0x9d49,
    0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,	0x3e13,  0x2e32,  0x1e51,  0x0e70,
    0xff9f,  0xefbe,  0xdfdd,  0xcffc,	0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
    0x9188,  0x81a9,  0xb1ca,  0xa1eb,	0xd10c,  0xc12d,  0xf14e,  0xe16f,
    0x1080,  0x00a1,  0x30c2,  0x20e3,	0x5004,  0x4025,  0x7046,  0x6067,
    0x83b9,  0x9398,  0xa3fb,  0xb3da,	0xc33d,  0xd31c,  0xe37f,  0xf35e,
    0x02b1,  0x1290,  0x22f3,  0x32d2,	0x4235,  0x5214,  0x6277,  0x7256,
    0xb5ea,  0xa5cb,  0x95a8,  0x8589,	0xf56e,  0xe54f,  0xd52c,  0xc50d,
    0x34e2,  0x24c3,  0x14a0,  0x0481,	0x7466,  0x6447,  0x5424,  0x4405,
    0xa7db,  0xb7fa,  0x8799,  0x97b8,	0xe75f,  0xf77e,  0xc71d,  0xd73c,
    0x26d3,  0x36f2,  0x0691,  0x16b0,	0x6657,  0x7676,  0x4615,  0x5634,
    0xd94c,  0xc96d,  0xf90e,  0xe92f,	0x99c8,  0x89e9,  0xb98a,  0xa9ab,
    0x5844,  0x4865,  0x7806,  0x6827,	0x18c0,  0x08e1,  0x3882,  0x28a3,
    0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,	0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
    0x4a75,  0x5a54,  0x6a37,  0x7a16,	0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
    0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,	0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
    0x7c26,  0x6c07,  0x5c64,  0x4c45,	0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
    0xef1f,  0xff3e,  0xcf5d,  0xdf7c,	0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
    0x6e17,  0x7e36,  0x4e55,  0x5e74,	0x2e93,  0x3eb2,  0x0ed1,  0x1ef0
};

static unsigned short _updcrc
(
	unsigned short  icrc,
	unsigned char  *icp,
	unsigned int    icnt
)
{
	register unsigned short  crc = icrc;
	register unsigned char  *cp  = icp;
	register unsigned int    cnt = icnt;

	while (cnt--)
	{
		crc = (crc<<B) ^ crctab[(crc>>(W-B)) ^ *cp++];
	}

	return (crc);
}


/*
 *  WaitAnswer_first
 *
 *  When the device are act as receiver, it need to wait for the sender
 *  to send the NAK signal,
 *  For the Hperterm it will send the 'C'(0x43) instat of the NAK
 *  If we receive the 0x43, we need to run the XMODEM-CRC protocol
 *
 */
#define CFGLOAD_XMODEM_CRC_SIGN 0x43
static unsigned short _wait_answer_first(int MaxWaitTick)
{
	int waittick;
	int ch;

	waittick = 0;

	while (waittick++ < MaxWaitTick)
	{
		ch = xModem_GetChar(XMODEM_UPLOAD_WAIT_TICK);

		if (ch == -1)
		{
			continue;
		}

		switch (ch)
		{
			case NAK:
			case CAN:
			case ACK:
			case EOT:
			case SOH:
			case CTL_C:
				return ch;

			case CFGLOAD_XMODEM_CRC_SIGN:
				return ch;
		}
	}

	return 0;
}


#define CFGLOAD_UPLOAD_TICKCOUNT	300
#define CFGLOAD_WAIT_ACK_SEC		100
#define XMODEM_WAIT_FIRST_SEC		(5*100*60)
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
)
{
	unsigned short ret;
	unsigned int   curseq;
	unsigned int   total_block;
	unsigned char  cksum;
	int	           retry;
	int	           i;
	unsigned short crc;
	int            xmodem_crc_protocol;

	xmodem_crc_protocol = 0;
	total_block         = retlen / XMODEM_BLOCK_SIZE;

	/* wait for 5*60*18 ticks */
	for (i=0; i< CFGLOAD_UPLOAD_TICKCOUNT; i++)
	{
		if ((ret = _wait_answer_first(XMODEM_WAIT_FIRST_SEC)) == CAN)
		{
			return CON_CST_USER_ABORT;
		}
		else if (ret == CTL_C)
		{
			return CON_CST_USER_ABORT;
		}
		else if (ret == NAK)
		{
			break;
		}
		else if (ret == CFGLOAD_XMODEM_CRC_SIGN)
		{
			xmodem_crc_protocol = 1;
			break;
		}
	}

	/* check if time out */
	if (i == CFGLOAD_UPLOAD_TICKCOUNT)
	{
		return CON_CST_TIME_OUT;
	}

	curseq = 1;
	retry  = 0;

	while ((curseq <= total_block) && (retry < XMODEM_MAX_RETRY_NO))
	{
		/* send SOH */
		xModem_SendChar(SOH);

		/* send sequence no */
		xModem_SendChar(curseq);

		/* send seq com */
		xModem_SendChar((~curseq) & 0xff);

		/* send 128 bytes */
		for (i=0,cksum=0; i<XMODEM_BLOCK_SIZE; i++)
		{
			cksum += curdata[i];
			xModem_SendChar(curdata[i]);
		}

		if (xmodem_crc_protocol)
		{
			crc = _updcrc(0, (unsigned char *)curdata, 128);
			xModem_SendChar((crc>>8) & 0xff);
			xModem_SendChar(crc & 0xff);
		}
		else
		{
			/* send only check sum */
			xModem_SendChar(cksum);
		}

		ret = _wait_answer(CFGLOAD_WAIT_ACK_SEC);
		if (ret == CTL_C)
		{
			xModem_SendChar(EOT);
			return CON_CST_USER_ABORT;
		}

		if (ret == ACK)
		{
			retry = 0;
			curseq++;
			curdata += XMODEM_BLOCK_SIZE;
		}
		else if (ret == E_NOACK)
		{
			retry++;
		}
		else if (ret == NAK)
		{
			/* try the same packet again */
			retry++;
		}
		else if (ret == CAN)
		{
			return CON_CST_USER_ABORT;
		}
	}

	/* end if transmit */
	if (retry >= XMODEM_MAX_RETRY_NO)
	{
		return CON_CST_TIME_OUT;
	}

	/* normal end */
	xModem_SendChar(EOT);
	ret = _wait_answer(CFGLOAD_WAIT_ACK_SEC);
	if (ret == ACK)
	{
		return CON_CST_XMODEM_UPLOAD_SUCCESS;
	}

	/* send again */
	xModem_SendChar(EOT);

	return CON_CST_XMODEM_UPLOAD_SUCCESS;
}

#endif


#if XMODEM_DOWNLOAD_ENABLE

static unsigned char xmodem_buf[TOTAL_FRAME_LEN];

/* wait 1sec  */
#define XMODEM_DOWNLOAD_WAIT_TICK		(1000)


/*
 * SaveToDram(char *data, int curseq, char *buf)
 *
 * On Entry:
 *	data	: data
 *	curseq	: current sequence number
 *	buf	    : buffer to keep the data
 *
 * On Exit:
 *	-1 : fail
 *  0  : success
 */
static int _save_to_dram
(
	unsigned char *data,
	unsigned long  curseq,
	unsigned char *buf
)
{
	int	           len;
	unsigned char  cksum = 0;
	unsigned char *destaddr;

	destaddr = &buf[(curseq-1) * XMODEM_BLOCK_SIZE];

	for (len=0; len<XMODEM_BLOCK_SIZE; len++)
	{
		cksum      += *data;
		*destaddr++ = *data++;
	}

	/* checksum error */
	if (cksum != *data)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

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
)
{
	const unsigned long	 max_block_no = buf_len/XMODEM_BLOCK_SIZE;
	unsigned long        retry_count;
	unsigned long        block_no, recv_len;
	int                  ch;

	*blkno   = 0;

	/* send start command */
	xModem_SendChar(NAK);

	block_no    = 1;
	retry_count = 0;
	while ((block_no <= max_block_no) && (retry_count++ < XMODEM_MAX_RETRY_NO))
	{
        /* clear the xmodem buffer */
        memset(xmodem_buf, 0x0, sizeof(xmodem_buf));

	    /* reset receive data, start a new trasmission */
	    recv_len = 0;

		/* wait for the start of packet from sender */
		while (1)
		{
    		ch = xModem_GetChar(XMODEM_DOWNLOAD_WAIT_TICK);
    		if (ch == SOH)
    		{
    			xmodem_buf[recv_len++] = ch;
    			retry_count = 0;

    			/* get a block */
    			while (recv_len < TOTAL_FRAME_LEN)
    			{
    				ch = xModem_GetChar(XMODEM_DOWNLOAD_WAIT_TICK);

    				if (ch != -1)
    				{
    					xmodem_buf[recv_len++] = ch;
    				}
    				else
    				{
    					/* timeout, reset first command to 0, to get it again */
    					xmodem_buf[0] = 0;
    					break;
    				}
    			}

    			break;
    		}
    		else if (ch == CAN)
    		{
    			xModem_SendChar(ACK);
    			*blkno = 0;
    			return CON_CST_USER_ABORT;
    		}
    		else if (ch == CTL_C)
    		{
    			xModem_SendChar(CAN);
    			*blkno = 0;
    			return CON_CST_USER_ABORT;
    		}
    		else if (ch == EOT)
    		{
    			/* receive trasmission finished command */
    			xModem_SendChar(ACK);
    			*blkno = block_no - 1;
    			return CON_CST_XMODEM_DOWNLOAD_SUCCESS;
    		}
    		else
    		{
    		    /* timeout, reset first command to 0, to get it again */
    		    xmodem_buf[0] = 0;
    			break;
    		}
		}

		if (xmodem_buf[0] != SOH)
		{
			/* invalid start of data format, get it again */
			xModem_SendChar(NAK);
		}
		else if (xmodem_buf[1] == (block_no - 1))
		{
			/* if sender retransmit the previous packet, just only send ack */
			xModem_SendChar(ACK);
		}
		else if (xmodem_buf[1] != (block_no & 0xff))
		{
			/* invalid block, get it again */
			xModem_SendChar(NAK);
		}
		else if (xmodem_buf[2] != (0xFF - (block_no & 0xff)))
 		{
			/* invalid block, get it again */
			xModem_SendChar(NAK);
		}
		else if (_save_to_dram(&xmodem_buf[3], block_no, buf) != 0)
		{
			/* save to dram fail, retry this packet */
			xModem_SendChar(NAK);
		}
		else
		{
			/* success, get next block */
			xModem_SendChar(ACK);
			retry_count = 0;
			block_no++;
		}
	}

	xModem_SendChar(CAN);
	return CON_CST_BAD_FILE;
}

#endif

