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
 *    This is the common hardware layer driver for flash,
 *     and, it supports x8 and x16 type flash.
 *     including AMD, INTEL, ATMEL, SST, SPANSION, EON, FUJISTU, and others.
 *
 * History:
 *    2007.03.27    Panda Xiong       Create
 *
******************************************************************************/

#include <h/basetype.h>
#include <inc/drv_flash.h>

/* define all flash information on board */
static FLASH_INFO_T allFlashInfo[] = DRV_FLASH_AllOnBoard;

/* semaphore lock flash */
#define FLASH_SEM_INIT()
#define FLASH_LOCK()
#define FLASH_UNLOCK()

typedef GT_BOOL (*FLASH_ERASE_FUNC)
(
    IN       GT_UI8			*sectorAddr,
    IN const FLASH_INFO_T	*flash_info
);
typedef GT_BOOL (*FLASH_WRITE_FUNC)
(
	IN       GT_UI8			*writeAddr,
	IN const GT_UI8			*dataAddr,
	IN const FLASH_INFO_T	*flash_info
);
typedef GT_BOOL (*FLASH_WRITEBUF_FUNC)
(
	IN       GT_UI8			*writeAddr,
	IN const GT_UI8			*data,
	IN       GT_UI32		count,
	IN const FLASH_INFO_T	*flash_info
);

/******************************************************************************
 * The below codes shouldn't be modified, unless you know it very much.
******************************************************************************/

#define DRV_FLASH_MIN_SECTOR_SIZE			KB(4)

/* software timing */
#define DRV_FLASH_ERASE_MIN_DELAY_TIME		25000		/* us, minimum 0.025ms */
#define DRV_FLASH_ERASE_MAX_POLLING_TIME	2000000		/* us, maximum 2s      */
#define DRV_FLASH_WRITE_MIN_DELAY_TIME		5			/* us, minimum 0.025us */
#define DRV_FLASH_WRITE_MAX_POLLING_TIME	1000000		/* us, maximum 1s      */

#define drv_flash_DelayUs(x)				delay_us(x)
#define drv_flash_DelayMs(x)				delay_ms(x)

#if AMD_FLASH_SUPPORT

/* AMD flash command */
#define Amd_Flash_Cmd_1      				0xAA
#define Amd_Flash_Cmd_2      				0x55
#define Amd_Flash_Cmd_Reset     			0xF0
#define Amd_Flash_Cmd_ID     				0x90
#define Amd_Flash_Cmd_Write     			0xA0
#define Amd_Flash_Cmd_Erase     			0x80
#define Amd_Flash_Cmd_EraseSector   		0x30
#define Amd_Flash_Cmd_EraseChip    			0x10
#define Amd_Flash_Cmd_WriteBuffer			0x25
#define Amd_Flash_Cmd_WriteBufferConfirm	0x29

/* AMD flash toggle bit */
#define Amd_Flash_Toggle_Bit6    			(1 << 6)

/* MAX buffer size */
#define Amd_Flash_MaxBuffer  				32

#endif

#if INTEL_FLASH_SUPPORT

/* Intel flash command */
#define Intel_Flash_Cmd_1      				0xAA
#define Intel_Flash_Cmd_2      				0x55
#define Intel_Flash_Cmd_Reset    			0xFF
#define Intel_Flash_Cmd_ReadStatus   		0x70
#define Intel_Flash_Cmd_ClearStatus   		0x50
#define Intel_Flash_Cmd_StatusReady   		0x80
#define Intel_Flash_Cmd_BlockErase_1 		0x20
#define Intel_Flash_Cmd_BlockErase_2  		0xD0
#define Intel_Flash_Cmd_WordByteProgram  	0x40
#define Intel_Flash_Cmd_WriteBuffer   		0xE8
#define Intel_Flash_Cmd_WriteBufferConfirm 	0xD0
#define Intel_Flash_Cmd_EnterLockMode		0x60
#define Intel_Flash_Cmd_Unlock				0xD0
#define Intel_Flash_Cmd_Lock				0x01
/* MAX buffer size */
#define Intel_Flash_MaxBuffer  				32

#endif

/*------------- some flash chip time table ------------
				Toggle  Polling  DQ5   time
ATMEL	Erase	   X	  	X 	    X	  	1.0s
		Program	   X      	X	    X	  	12us
AMD		Erase	   X	  	X 	    X	  	0.7s
		Program	   X      	X	    X	  	7us(word)
K8D		Erase	   X	  	X 	    X	  	0.7s
		Program	   X      	X	    X	  	14us
MX		Erase	   X	  	X 	    X	  	0.7s
		Program	   X      	X	    X	  	14us
Spasion	Erase	   X	  	X 	    X	  	0.7s
		Program	   X      	X	    X	  	18us
winbond	Erase	   X	  	X 	    X	  	0.7s
		Program	   X      	X	    X	  	7us
SST		Erase	   X	  	X 	    X	  	0.025s
		Program	   X      	X	    X	  	10us
------------------------------------------------*/
#define Flash_EraseDelayTime   				25000
#define Flash_WriteDelayTime   				7
#define Flash_ErasePollingTime    			20000000
#define Flash_WritePollingTime    			1000

/* used to distinguish is erase process in auto program or not */
typedef enum
{
	AUTO_PROGRAM_EXIT        = 0,
	AUTO_PROGRAM_ERASE_STAGE = 1,
	AUTO_PROGRAM_WRITE_STAGE = 2
} DRV_FLASH_AutoProgramStage;

static DRV_FLASH_AutoProgramStage autoProgramStage  = AUTO_PROGRAM_EXIT;

static void drv_flash_Delay(GT_UI32 us)
{
	volatile unsigned long n;

	n = us * 12;
    while (n--)
    {}
}

