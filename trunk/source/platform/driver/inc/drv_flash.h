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
 *     and, it supports all x8 and x16 type flash.
 *     including AMD, INTEL, ATMEL, SST, SPANSION, EON, FUJISTU, and others.
 *
 * History:
 *    2007.03.27    Panda Xiong       Create
 *
 * Porting Guide:
 *   This is the hardware layer driver of flash, but it's very easy to porting.
 *   If you want to port this flash driver to your project, you just only need
 *    to modify the porting part of this header file, no need to modify the c
 *    file;
 *   And, don't forget to change the include files to be included.
******************************************************************************/

#ifndef __DRV_FLASH_H
#define __DRV_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>
#include <inc/boot_define.h>
#include <inc/delay.h>
#include <inc/cpu_io.h>

#define AMD_FLASH_SUPPORT		1
#define INTEL_FLASH_SUPPORT		1

#if (!AMD_FLASH_SUPPORT && !INTEL_FLASH_SUPPORT)
  #error "drv_flash.h : No flash type supported!"
#endif

/******************************************************************************
 *  Porting Part of Flash driver :
*******************************************************************************/
/* define all flashs on board,
 * the format must follow the structure FLASH_INFO_T */
#define DRV_FLASH_AllOnBoard				                       \
{											                       \
	{                                                              \
		(GT_UI8 *)(FLASH_BASE_ADDR),   /* flash base address */    \
		INTEL_TYPE,                    /* flash type         */    \
		FLASH_SIZE,                    /* flash size         */    \
		KB(128),                       /* per sector size    */    \
		GT_FALSE,                      /* byte mode          */    \
		GT_TRUE                        /* buffer mode        */    \
	},                                                             \
}

/* for different CPU, this need to be changed,
 * but please be very careful, cause this may cause system crash in some conditions. */
#define Drv_Flash_IO_ReadByte(addr)				(GT_UI8)CPU_IO_ReadByte((GT_UI8 *)(addr))
#define Drv_Flash_IO_WriteByte(addr, value)		CPU_IO_WriteByte((GT_UI8 *)(addr), (GT_UI8)(value))
#define Drv_Flash_IO_ReadHalfWord(addr)			(GT_UI16)CPU_IO_ReadHalfWord((GT_UI16 *)(addr))
#define Drv_Flash_IO_WriteHalfWord(addr, value)	CPU_IO_WriteHalfWord((GT_UI16 *)(addr), (GT_UI16)(value))
#define Drv_Flash_IO_ReadWord(addr)				(GT_UI32)CPU_IO_ReadWord((GT_UI32 *)(addr))
#define Drv_Flash_IO_WriteWord(addr, value)		CPU_IO_WriteWord((GT_UI32 *)(addr), (GT_UI32)(value))

#define Drv_Flash_IntLock()             1
#define Drv_Flash_IntUnlock(x)          NO_WARNING(x)

/* define display percent callback */
#define CALLBACK_STEP					1
typedef void (*DISPLAY_CALLBACK)(IN const GT_UI32 percent);


/******************************************************************************
 * In most cases, the below lines has no need to be modified,
 *  unless you know it very much.
 ******************************************************************************/
typedef enum
{
	UNKNOW_TYPE = 0,
	AMD_TYPE,
	INTEL_TYPE
} FLASH_TYPE;

typedef struct
{
	GT_UI8		*baseAddr;		/* flash base address         */
	FLASH_TYPE	type;			/* flash type                 */
	GT_UI32		size;			/* flash size                 */
	GT_UI32		blockSize;		/* per block size             */
	GT_BOOL  	byteMode;		/* byte mode                  */
	GT_BOOL	    bufferMode;		/* intel flash only supported */
} FLASH_INFO_T;


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
);

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
);

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
);

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
);

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
);

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
);

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
GT_BOOL DRV_FLASH_Init(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __DRV_FLASH_H */