/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static void drv_flash_DealCallback
(
	IN DISPLAY_CALLBACK	displayFunc,		/* display callback function */
	IN GT_UI32			total,				/* total numbers to deal     */
	IN GT_UI32			current,			/* current number            */
	IN OUT GT_UI32		*prev_percent		/* previous display percent  */
)
{
	GT_UI32	current_percent;
	GT_UI32	display_percent;
	GT_UI32	callback_step;

	if ((displayFunc == NULL)
			|| (prev_percent == NULL)
			|| (*prev_percent > 100))		/* over 100%, don't display    */
	{
		return;
	}

	/* get the current percent */
	current_percent = current * 100 / total;

	if (current_percent != *prev_percent)
	{
		if (autoProgramStage == AUTO_PROGRAM_EXIT)
		{
			callback_step = CALLBACK_STEP;
		}
		else
		{
			callback_step = CALLBACK_STEP*2;
		}

		/* align to the callback percent */
		current_percent = ((current_percent+callback_step-1)/callback_step) * callback_step;

		if (current_percent - *prev_percent >= callback_step)
		{
			switch (autoProgramStage)
			{
				case AUTO_PROGRAM_ERASE_STAGE:
					display_percent = current_percent/2;
					break;

				case AUTO_PROGRAM_WRITE_STAGE:
					display_percent = current_percent/2 + 50;
					break;

				case AUTO_PROGRAM_EXIT:
					display_percent = current_percent;
					break;

				default:
					return;
			}

			(*displayFunc)(display_percent);

			/* save the new percent */
			*prev_percent = current_percent;
		}
	}

	return;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static FLASH_INFO_T *drv_flash_GetFlashInfo
(
	IN GT_UI8	*addr,
	IN GT_UI32	len
)
{
	GT_UI32	i;

	if (addr == NULL)
	{
		return NULL;
	}

	for (i=0; i<sizeof(allFlashInfo)/sizeof(allFlashInfo[0]); i++)
	{
		if ((addr >= allFlashInfo[i].baseAddr)
			&& (addr <= (allFlashInfo[i].baseAddr - 1 + allFlashInfo[i].size))
			&& ((addr+len) <= (allFlashInfo[i].baseAddr - 1 + allFlashInfo[i].size)))
		{
			return (FLASH_INFO_T *)&allFlashInfo[i];
		}
	}

	return NULL;
}


#if AMD_FLASH_SUPPORT

/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_AmdCheckResult
(
    IN const GT_UI8			*addr,
	IN const FLASH_INFO_T	*flash_info
)
{
    GT_I32 	int_level;
    GT_UI8 	prev, curr, dq6_toggles;

    if (addr == NULL)
    {
        return GT_FALSE;
    }

    int_level = Drv_Flash_IntLock();
	if (flash_info->byteMode)
	{
		prev = Drv_Flash_IO_ReadByte(addr);
		curr = Drv_Flash_IO_ReadByte(addr);
	}
	else
	{
        prev = Drv_Flash_IO_ReadHalfWord(addr);
        curr = Drv_Flash_IO_ReadHalfWord(addr);
	}
    Drv_Flash_IntUnlock(int_level);

	dq6_toggles = (prev ^ curr) & Amd_Flash_Toggle_Bit6;

    if (dq6_toggles)
    {
	    int_level = Drv_Flash_IntLock();
		if (flash_info->byteMode)
		{
			prev = Drv_Flash_IO_ReadByte(addr);
			curr = Drv_Flash_IO_ReadByte(addr);
		}
		else
		{
			prev = Drv_Flash_IO_ReadHalfWord(addr);
	        curr = Drv_Flash_IO_ReadHalfWord(addr);
		}
	    Drv_Flash_IntUnlock(int_level);

		dq6_toggles = (prev ^ curr) & Amd_Flash_Toggle_Bit6;
		if (dq6_toggles)
		{
			return GT_FALSE;
		}
    }
	else
	{
		return GT_TRUE;
	}

    return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static void drv_flash_AmdReset
(
    IN const FLASH_INFO_T	*flash_info
)
{
    GT_UI8	*addr555, *addrAAA;
    GT_I32	int_level;

    if (flash_info == NULL)
    {
        return;
    }

    if (flash_info->byteMode)
    {
        addr555 = flash_info->baseAddr + 0x5555;
        addrAAA = flash_info->baseAddr + 0xAAAA;
    }
    else
    {
        addr555 = flash_info->baseAddr + 0xAAAA;
        addrAAA = flash_info->baseAddr + 0x5555;
    }

    /* write reset command */
	int_level = Drv_Flash_IntLock();
    Drv_Flash_IO_WriteByte(addr555, Amd_Flash_Cmd_1);
    Drv_Flash_IO_WriteByte(addrAAA, Amd_Flash_Cmd_2);
    Drv_Flash_IO_WriteByte(addr555, Amd_Flash_Cmd_Reset);
	Drv_Flash_IntUnlock(int_level);

    /* for SST */
    drv_flash_Delay(1);
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static void drv_flash_AmdSetSectorLockStatus
(
    IN       GT_UI8			*sectorAddr,
    IN const GT_BOOL		lock_status,
    IN const FLASH_INFO_T	*flash_info
)
{
	GT_I32	int_level;

    int_level = Drv_Flash_IntLock();

	/* reset */
	drv_flash_AmdReset(flash_info);

	// TBD

	/* reset */
	drv_flash_AmdReset(flash_info);

    Drv_Flash_IntUnlock(int_level);

	return;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_AmdUnlock
(
    IN const FLASH_INFO_T	*flash_info
)
{
	GT_UI32		sector_num;
	GT_UI32		total_sector_num;
	GT_UI8		*sector_base;

	if (flash_info == NULL)
	{
		return GT_FALSE;
	}

	total_sector_num = flash_info->size / flash_info->blockSize;

	for (sector_num = 0; sector_num < total_sector_num; sector_num++)
	{
		sector_base = (GT_UI8 *)((GT_UI32)flash_info->baseAddr
									+ sector_num * flash_info->blockSize);

		drv_flash_AmdSetSectorLockStatus(sector_base, GT_FALSE, flash_info);
	}

	return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_AmdEraseSector
(
    IN       GT_UI8			*sectorAddr,
    IN const FLASH_INFO_T	*flash_info
)
{
    GT_UI8		*addr555, *addrAAA;
    GT_I32		int_level, i;

    if ((sectorAddr == NULL) || (flash_info == NULL))
    {
        return GT_FALSE;
    }

    if (flash_info->byteMode)
    {
        addr555 = flash_info->baseAddr + 0x5555;
        addrAAA = flash_info->baseAddr + 0xAAAA;
    }
    else
    {
        addr555 = flash_info->baseAddr + 0xAAAA;
        addrAAA = flash_info->baseAddr + 0x5555;
    }

    /* reset */
    drv_flash_AmdReset(flash_info);

    /* erase sector */
    int_level = Drv_Flash_IntLock();
    Drv_Flash_IO_WriteByte(addr555,    Amd_Flash_Cmd_1);
    Drv_Flash_IO_WriteByte(addrAAA,    Amd_Flash_Cmd_2);
    Drv_Flash_IO_WriteByte(addr555,    Amd_Flash_Cmd_Erase);
    Drv_Flash_IO_WriteByte(addr555,    Amd_Flash_Cmd_1);
    Drv_Flash_IO_WriteByte(addrAAA,    Amd_Flash_Cmd_2);
	if (flash_info->byteMode)
	{
	    Drv_Flash_IO_WriteByte(sectorAddr, Amd_Flash_Cmd_EraseSector);
	}
	else
	{
	    Drv_Flash_IO_WriteHalfWord(sectorAddr, Amd_Flash_Cmd_EraseSector);
	}
    Drv_Flash_IntUnlock(int_level);

	/* check write result */
	for (i = 0;
		 i < DRV_FLASH_ERASE_MAX_POLLING_TIME/DRV_FLASH_ERASE_MIN_DELAY_TIME;
		 i++)
	{
		/* delay some time, to give out the CPU,
		 * cause there is a minimum erase sector timing.
		 */
		drv_flash_DelayMs(DRV_FLASH_ERASE_MIN_DELAY_TIME/1000);

		if (drv_flash_AmdCheckResult(sectorAddr, flash_info))
		{
			return GT_TRUE;
		}
	}

    /* reset */
    drv_flash_AmdReset(flash_info);

    return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_AmdWrite
(
    IN       GT_UI8			*writeAddr,
    IN const GT_UI8			*dataAddr,
    IN const FLASH_INFO_T	*flash_info
)
{
    GT_UI8		*addr555, *addrAAA;
    GT_I32		int_level, i;

    if ((writeAddr == NULL) || (flash_info == NULL))
    {
        return GT_FALSE;
    }

    if (flash_info->byteMode)
    {
	    /* check if need to write or not,
	     * if equal, skip write.
	     */
        if (Drv_Flash_IO_ReadByte(writeAddr) == (VP8(dataAddr)))
        {
            return GT_TRUE;
        }

        addr555 = flash_info->baseAddr + 0x5555;
        addrAAA = flash_info->baseAddr + 0xAAAA;
    }
    else
    {
    	/* check if need to write or not,
	     * if equal, skip write.
	     */
        if (Drv_Flash_IO_ReadHalfWord(writeAddr) == (VP16(dataAddr)))
        {
            return GT_TRUE;
        }

        addr555 = flash_info->baseAddr + 0xAAAA;
        addrAAA = flash_info->baseAddr + 0x5555;
    }

    /* reset */
    drv_flash_AmdReset(flash_info);

    /* write */
    int_level = Drv_Flash_IntLock();
    Drv_Flash_IO_WriteByte(addr555, Amd_Flash_Cmd_1);
    Drv_Flash_IO_WriteByte(addrAAA, Amd_Flash_Cmd_2);
    Drv_Flash_IO_WriteByte(addr555, Amd_Flash_Cmd_Write);
    if (flash_info->byteMode)
    {
		Drv_Flash_IO_WriteByte(writeAddr, (VP8(dataAddr)));
    }
    else
    {
		Drv_Flash_IO_WriteHalfWord(writeAddr, (VP16(dataAddr)));
    }
    Drv_Flash_IntUnlock(int_level);

	/* check write result */
	for (i = 0;
		 i < DRV_FLASH_WRITE_MAX_POLLING_TIME/DRV_FLASH_WRITE_MIN_DELAY_TIME;
		 i++)
	{
		/* delay some time, to give out the CPU,
		 * cause there is a minimum write byte/word timing.
		 */
		drv_flash_DelayUs(DRV_FLASH_WRITE_MIN_DELAY_TIME);

		if (drv_flash_AmdCheckResult(writeAddr, flash_info))
		{
			/* here, we can not check the written data is valid or not,
			 * see "Program Attention" at the head of this file for details.
			 */
			return GT_TRUE;
		}
	}

    /* reset */
    drv_flash_AmdReset(flash_info);

    return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_AmdWriteBuffer
(
	IN       GT_UI8			*writeAddr,
	IN const GT_UI8			*data,
	IN       GT_UI32		count,
	IN const FLASH_INFO_T	*flash_info
)
{
    GT_UI8	*addr555, *addrAAA;
	GT_UI8	*lastAddr;
	GT_I32	int_level, i;
	GT_UI32	tmp_addr;

	/* no need to write */
	if (count == 0)
	{
		return GT_TRUE;
	}

    if (flash_info->byteMode)
    {
	    /* check if need to write or not,
	     * if equal, skip write.
	     */

        addr555 = flash_info->baseAddr + 0x5555;
        addrAAA = flash_info->baseAddr + 0xAAAA;
    }
    else
    {
    	/* check if need to write or not,
	     * if equal, skip write.
	     */

        addr555 = flash_info->baseAddr + 0xAAAA;
        addrAAA = flash_info->baseAddr + 0x5555;
    }

    /* reset */
    drv_flash_AmdReset(flash_info);

    int_level = Drv_Flash_IntLock();
    Drv_Flash_IO_WriteByte(addr555, Amd_Flash_Cmd_1);
    Drv_Flash_IO_WriteByte(addrAAA, Amd_Flash_Cmd_2);
    Drv_Flash_IO_WriteByte(addr555, Amd_Flash_Cmd_WriteBuffer);

    tmp_addr  = (GT_UI32)data;
	if (flash_info->byteMode)
	{
	    /* tell the flash, the count of byte will be written */
		Drv_Flash_IO_WriteByte(writeAddr, count-1);

		for (i=0; i<count; i++, tmp_addr++)
		{
			Drv_Flash_IO_WriteByte(writeAddr+i, (VP8(tmp_addr)));
		}

		lastAddr = (GT_UI8 *)(tmp_addr - 1);
		Drv_Flash_IO_WriteByte(lastAddr, Amd_Flash_Cmd_WriteBufferConfirm);
	}
	else
	{
	    /* tell the flash, the count of half-word will be written */
		Drv_Flash_IO_WriteHalfWord(writeAddr, count/2-1);

		for (i=0; i<count; i+=2, tmp_addr+=2)
		{
			Drv_Flash_IO_WriteHalfWord(writeAddr+i, (VP16(tmp_addr)));
		}

		lastAddr = (GT_UI8 *)(tmp_addr - 2);
		Drv_Flash_IO_WriteHalfWord(lastAddr, Amd_Flash_Cmd_WriteBufferConfirm);
	}
	Drv_Flash_IntUnlock(int_level);

	/* check write result */
	for (i = 0;
		 i < DRV_FLASH_WRITE_MAX_POLLING_TIME/DRV_FLASH_WRITE_MIN_DELAY_TIME;
		 i++)
	{
		/* delay some time, to give out the CPU,
		 * cause there is a minimum write byte/word timing.
		 */
		drv_flash_DelayUs(DRV_FLASH_WRITE_MIN_DELAY_TIME);

		if (drv_flash_AmdCheckResult(writeAddr, flash_info))
		{
			/* here, we can not check the written data is valid or not,
			 * see "Program Attention" at the head of this file for details.
			 */
			return GT_TRUE;
		}
	}

    /* reset */
    drv_flash_AmdReset(flash_info);

    return GT_FALSE;
}

#endif


#if INTEL_FLASH_SUPPORT

/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_IntelCheckResult
(
    IN const GT_UI8			*addr,
	IN const FLASH_INFO_T	*flash_info
)
{
    GT_I32	int_level;
    GT_UI8	status;

	int_level = Drv_Flash_IntLock();
	if (flash_info->byteMode)
	{
    	status = Drv_Flash_IO_ReadByte(addr);
	}
	else
	{
		status = Drv_Flash_IO_ReadHalfWord(addr);
	}
	Drv_Flash_IntUnlock(int_level);

    if (status & Intel_Flash_Cmd_StatusReady)
    {
        return GT_TRUE;
    }

	return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static void drv_flash_IntelReset
(
    IN const FLASH_INFO_T	*flash_info
)
{
    GT_UI8	*addr555, *addrAAA;
	GT_I32	int_level;

    if (flash_info == NULL)
    {
        return;
    }

    if (flash_info->byteMode)
    {
        addr555 = flash_info->baseAddr + 0x5555;
        addrAAA = flash_info->baseAddr + 0xAAAA;
    }
    else
    {
        addr555 = flash_info->baseAddr + 0xAAAA;
        addrAAA = flash_info->baseAddr + 0x5555;
    }

    /* write reset command */
    int_level = Drv_Flash_IntLock();
    Drv_Flash_IO_WriteByte(addr555, Intel_Flash_Cmd_1);
    Drv_Flash_IO_WriteByte(addrAAA, Intel_Flash_Cmd_2);
    Drv_Flash_IO_WriteByte(addr555, Intel_Flash_Cmd_ClearStatus);
    Drv_Flash_IO_WriteByte(addr555, Intel_Flash_Cmd_Reset);
    Drv_Flash_IntUnlock(int_level);

	return;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static void drv_flash_IntelSetSectorLockStatus
(
    IN       GT_UI8			*sectorAddr,
    IN const GT_BOOL		lock_status,
    IN const FLASH_INFO_T	*flash_info
)
{
	GT_I32	int_level;

    int_level = Drv_Flash_IntLock();

	/* reset */
	drv_flash_IntelReset(flash_info);

	/* enter lock mode */
    Drv_Flash_IO_WriteByte(sectorAddr, Intel_Flash_Cmd_EnterLockMode);

	/* set lock/unlock status */
	if (lock_status)
	{
		Drv_Flash_IO_WriteByte(sectorAddr, Intel_Flash_Cmd_Lock);
	}
	else
	{
	    Drv_Flash_IO_WriteByte(sectorAddr, Intel_Flash_Cmd_Unlock);
	}

	/* reset */
    Drv_Flash_IO_WriteByte(sectorAddr, Intel_Flash_Cmd_Reset);

    Drv_Flash_IntUnlock(int_level);

	return;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_IntelUnlock
(
    IN const FLASH_INFO_T	*flash_info
)
{
	GT_UI32		sector_num;
	GT_UI32		total_sector_num;
	GT_UI8		*sector_base;

	if (flash_info == NULL)
	{
		return GT_FALSE;
	}

	total_sector_num = flash_info->size / flash_info->blockSize;

	for (sector_num = 0; sector_num < total_sector_num; sector_num++)
	{
		sector_base = (GT_UI8 *)((GT_UI32)flash_info->baseAddr
									+ sector_num * flash_info->blockSize);

		drv_flash_IntelSetSectorLockStatus(sector_base, GT_FALSE, flash_info);
	}

	return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_IntelEraseSector
(
    IN       GT_UI8			*sectorAddr,
    IN const FLASH_INFO_T	*flash_info
)
{
    GT_I32		int_level, i;

    if ((sectorAddr == NULL) || (flash_info == NULL))
    {
        return GT_FALSE;
    }

	/* here, we can't reset flash.
	 * else, the flash can't be erased.
	 * the reason is unknown.
	 */

    int_level = Drv_Flash_IntLock();
    Drv_Flash_IO_WriteByte(sectorAddr, Intel_Flash_Cmd_BlockErase_1);
    Drv_Flash_IO_WriteByte(sectorAddr, Intel_Flash_Cmd_BlockErase_2);
    Drv_Flash_IntUnlock(int_level);

	drv_flash_Delay(Flash_EraseDelayTime);

	/* check write result */
	for (i = 0;
		 i < DRV_FLASH_ERASE_MAX_POLLING_TIME/DRV_FLASH_ERASE_MIN_DELAY_TIME;
		 i++)
	{
		/* delay some time, to give out the CPU,
		 * cause there is a minimum erase sector timing.
		 */
		drv_flash_DelayMs(DRV_FLASH_ERASE_MIN_DELAY_TIME/1000);

		if (drv_flash_IntelCheckResult(sectorAddr, flash_info))
		{
			return GT_TRUE;
		}
	}

	/* reset */
	drv_flash_IntelReset(flash_info);

    return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_IntelWrite
(
	IN       GT_UI8			*writeAddr,
	IN const GT_UI8			*dataAddr,
	IN const FLASH_INFO_T	*flash_info
)
{
	GT_I32		int_level, i;

	/* check if need to write or not,
     * if equal, skip write.
     */
	if (flash_info->byteMode)
	{
		if (Drv_Flash_IO_ReadByte(writeAddr) == (VP8(dataAddr)))
		{
			return GT_TRUE;
		}
	}
	else
	{
		if (Drv_Flash_IO_ReadHalfWord(writeAddr) == (VP16(dataAddr)))
		{
			return GT_TRUE;
		}
	}


	/* here, we can't reset flash.
	 * else, the flash can't be writted.
	 * the reason is unknown.
	 */

	/* write */
	int_level = Drv_Flash_IntLock();
	Drv_Flash_IO_WriteByte(writeAddr, Intel_Flash_Cmd_WordByteProgram);
	if (flash_info->byteMode)
	{
		Drv_Flash_IO_WriteByte(writeAddr, (VP8(dataAddr)));
	}
	else
	{
		Drv_Flash_IO_WriteHalfWord(writeAddr, (VP16(dataAddr)));
	}
	Drv_Flash_IntUnlock(int_level);

	/* check write result */
	for (i = 0;
		 i < DRV_FLASH_WRITE_MAX_POLLING_TIME/DRV_FLASH_WRITE_MIN_DELAY_TIME;
		 i++)
	{
		/* delay some time, to give out the CPU,
		 * cause there is a minimum write byte/word timing.
		 */
		drv_flash_DelayUs(DRV_FLASH_WRITE_MIN_DELAY_TIME);

		if (drv_flash_IntelCheckResult(writeAddr, flash_info))
		{
			/* here, we can not check the written data is valid or not,
			 * see "Program Attention" at the head of this file for details.
			 */
			return GT_TRUE;
		}
	}

	/* reset */
	drv_flash_IntelReset(flash_info);

	return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_IntelWriteBuffer
(
	IN       GT_UI8			*writeAddr,
	IN const GT_UI8			*data,
	IN       GT_UI32		count,
	IN const FLASH_INFO_T	*flash_info
)
{
	GT_I32		int_level, i;
	GT_UI32		tmp_addr;

	/* no need to write */
	if (count == 0)
	{
		return GT_TRUE;
	}

	/* here, we can't reset flash.
	 * else, the flash can't be writted.
	 * the reason is unknown.
	 */

	int_level = Drv_Flash_IntLock();
	Drv_Flash_IO_WriteByte(writeAddr, Intel_Flash_Cmd_WriteBuffer);
	Drv_Flash_IntUnlock(int_level);

	/* check result */
	for (i = 0;
		 i < DRV_FLASH_WRITE_MAX_POLLING_TIME/DRV_FLASH_WRITE_MIN_DELAY_TIME;
		 i++)
	{
		/* delay some time, to give out the CPU,
		 * cause there is a minimum write byte/word timing.
		 */
		drv_flash_DelayUs(DRV_FLASH_WRITE_MIN_DELAY_TIME);

		if (drv_flash_IntelCheckResult(writeAddr, flash_info))
		{
			goto _exit;
		}
	}

    tmp_addr  = (GT_UI32)data;
	int_level = Drv_Flash_IntLock();
	if (flash_info->byteMode)
	{
	    /* tell the flash, the count of byte will be written */
		Drv_Flash_IO_WriteByte(writeAddr, count-1);

		for (i=0; i<count; i++, tmp_addr++)
		{
			Drv_Flash_IO_WriteByte(writeAddr+i, (VP8(tmp_addr)));
		}
	}
	else
	{
	    /* tell the flash, the count of half-word will be written */
		Drv_Flash_IO_WriteByte(writeAddr, count/2-1);

		for (i=0; i<count; i+=2, tmp_addr+=2)
		{
			Drv_Flash_IO_WriteHalfWord(writeAddr+i, (VP16(tmp_addr)));
		}
	}
	Drv_Flash_IO_WriteByte(writeAddr, Intel_Flash_Cmd_WriteBufferConfirm);
	Drv_Flash_IntUnlock(int_level);

	/* check write result */
	for (i = 0;
		 i < DRV_FLASH_WRITE_MAX_POLLING_TIME/DRV_FLASH_WRITE_MIN_DELAY_TIME;
		 i++)
	{
		/* delay some time, to give out the CPU,
		 * cause there is a minimum write byte/word timing.
		 */
		drv_flash_DelayUs(DRV_FLASH_WRITE_MIN_DELAY_TIME);

		if (drv_flash_IntelCheckResult(writeAddr, flash_info))
		{
			/* here, we can not check the written data is valid or not,
			 * see "Program Attention" at the head of this file for details.
			 */
			return GT_TRUE;
		}
	}

_exit:
	/* reset */
	drv_flash_IntelReset(flash_info);

	return GT_FALSE;
}

#endif


/*******************************************************************************
FUNCTION NAME:
	commonflash_GetAlignSize
DESCRIPTION:
	get align size.
INPUT:
	block_size		: flash block size.
OUTPUT:
	None
RETURN:
	align size.
NOTES:
	 block size        align size
	------------      ------------
	      2 KB         0xFFFFF800
	      4 KB         0xFFFFF000
	      8 KB         0xFFFFE000
	     16 KB         0xFFFFC000
	     32 KB         0xFFFF8000
	     64 KB         0xFFFF0000
	    128 KB         0xFFFE0000
HISTORY:
	Ver1.00   2008.09.27    Panda Xiong      Create
*******************************************************************************/
static GT_UI32 drv_flash_GetAlignSize(IN GT_UI32  block_size)
{
	GT_UI32	align_size = (GT_UI32)(~0);

	if ((block_size == 0) || (block_size & 0x1))
	{
		return align_size;
	}

	while ((block_size & 0x1) == 0)
	{
		align_size <<= 1;
		block_size >>= 1;
	}

	return align_size;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_EraseBlock
(
	IN       GT_UI8 		*sectorAddr,
	IN const FLASH_INFO_T	*flash_info
)
{
	GT_UI8				*addr;
	GT_UI32				i, align_size, sector_num;
	const GT_UI8        tmp_val = 0;
	FLASH_WRITE_FUNC	write_func;
	FLASH_ERASE_FUNC	erase_func;

	/* step 0 : check whole sector are all 0xFFFFFFFF,
		because most sector erase need 700ms,
		but read one block only need about 7ms,
		so do this check.
	*/
	for (i = 0, addr = sectorAddr;
		 i < flash_info->blockSize;
		 i+=4, addr+=4)
	{
		if ((VP32(addr)) != (GT_UI32)(~0))
		{
			goto erase_block_next;
		}
	}

	/* all are erased, no need to erase. */
	return GT_TRUE;

erase_block_next:

	align_size = drv_flash_GetAlignSize(flash_info->blockSize);
	sector_num = (flash_info->blockSize / DRV_FLASH_MIN_SECTOR_SIZE);

	/* get the corresponding function to control */
	if (flash_info->type == INTEL_TYPE)
	{
	#if INTEL_FLASH_SUPPORT
		write_func = drv_flash_IntelWrite;
		erase_func = drv_flash_IntelEraseSector;
	#else
		return GT_FALSE;
	#endif
	}
	else
	{
	#if AMD_FLASH_SUPPORT
		write_func = drv_flash_AmdWrite;
		erase_func = drv_flash_AmdEraseSector;
	#else
		return GT_FALSE;
	#endif
	}

	/* step 1 : Each sector first word program to 0,
	 *          used for check need erase.
	 */
	addr = sectorAddr + DRV_FLASH_MIN_SECTOR_SIZE;
	for (i=1; i<sector_num; i++, addr += DRV_FLASH_MIN_SECTOR_SIZE)
	{
		(*write_func)(addr, &tmp_val, flash_info);
	}

	/* step 2: Erase first block sector,
	 *         maybe real is a 4K/8K/16K/32K/64K/128K sector).
	 */
	if (!(*erase_func)(sectorAddr, flash_info))
	{
		return GT_FALSE;
	}

	/* step 3 : check next sector are erased, if not, erase it. */
	addr = sectorAddr + DRV_FLASH_MIN_SECTOR_SIZE;
	for (i=1; i<sector_num; i++, addr += DRV_FLASH_MIN_SECTOR_SIZE)
	{
		if (((VP8(addr)) != (GT_UI8)(~0))
				&& (!(*erase_func)(addr, flash_info)))
		{
			return GT_FALSE;
		}
	}

	return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_Erase
(
	IN GT_UI8			*eraseAddr,
	IN GT_UI32			eraseLen,
	IN DISPLAY_CALLBACK	displayFunc
)
{
	GT_UI8			*start, *end;
	GT_UI32			block_num, loop;
	GT_UI32			current_percent = 0;
	GT_UI32			align_size;
	GT_UI32			total_len;
	FLASH_INFO_T	*flash_info;

    /* align to 2 byte */
	total_len = (eraseLen+1) & (~0x1);

	/* get the corresponding flash information */
	flash_info = drv_flash_GetFlashInfo(eraseAddr, total_len);
	if (flash_info == NULL)
	{
		return GT_FALSE;
	}

	/* unlock this flash */
	if (flash_info->type == INTEL_TYPE)
	{
		drv_flash_IntelUnlock(flash_info);
	}
	else
	{
		drv_flash_AmdUnlock(flash_info);
	}

	align_size = drv_flash_GetAlignSize(flash_info->blockSize);

	/* align start and end offset to this block */
	start = (GT_UI8 *)((GT_UI32)eraseAddr & align_size);
	end   = (GT_UI8 *)((GT_UI32)(start - 1 + total_len) & align_size);

	/* total block number, start from 1 */
	block_num = (GT_UI32)(end-start)/flash_info->blockSize + 1;

	/* erase each block */
	for (loop = 1;
		 loop <= block_num;
		 loop++)
	{
		if (!drv_flash_EraseBlock(start, flash_info))
		{
			return GT_FALSE;
		}

		drv_flash_DealCallback(displayFunc,
							   block_num,
							   loop,
							   &current_percent);

		start += flash_info->blockSize;
	}

	return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
static GT_BOOL drv_flash_Write
(
	IN       GT_UI8 		*writeAddr,
	IN const GT_UI8 		*dataAddr,
	IN       GT_UI32 		dataLen,
	IN DISPLAY_CALLBACK		displayFunc
)
{
	GT_UI8				*current_flash_addr;
	GT_UI8				*current_data_addr;
	GT_UI32				current_percent = 0;
	GT_UI32				step_len;
	GT_UI32				max_buf_bytes;
	GT_UI32				total_len;
	FLASH_WRITE_FUNC	write_func;
	FLASH_WRITEBUF_FUNC	writebuf_func;
	FLASH_INFO_T		*flash_info;

	if ((writeAddr == NULL) || (dataAddr == NULL))
	{
		return GT_FALSE;
	}

    /* align to 2 byte */
	total_len = (dataLen+1) & (~0x1);

	/* get the corresponding flash information */
	flash_info = drv_flash_GetFlashInfo(writeAddr, total_len);
	if (flash_info == NULL)
	{
		return GT_FALSE;
	}

	/* unlock this flash */
	if (flash_info->type == INTEL_TYPE)
	{
		drv_flash_IntelUnlock(flash_info);
	}
	else
	{
		drv_flash_AmdUnlock(flash_info);
	}

	/* init current address in flash and SDRAM */
	current_flash_addr = (GT_UI8 *)writeAddr;
	current_data_addr  = (GT_UI8 *)dataAddr;

	if (flash_info->byteMode)
	{
	    step_len = 1;	/* write 1 bytes each time */
	}
	else
	{
	    step_len = 2;	/* write 2 bytes each time */
	}

	/* get the corresponding API to write */
	if (flash_info->type == INTEL_TYPE)
	{
		max_buf_bytes = Intel_Flash_MaxBuffer;

	  #if INTEL_FLASH_SUPPORT
		write_func    = drv_flash_IntelWrite;
		writebuf_func = drv_flash_IntelWriteBuffer;
	  #else
		return GT_FALSE;
	  #endif
	}
	else
	{
		max_buf_bytes = Amd_Flash_MaxBuffer;

	  #if AMD_FLASH_SUPPORT
		write_func    = drv_flash_AmdWrite;
		writebuf_func = drv_flash_AmdWriteBuffer;
	  #else
		return GT_FALSE;
	  #endif
	}

	if (flash_info->bufferMode)
	{
		GT_UI32		pre_write_count;

		/* 1st write, use word mode to write,
		 * to align the write address to flash max buffer size.
		 */
		if (((GT_UI32)writeAddr % max_buf_bytes) != 0)
		{
			pre_write_count = max_buf_bytes - (GT_UI32)writeAddr % max_buf_bytes;
			while ((pre_write_count > 0)
					&& (current_data_addr < dataAddr + total_len))
			{
				if (!(*write_func)(current_flash_addr, current_data_addr, flash_info))
				{
					return GT_FALSE;
				}

	    		drv_flash_DealCallback(displayFunc,
	    							   total_len,
	    							   current_data_addr - dataAddr,
	    							   &current_percent);

				current_data_addr  += step_len;
				current_flash_addr += step_len;
				pre_write_count--;
			}
		}

		/* 2nd write, use buffer mode to write */
		while (current_data_addr < dataAddr + total_len)
		{
			if (writeAddr + total_len - current_flash_addr < max_buf_bytes)
			{
				break;
			}

			if (!(*writebuf_func)(current_flash_addr,
								  current_data_addr,
								  max_buf_bytes,
								  flash_info))
			{
				return GT_FALSE;
			}

    		drv_flash_DealCallback(displayFunc,
    							   total_len,
    							   current_data_addr - dataAddr,
    							   &current_percent);

			current_flash_addr += max_buf_bytes;
			current_data_addr  += max_buf_bytes;
		}

		/* 3rd write, use word mode to write */
		while (current_data_addr < dataAddr + total_len)
		{
			if (!(*write_func)(current_flash_addr, current_data_addr, flash_info))
			{
				return GT_FALSE;
			}

    		drv_flash_DealCallback(displayFunc,
    							   total_len,
    							   current_data_addr - dataAddr,
    							   &current_percent);

			current_data_addr  += step_len;
			current_flash_addr += step_len;
		}
	}
	else
	{
		while (current_data_addr < dataAddr + total_len)
		{
			if (!(*write_func)(current_flash_addr, current_data_addr, flash_info))
			{
				return GT_FALSE;
			}

    		drv_flash_DealCallback(displayFunc,
    							   total_len,
    							   current_data_addr - dataAddr,
    							   &current_percent);

			current_flash_addr += step_len;
			current_data_addr  += step_len;
		}
	}

	return GT_TRUE;
}


/* flash API */
#if 1

/******************************************************************************
 * FUNCTION NAME:
 *		DRV_FLASH_AutoDetect
 * DESCRIPTION:
 *		Auto Detect flash.
 * INPUT:
 *		baseAddr : flash base address to be detected.
 *      bytemode : =TRUE, use byte mode to detect;
 *                 =FALSE, use none byte mode to detect.
 * OUTPUT:
 *		None
 * RETURN:
 *		Flash ID = dev_id3 | dev_id2 | dev_id1 | man_id.
 * NOTES:
 *		None
 * HISTORY:
 *		2008.03.03		Panda Xiong		Create
******************************************************************************/
GT_UI32 DRV_FLASH_AutoDetect
(
	IN const GT_UI8 *baseAddr,
	IN const GT_BOOL bytemode
)
{
	GT_UI8			*addrAAA, *addr555;
	GT_UI8			man_id, dev_id;
	GT_UI8			dev_id2=0, dev_id3=0;
	GT_UI32			id;
	int				int_level;
	FLASH_INFO_T	flash_info;

	MEMSET(&flash_info, 0x0, sizeof(flash_info));

	/* here, we only need the baseAddr and bytemode */
	flash_info.baseAddr = (GT_UI8 *)baseAddr;
	flash_info.byteMode = (GT_BOOL)bytemode;

	if (bytemode)
	{
		addrAAA = flash_info.baseAddr + 0x5555;
		addr555 = flash_info.baseAddr + 0xAAAA;
	}
	else
	{
		addrAAA = flash_info.baseAddr + 0xAAAA;
		addr555 = flash_info.baseAddr + 0x5555;
	}

	int_level = Drv_Flash_IntLock();

	/* reset */
	drv_flash_AmdReset(&flash_info);

	/* write read ID command */
	Drv_Flash_IO_WriteByte(addrAAA, Amd_Flash_Cmd_1);
	Drv_Flash_IO_WriteByte(addr555, Amd_Flash_Cmd_2);
	Drv_Flash_IO_WriteByte(addrAAA, Amd_Flash_Cmd_ID);

    /* for SST */
    drv_flash_Delay(1);

	/* get manufacture ID and device ID */
	if (bytemode)
	{
		man_id = (GT_UI8)Drv_Flash_IO_ReadByte(flash_info.baseAddr);
		dev_id = (GT_UI8)Drv_Flash_IO_ReadByte(flash_info.baseAddr + 1);
	}
	else
	{
		man_id = (GT_UI8)Drv_Flash_IO_ReadHalfWord(flash_info.baseAddr);
		dev_id = (GT_UI8)Drv_Flash_IO_ReadHalfWord((flash_info.baseAddr + 2));

		if (man_id == 0x7E)
		{
			/* if first device id is 0x7E, need cicle 2 and cicle 3,
			 * to distinguish them.
			 */
			dev_id2 = (GT_UI8)Drv_Flash_IO_ReadHalfWord((flash_info.baseAddr + 0x1C));
			dev_id3 = (GT_UI8)Drv_Flash_IO_ReadHalfWord((flash_info.baseAddr + 0x1E));
		}
	}

    /* for SST */
    drv_flash_Delay(1);

	/* reset */
	drv_flash_AmdReset(&flash_info);

	Drv_Flash_IntUnlock(int_level);

	id = (((GT_UI32)dev_id3<<24)
			| ((GT_UI32)dev_id2<<16)
			| ((GT_UI32)dev_id<<8)
			| ((GT_UI32)man_id));

	return id;
}


/******************************************************************************
 * FUNCTION NAME:
 *		DRV_FLASH_SetFlashType
 * DESCRIPTION:
 *		set flash info, used for flash debug.
 * INPUT:
 *		flash_info : flash information to be set.
 * OUTPUT:
 *		None
 * RETURN:
 *		TRUE   : set success.
 *      FALSE  : set fail.
 * NOTES:
 *		NONE
 * HISTORY:
 *		2008.03.03		Panda Xiong		Create
******************************************************************************/
GT_BOOL DRV_FLASH_SetFlashType
(
	IN const FLASH_INFO_T *flash_info
)
{
	GT_UI32		loop;
	GT_I32		flash_info_id = -1;
	GT_I32		int_level;

	if (flash_info == NULL)
	{
		return GT_FALSE;
	}

	for (loop = 0; loop < sizeof(allFlashInfo)/sizeof(allFlashInfo[0]); loop++)
	{
		if (flash_info->baseAddr == allFlashInfo[loop].baseAddr)
		{
			flash_info_id = loop;
		}
	}

	if ((flash_info_id != -1)
			&& (flash_info_id < sizeof(allFlashInfo)/sizeof(allFlashInfo[0])))
	{
		allFlashInfo[flash_info_id].type       = flash_info->type;
		allFlashInfo[flash_info_id].size       = flash_info->size;
		allFlashInfo[flash_info_id].blockSize  = flash_info->blockSize;
		allFlashInfo[flash_info_id].byteMode   = flash_info->byteMode;
		allFlashInfo[flash_info_id].bufferMode = flash_info->bufferMode;

		/* reset flash */
		if (flash_info->type == INTEL_TYPE)
		{
		    int_level = Drv_Flash_IntLock();
		    Drv_Flash_IO_WriteByte(flash_info->baseAddr, Intel_Flash_Cmd_ClearStatus);
		    Drv_Flash_IO_WriteByte(flash_info->baseAddr, Intel_Flash_Cmd_Reset);
		    Drv_Flash_IntUnlock(int_level);
		}
		else
		{
		    int_level = Drv_Flash_IntLock();
		    drv_flash_AmdReset(flash_info);
		    Drv_Flash_IntUnlock(int_level);
		}

		return GT_TRUE;
	}

	return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      DRV_FLASH_GetFlashInfo
 * DESCRIPTION:
 *      Get flash information.
 * INPUT:
 *      None
 * OUTPUT:
 *      flash_num : total flash numbers.
 * RETURN:
 *      Flash information.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
FLASH_INFO_T *DRV_FLASH_GetFlashInfo
(
	OUT GT_UI32 *flash_num
)
{
	if (flash_num == NULL)
	{
		return NULL;
	}

	*flash_num = COUNT_OF(allFlashInfo);
	return (FLASH_INFO_T *)allFlashInfo;
}


/******************************************************************************
 * FUNCTION NAME:
 *      DRV_FLASH_Erase
 * DESCRIPTION:
 *      Erase flash API.
 * INPUT:
 *      eraseAddr   : the flash start address to be erased,
 *                    it will be auto alligned to 64KB/128KB, then do erase.
 *      eraseLen    : erase flash length.
 *      displayFunc : callback display API,
 *                    if don't need to callback display, leave this as NULL.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE / GT_FALSE : Erase Success / Fail.
 * NOTES:
 *      1. This API only do flash erase.
 *      2. The eraseAddr must be the flash address,
 *         else, the result is unknown.
 *      3. If the eraseLen is less than 64KB/128KB,
 *         it will auto erase 64KB for AMD Type flash,
 *         and 128KB for Intel Type flash.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL DRV_FLASH_Erase
(
	IN GT_UI8			*eraseAddr,
	IN GT_UI32			eraseLen,
	IN DISPLAY_CALLBACK	displayFunc
)
{
	GT_BOOL 	ret;

    if (eraseAddr == NULL)
    {
        return GT_FALSE;
    }

	/* if erase length is 0, no need to erase */
    if (eraseLen == 0)
    {
    	return GT_TRUE;
    }

	FLASH_LOCK();
	ret = drv_flash_Erase(eraseAddr, eraseLen, displayFunc);
	FLASH_UNLOCK();

	return ret;
}


/******************************************************************************
 * FUNCTION NAME:
 *      DRV_FLASH_Write
 * DESCRIPTION:
 *      Write flash API.
 * INPUT:
 *      writeAddr   : the flash start address to be written.
 *      dataAddr    : data start address to be written.
 *      dataLen     : the length of data to be written.
 *      displayFunc : callback display API,
 *                    if don't need to callback display, leave this as NULL.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE / GT_FALSE : Write Success / Fail.
 * NOTES:
 *      1. This API only do flash write.
 *      2. The writeAddr must be the flash address,
 *         else, the result is unknown.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL DRV_FLASH_Write
(
	IN       GT_UI8 		*writeAddr,
	IN const GT_UI8 		*dataAddr,
	IN       GT_UI32 		 dataLen,
	IN DISPLAY_CALLBACK		 displayFunc
)
{
	GT_BOOL 	ret;

    if ((dataAddr == NULL) || (writeAddr == NULL))
    {
        return GT_FALSE;
    }

	/* if data length is 0, no need to write */
    if (dataLen == 0)
    {
    	return GT_TRUE;
    }

	FLASH_LOCK();
	ret = drv_flash_Write(writeAddr, dataAddr, dataLen, displayFunc);
	FLASH_UNLOCK();

	return ret;
}


/******************************************************************************
 * FUNCTION NAME:
 *      DRV_FLASH_Program
 * DESCRIPTION:
 *      Program flash API.
 * INPUT:
 *      writeAddr   : the flash start address to be programmed.
 *      dataAddr    : data start address to be programmed.
 *      dataLen     : the length of data to be programmed.
 *      displayFunc : callback display API,
 *                    if don't need to callback display, leave this as NULL.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE / GT_FALSE : Program Success / Fail.
 * NOTES:
 *      1. This API will auto do flash erase, if this flash is not be erased.
 *      2. The writeAddr must be the flash address,
 *         else, the result is unknown.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL DRV_FLASH_Program
(
	IN       GT_UI8 		*writeAddr,
	IN const GT_UI8 		*dataAddr,
	IN       GT_UI32 		 dataLen,
	IN DISPLAY_CALLBACK		 displayFunc
)
{
	GT_BOOL 	ret;

    if ((dataAddr == NULL) || (writeAddr == NULL))
    {
        return GT_FALSE;
    }

	/* if data length is 0, no need to program */
    if (dataLen == 0)
    {
    	return GT_TRUE;
    }

	FLASH_LOCK();

	autoProgramStage = AUTO_PROGRAM_ERASE_STAGE;	 /* set erase stage flag */
	ret = drv_flash_Erase(writeAddr, dataLen, displayFunc);
	if (ret)
	{
		autoProgramStage = AUTO_PROGRAM_WRITE_STAGE; /* set write stage flag */
		ret = drv_flash_Write(writeAddr, dataAddr, dataLen, displayFunc);
	}
	autoProgramStage = AUTO_PROGRAM_EXIT;	/* set auto program exit flag */

	FLASH_UNLOCK();

	return ret;
}


/******************************************************************************
 * FUNCTION NAME:
 *      DRV_FLASH_Init
 * DESCRIPTION:
 *      Init flash API.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE / GT_FALSE : Init Success / Fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL DRV_FLASH_Init(void)
{
	/* if no flash defined, return GT_FALSE */
	if (COUNT_OF(allFlashInfo) <= 0)
	{
		return GT_FALSE;
	}

	FLASH_SEM_INIT();

	return GT_TRUE;
}

#endif

