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

#include <inc/cli_api.h>
#include <inc/cmd_app.h>
#include <inc/cpu_watchdog.h>
#include <inc/delay.h>
#include <inc/boot_define.h>
#include <inc/reg.h>
#include <inc/cpu_io.h>
#include <inc/drv_flash.h>
#include <inc/arm_int_ctrl.h>
#include <image_define.h>
#include <inc/drv_image.h>
#include <xmodem/inc/xmodem_port.h>
#include <inc/cpu_int.h>


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
static CLI_REG_CMD_RETURN_T app_cmd_Reboot
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    UTL_PRT_Printf("\n\rPlease wait, system rebooting ...");

    /* disable CPU global interrupt */
    CPU_INT_SetGlobalState(GT_FALSE);

    CPU_WatchDog_Set(GT_TRUE);

    while (1)
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
static CLI_REG_CMD_RETURN_T app_cmd_TestKey
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_I32   key_value;

    if (n_param > 1)
    {
        UTL_PRT_Printf("\n\rUnknown parameter.");
        return CLI_PRT_FAIL;
    }

    UTL_PRT_Printf("\n\r Warning : The system will enter dead loop for test key,"
                   "\n\r           the only way to exit this loop is hardware reset."
                   "\n\r"
                   "\n\r Press 'Y' to continue, or 'N' to quit. ");

    while (1)
    {
        while ((key_value = UTL_VT_GetChar(1)) == -1)
        {}

        if ((key_value == 'y') || (key_value == 'Y'))
        {
            UTL_PRT_Printf("%c \n\r", key_value);
            break;
        }
        else if ((key_value == 'n') || (key_value == 'N'))
        {
            UTL_PRT_Printf("%c \n\r", key_value);
            return CLI_PRT_NONE;
        }
        else
        {}
    }

    while (1)
    {
        while ((key_value = UTL_VT_GetChar(1)) == -1)
        {}

        UTL_PRT_Printf("0x%X   ", key_value);
    }

    return CLI_PRT_NONE;
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
static CLI_REG_CMD_RETURN_T app_cmd_Jump
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    typedef void (*JUMP_FUNC)(void);

    GT_UI32   addr;
    JUMP_FUNC func;

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    addr = ATOI(param[1]);

	if (!IS_ADDR_FLASH_AREA(addr, 1) && !IS_ADDR_RAM_AREA(addr, 1))
	{
		UTL_PRT_Printf("\n\rInvalid address.");
		return CLI_PRT_FAIL;
	}

    UTL_PRT_Printf("\n\r");
    func = (JUMP_FUNC)(addr);

    CPU_WatchDog_Set(GT_FALSE);

    /* call this function, can't return */
    (*func)();

    /* never reach here */
    return CLI_PRT_NONE;
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
static CLI_REG_CMD_RETURN_T app_cmd_ShowMem32
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  start, end;
    GT_I32   n;
    GT_UI8  *p = NULL;

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    start = ATOI(param[1]);

    if (start & 3)
    {
        UTL_PRT_Printf("\n\rInvalid start address.");
        return CLI_PRT_FAIL;
    }

    if (n_param > 2)
    {
        if (param[2][0] == '-')
        {
            if (n_param > 3)
            {
                end = ATOI(param[3]);
            }
            else
            {
                UTL_PRT_Printf("\n\rUnknown end address.");
                return CLI_PRT_FAIL;
            }
        }
        else
        {
            end = ATOI(param[2]);
            end = start + end*4;
        }
    }
    else
    {
        end = start + 256 - 4;
    }

    if (end & 3)
    {
        UTL_PRT_Printf("\n\rInvalid end address.");
        return CLI_PRT_FAIL;
    }

    if (start > end)
    {
        UTL_PRT_Printf("\n\rError: end address must larger than start address.");
        return CLI_PRT_FAIL;
    }

    n = 0;
    for (; start<=end; start+=4)
    {
        if (!n)
        {
            p = (GT_UI8 *)start;
            UTL_PRT_Printf("\n\r%08X :", start);
        }

        UTL_PRT_Printf(" %08X", (VP32(start)));

        n++;
        n = n&3;

        if (!n)
        {
            UTL_PRT_Printf("   ");
            for (n=0; n<16; n++)
            {
                if ((p) && ((*p) >= ' ') && ((*p)<='~'))
                {
                    UTL_PRT_Printf("%c", *p);
                    p++;
                }
                else
                {
                    UTL_PRT_Printf("%c", '.');
                }
            }
            n = 0;
        }
    }

    if (n)
    {
        end = (GT_UI32)n*4;
        UTL_PRT_Printf("   ");
        for (n=0; n<end; n++)
        {
            if ((p) && ((*p) >= ' ') && ((*p)<='~'))
            {
                UTL_PRT_Printf("%c", *p);
                p++;
            }
            else
            {
                UTL_PRT_Printf("%c", '.');
            }
        }
    }

    return CLI_PRT_NONE;
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
static CLI_REG_CMD_RETURN_T app_cmd_ShowMem16
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  start, end;
    GT_I32   n;
    GT_UI8  *p = NULL;

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    start = ATOI(param[1]);

    if (start & 3)
    {
        UTL_PRT_Printf("\n\rInvalid start address.");
        return CLI_PRT_FAIL;
    }

    if (n_param > 2)
    {
        if (param[2][0] == '-')
        {
            if (n_param > 3)
            {
                end = ATOI(param[3]);
            }
            else
            {
                UTL_PRT_Printf("\n\rInvalid address.");
                return CLI_PRT_FAIL;
            }
        }
        else
        {
            end = ATOI(param[2]);
            end = start + end*4;
        }
    }
    else
    {
        end = start + 256 - 4;
    }

    if (end & 3)
    {
        UTL_PRT_Printf("\n\rInvalid end address.");
        return CLI_PRT_FAIL;
    }

    if (start > end)
    {
        UTL_PRT_Printf("\n\rError: end address must larger than start address.");
        return CLI_PRT_FAIL;
    }

    n = 0;
    for (;start<=end; start+=2)
    {
        if (!n)
        {
            p = (GT_UI8 *)start;
            UTL_PRT_Printf("\n\r%08X :", start);
        }

        UTL_PRT_Printf(" %04X", (VP16(start)));

        n++;
        n = n&0x7;

        if (!n)
        {
            UTL_PRT_Printf("   ");
            for (n=0; n<16; n++)
            {
                if ((p) && ((*p) >= ' ') && ((*p)<='~'))
                {
                    UTL_PRT_Printf("%c", *p);
                    p++;
                }
                else
                {
                    UTL_PRT_Printf("%c", '.');
                }
            }
            n = 0;
        }

    }

    if (n)
    {
        end = (GT_UI32)n*2;
        UTL_PRT_Printf("   ");
        for (n=0; n<end; n++)
        {
            if ((p) && ((*p) >= ' ') && ((*p)<='~'))
            {
                UTL_PRT_Printf("%c", *p);
                p++;
            }
            else
            {
                UTL_PRT_Printf("%c", '.');
            }
        }
    }

    return CLI_PRT_NONE;
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
static CLI_REG_CMD_RETURN_T app_cmd_ShowMem8
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  start, end;
    GT_I32   n;
    GT_UI8  *p = NULL;

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    start = ATOI(param[1]);

    if (start & 3)
    {
        UTL_PRT_Printf("\n\rInvalid start address.");
        return CLI_PRT_FAIL;
    }

    if (n_param > 2)
    {
        if (param[2][0] == '-')
        {
            if (n_param > 3)
            {
                end = ATOI(param[3]);
            }
            else
            {
                UTL_PRT_Printf("\n\rInvalid address.");
                return CLI_PRT_FAIL;
            }
        }
        else
        {
            end = ATOI(param[2]);
            end = start + end*4;
        }
    }
    else
    {
        end = start + 256 - 4;
    }

    if (end & 3)
    {
        UTL_PRT_Printf("\n\rInvalid end address.");
        return CLI_PRT_FAIL;
    }

    if (start > end)
    {
        UTL_PRT_Printf("\n\rError: end address must larger than start address.");
        return CLI_PRT_FAIL;
    }

    n = 0;
    for (; start<=end; start++)
    {
        if (!n)
        {
            p = (GT_UI8 *)start;
            UTL_PRT_Printf("\n\r%08X :",start);
        }

        UTL_PRT_Printf(" %02X",(VP8(start)));

        n++;
        n = n&0xF;

        if (!n)
        {
            UTL_PRT_Printf("   ");
            for (n=0; n<16; n++)
            {
                if ((p) && ((*p) >= ' ') && ((*p)<='~'))
                {
                    UTL_PRT_Printf("%c", *p);
                    p++;
                }
                else
                {
                    UTL_PRT_Printf("%c", '.');
                }
            }
            n = 0;
        }

    }

    if (n)
    {
        end = (GT_UI32)n;
        UTL_PRT_Printf("   ");
        for (n=0; n<end; n++)
        {
            if ((p) && ((*p) >= ' ') && ((*p)<='~'))
            {
                UTL_PRT_Printf("%c", *p);
                p++;
            }
            else
            {
                UTL_PRT_Printf("%c", '.');
            }
        }
    }

    return CLI_PRT_NONE;
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
static CLI_REG_CMD_RETURN_T app_cmd_Write32
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32   addr, value, repeat, i, k;
    GT_UI8   *p;

    if (n_param < 3)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    addr = ATOI(param[1]);

    for (i=2; i<n_param; i++)
    {
        p = strchr(param[i], '(');
        if (p)
        {
            repeat = ATOI(p+1);
            *p= 0;
        }
        else
            repeat = 1;

        value = ATOI(param[i]);
        for (k=0; k<repeat; k++)
        {
            (VP32(addr)) = value;
            addr += 4;
        }
    }

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_Write16
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  addr, value, repeat, i, k;
    GT_UI8  *p;

    if (n_param < 3)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    addr = ATOI(param[1]);

    for (i=2; i<n_param; i++)
    {
        p = strchr(param[i], '(');
        if (p)
        {
            repeat = ATOI(p+1);
            *p= 0;
        }
        else
            repeat = 1;

        value = ATOI(param[i]);
        for (k=0; k<repeat; k++)
        {
            (VP16(addr)) = (GT_UI16)value;
            addr += 2;
        }
    }

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_Write8
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32    addr, value, repeat, i, k;
    GT_UI8    *p;

    if (n_param < 3)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    addr = ATOI(param[1]);

    for (i=2; i<n_param; i++)
    {
        p = strchr(param[i], '(');
        if (p)
        {
            repeat = ATOI(p+1);
            *p= 0;
        }
        else
            repeat = 1;

        value = ATOI(param[i]);
        value = value & 0xff;
        for (k=0; k<repeat; k++)
        {
            (VP8(addr)) = (GT_UI8)value;
            addr++;
        }
    }

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_Memset
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  addr, val, len;

    if (n_param < 4)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    addr = ATOI(param[1]);
    val  = ATOI(param[2]) & 0xff;
    len  = ATOI(param[3]);

	if (IS_ADDR_FLASH_AREA(addr, len))
	{
		UTL_PRT_Printf("\n\rCan't write flash area.");
		return CLI_PRT_FAIL;
	}

    MEMSET((GT_UI8 *)addr, val, len);

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_Copy
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  addr_1, addr_2, len;

    if (n_param < 4)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    addr_1 = ATOI(param[1]);
    addr_2 = ATOI(param[2]);
    len    = ATOI(param[3]);

    if (IS_ADDR_RAM_AREA(addr_2, len))
    {
		UTL_PRT_Printf("\n\rMemory Copying .....................   0 %%");
        memcpy((GT_UI8 *)addr_2, (GT_UI8 *)addr_1, len);
		UTL_PRT_Display_Callback(100);
    }
    else if (IS_ADDR_FLASH_AREA(addr_2, len))
    {
        UTL_PRT_Printf("\n\rCopying 0x%XBytes from 0x%08X to 0x%08X .........  00 %%",
                   len, addr_1, addr_2);
        if (!DRV_FLASH_Program((GT_UI8 *)addr_2,
                               (GT_UI8 *)addr_1,
                               len,
                               UTL_PRT_Display_Callback))
        {
            return CLI_PRT_FAIL;
        }
		UTL_PRT_Display_Callback(100);
    }
	else
	{
		UTL_PRT_Printf("\n\rInvalid dest address.");
		return CLI_PRT_FAIL;
	}

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_Download
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  image_size;

    if (!UTL_VT_SetUartMode(UTL_VT_UART_POLLING_MODE))
    {
        UTL_PRT_Printf("\n\rSwitch uart to polling mode fail!");
        return CLI_PRT_FAIL;
    }

	UTL_PRT_Printf("\n\rxModem Downloading Image ................. ");
	image_size = xModem_Download((GT_UI8 *)LOADER_TEXT_BASE, IMAGE_LOADER_SIZE);
    if (image_size == 0)
	{
		UTL_PRT_Printf("Fail!");
		return CLI_PRT_FAIL;
    }
	UTL_PRT_Printf("100 %%");

    if (!UTL_VT_SetUartMode(UTL_VT_UART_INTERRUPT_MODE))
    {
        UTL_PRT_Printf("\n\rRestore uart to interrupt mode fail!");
        return CLI_PRT_FAIL;
    }

	switch (DRV_Image_GetImageType((GT_UI8 *)LOADER_TEXT_BASE))
	{
	    case DRV_IMAGE_LOADER:
	        UTL_PRT_Printf("\n\rProgramming BootLoader Image .............   0 %%");
			if (!DRV_FLASH_Program((GT_UI8 *)IMAGE_LOADER_BASE,
								   (GT_UI8 *)LOADER_TEXT_BASE,
								   image_size,
								   UTL_PRT_Display_Callback))
			{
                UTL_PRT_Printf("\n\rFail!");
			}
	        break;

	    case DRV_IMAGE_KERNEL:
	        UTL_PRT_Printf("\n\rProgramming Kernel Image .................   0 %%");
			if (!DRV_FLASH_Program((GT_UI8 *)IMAGE_KERNEL_BASE,
								   (GT_UI8 *)LOADER_TEXT_BASE,
								   image_size,
								   UTL_PRT_Display_Callback))
			{
				UTL_PRT_Printf("\n\rFail!");
			}
	        break;

	    default:
	        UTL_PRT_Printf("\n\rError : Invalid Image!");
	        return CLI_PRT_FAIL;
	}

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_Upload
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
	GT_UI32  addr = 0;
	GT_UI32  len;

	if (n_param < 3)
	{
		UTL_PRT_Printf("\n\rmissing parameter.");
		return CLI_PRT_FAIL;
	}

	if ((param[1][0] < '0') || (param[1][0] > '9'))
	{
		UTL_PRT_Printf("\n\rInvalid address!");
		return CLI_PRT_FAIL;
	}

	if ((param[2][0] < '0') || (param[2][0] > '9'))
	{
		UTL_PRT_Printf("\n\rInvalid address!");
		return CLI_PRT_FAIL;
	}

	addr = ATOI(param[1]);
	len  = ATOI(param[2]);

	if (!IS_ADDR_FLASH_AREA(addr, len) && !IS_ADDR_RAM_AREA(addr, len))
	{
		UTL_PRT_Printf("\n\rInvalid address.");
		return CLI_PRT_FAIL;
	}

	UTL_PRT_Printf("\n\rxModem Uploading ................. ");
    if (!xModem_Upload((GT_UI8 *)addr, len))
	{
		UTL_PRT_Printf("Fail!");
		return CLI_PRT_FAIL;
    }

	UTL_PRT_Printf("100 %%");
    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_DebugFlash
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    FLASH_INFO_T    *flash_info;
    GT_UI32         flash_num, loop;
    GT_UI32         addr, len;
	FLASH_INFO_T	flash_info_input;
	GT_UI8			buf[100];

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter.");
        return CLI_PRT_FAIL;
    }

    if (strcmp(param[1], (const GT_UI8 *)"init") == 0)
    {
        return (DRV_FLASH_Init()? CLI_PRT_SUCCESS : CLI_PRT_FAIL);
    }
	else if (strcmp(param[1], (const GT_UI8 *)"config") == 0)
	{
		memset(&flash_info_input, 0x0, sizeof(flash_info_input));

		UTL_PRT_Printf("\n\r Flash Addr            : ");
		memset(buf, 0x0, sizeof(buf));
		if (!CLI_IO_ReadLine(sizeof(buf), buf))
		{
			return CLI_PRT_FAIL;
		}
		flash_info_input.baseAddr = (GT_UI8 *)ATOI(buf);

		while (1)
		{
			UTL_PRT_Printf("\n\r Flash Type (A/I)      : ");
			memset(buf, 0x0, sizeof(buf));
			if (!CLI_IO_ReadLine(sizeof(buf), buf))
			{
				return CLI_PRT_FAIL;
			}

			if ((strcmp(buf, (const GT_UI8 *)"A") == 0)
				|| (strcmp(buf, (const GT_UI8 *)"a") == 0))
			{
				flash_info_input.type = AMD_TYPE;
				break;
			}
			else if ((strcmp(buf, (const GT_UI8 *)"I") == 0)
					|| (strcmp(buf, (const GT_UI8 *)"i") == 0))
			{
				flash_info_input.type = INTEL_TYPE;
				break;
			}
		}

		UTL_PRT_Printf("\n\r Flash Size (KB)       : ");
		memset(buf, 0x0, sizeof(buf));
		if (!CLI_IO_ReadLine(sizeof(buf), buf))
		{
			return CLI_PRT_FAIL;
		}
		flash_info_input.size = ATOI(buf) * KB(1);

		UTL_PRT_Printf("\n\r Sector Size (KB)      : ");
		memset(buf, 0x0, sizeof(buf));
		if (!CLI_IO_ReadLine(sizeof(buf), buf))
		{
			return CLI_PRT_FAIL;
		}
		flash_info_input.blockSize = ATOI(buf) * KB(1);

		while (1)
		{
			UTL_PRT_Printf("\n\r Byte Mode (T/F)       : ");
			memset(buf, 0x0, sizeof(buf));
			if (!CLI_IO_ReadLine(sizeof(buf), buf))
			{
				return CLI_PRT_FAIL;
			}

			if ((strcmp(buf, (const GT_UI8 *)"T") == 0)
					|| (strcmp(buf, (const GT_UI8 *)"t") == 0))
			{
				flash_info_input.byteMode = GT_TRUE;
				break;
			}
			else if ((strcmp(buf, (const GT_UI8 *)"F") == 0)
					|| (strcmp(buf, (const GT_UI8 *)"f") == 0))
			{
				flash_info_input.byteMode = GT_FALSE;
				break;
			}
		}

		while (1)
		{
			UTL_PRT_Printf("\n\r Buffer Mode (T/F)     : ");
			memset(buf, 0x0, sizeof(buf));
			if (!CLI_IO_ReadLine(sizeof(buf), buf))
			{
				return CLI_PRT_FAIL;
			}

			if ((strcmp(buf, (const GT_UI8 *)"T") == 0)
					|| (strcmp(buf, (const GT_UI8 *)"t") == 0))
			{
				flash_info_input.bufferMode = GT_TRUE;
				break;
			}
			else if ((strcmp(buf, (const GT_UI8 *)"F") == 0)
					|| (strcmp(buf, (const GT_UI8 *)"f") == 0))
			{
				flash_info_input.bufferMode = GT_FALSE;
				break;
			}
		}

		if (DRV_FLASH_SetFlashType(&flash_info_input))
		{
			return CLI_PRT_SUCCESS;
		}
		else
		{
			return CLI_PRT_FAIL;
		}
	}
    else if (strcmp(param[1], (const GT_UI8 *)"info") == 0)
    {
        flash_info = DRV_FLASH_GetFlashInfo(&flash_num);
        if (flash_info != NULL)
        {
            for (loop = 0; loop < flash_num; loop++)
            {
                UTL_PRT_Printf(
                        "\n\r Flash %d Information:"
                        "\n\r   base address   : 0x%08X"
                        "\n\r   flash type     : %s"
                        "\n\r   size           : %dKB"
                        "\n\r   block size     : %dKB"
                        "\n\r   byte mode      : %s"
                        "\n\r   buffer mode    : %s"
                        "\n\r",
                        loop+1,
                        (GT_UI32)(flash_info[loop].baseAddr),
                        ((flash_info[loop].type == INTEL_TYPE)? "Intel Type" : "AMD Type"),
                        flash_info[loop].size / KB(1),
                        flash_info[loop].blockSize / KB(1),
                        (flash_info[loop].byteMode ? "TRUE" : "FALSE"),
                        (flash_info[loop].bufferMode ? "TRUE" : "FALSE"));
            }
        }

        return CLI_PRT_NONE;
    }
    else if (strcmp(param[1], (const GT_UI8 *)"erase") == 0)
    {
        if (n_param < 4)
        {
            UTL_PRT_Printf("\n\rMissing parameter.");
            return CLI_PRT_FAIL;
        }

        addr = ATOI(param[2]);
        len  = ATOI(param[3]);

        UTL_PRT_Printf("\n\rErasing 0x%XBytes start from 0x%08X ...............  00 %%", len, addr);
        if (!DRV_FLASH_Erase((GT_UI8 *)addr, len, UTL_PRT_Display_Callback))
        {
            return CLI_PRT_FAIL;
        }
        else
        {
            return CLI_PRT_SUCCESS;
        }
    }
	else if (strcmp(param[1], (const GT_UI8 *)"detect") == 0)
	{
		GT_UI32		id;
		GT_BOOL		byte_mode;

        if (n_param < 4)
        {
            UTL_PRT_Printf("\n\rMissing parameter.");
            return CLI_PRT_FAIL;
        }

        addr = ATOI(param[2]);

		if (!IS_ADDR_FLASH_AREA(addr, 1))
		{
			UTL_PRT_Printf("\n\rInvalid dest address.");
			return CLI_PRT_FAIL;
		}

		if ((strcmp(param[3], (const GT_UI8 *)"T") == 0)
			|| (strcmp(param[3], (const GT_UI8 *)"t") == 0))
		{
			byte_mode = GT_TRUE;
		}
		else if ((strcmp(param[3], (const GT_UI8 *)"F") == 0)
				|| (strcmp(param[3], (const GT_UI8 *)"f") == 0))
		{
			byte_mode = GT_FALSE;
		}
		else
		{
			UTL_PRT_Printf("\n\rInvalid byte mode.");
			return CLI_PRT_FAIL;
		}

		id = DRV_FLASH_AutoDetect((GT_UI8 *)addr, byte_mode);
		UTL_PRT_Printf("\n\rDetect Flash ID: 0x%08X"
					   "\n\r  Manu_ID   : 0x%02X"
					   "\n\r  Dev_ID_1  : 0x%02X"
					   "\n\r  Dev_ID_2  : 0x%02X"
					   "\n\r  Dev_ID_3  : 0x%02X",
					   id,
					   ((id >>  0) & 0xFF),
					   ((id >>  8) & 0xFF),
					   ((id >> 16) & 0xFF),
					   ((id >> 24) & 0xFF));

		return CLI_PRT_SUCCESS;
	}

    else
    {
        UTL_PRT_Printf("\n\rUnknown parameter.");
        return CLI_PRT_FAIL;
    }
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
static CLI_REG_CMD_RETURN_T app_cmd_IntCtrl
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  current_param = 1;
    GT_UI32  int_num;
    GT_UI32  loop;

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter!");
        return CLI_PRT_FAIL;
    }

    int_num = ATOI(param[current_param]);
    if ((int_num <= 0) || (int_num >= 32))
    {
        UTL_PRT_Printf("\n\rInterrupt Number error!");
        return CLI_PRT_FAIL;
    }

    current_param++;
    if (strcmp(param[current_param], (const GT_UI8 *)"enable") == 0)
    {
        (VP32(AIC_IECR)) = (1 << int_num); /* enable interrupt */
    }
    else if (strcmp(param[current_param], (const GT_UI8 *)"disable") == 0)
    {
        (VP32(AIC_IDCR)) = (1 << int_num); /* disable interrupt */
    }
    else if (strcmp(param[current_param], (const GT_UI8 *)"dump") == 0)
    {
        UTL_PRT_Printf("\n\rAIC Register Dump:");

        for (loop = 0; loop < 32; loop++)
        {
            UTL_PRT_Printf("\n\r  AIC_SMR%-2d  : 0x%08X",
                           loop, (VP32(AIC_SMR(loop))));
        }

        for (loop = 0; loop < 32; loop++)
        {
            UTL_PRT_Printf("\n\r  AIC_SVR%-2d  : 0x%08X",
                           loop, (VP32(AIC_SVR(loop))));
        }

        UTL_PRT_Printf("\n\r  AIC_IVR    : 0x%08X"
                       "\n\r  AIC_FVR    : 0x%08X"
                       "\n\r  AIC_ISR    : 0x%08X"
                       "\n\r  AIC_IPR    : 0x%08X"
                       "\n\r  AIC_IMR    : 0x%08X"
                       "\n\r  AIC_CISR   : 0x%08X"
                       "\n\r  AIC_SPU    : 0x%08X"
                       "\n\r  AIC_DCR    : 0x%08X"
                       "\n\r  AIC_FFSR   : 0x%08X",
                       (VP32(AIC_IVR)),
                       (VP32(AIC_FVR)),
                       (VP32(AIC_ISR)),
                       (VP32(AIC_IPR)),
                       (VP32(AIC_IMR)),
                       (VP32(AIC_CISR)),
                       (VP32(AIC_SPU)),
                       (VP32(AIC_DCR)),
                       (VP32(AIC_FFSR))
                       );

        return CLI_PRT_NONE;
    }
    else if (strcmp(param[current_param], (const GT_UI8 *)"test") == 0)
    {
        GT_IMAGE_HEADER *p_test = (GT_IMAGE_HEADER *)0;

        p_test->signature = 0xAA;
        p_test->checksum  = 0x55;
        return CLI_PRT_NONE;
    }
    else
    {
        UTL_PRT_Printf("\n\rUnknown parameter!");
        return CLI_PRT_FAIL;
    }

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_ClockCtrl
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter!");
        return CLI_PRT_FAIL;
    }

    if (strcmp(param[1], (const GT_UI8 *)"enable") == 0)
    {
        /* enable all peripherals clock */
        (VP32(PMC_PCER)) = ~0;
    }
    else if (strcmp(param[1], (const GT_UI8 *)"disable") == 0)
    {
        /* disable all peripherals clock */
        (VP32(PMC_PCDR)) = ~0;
    }
    else if (strcmp(param[1], (const GT_UI8 *)"dump") == 0)
    {
        UTL_PRT_Printf("\n\rClock Register Dump:"
                       "\n\r  PMC_SCSR   : 0x%08X"
                       "\n\r  PMC_PCSR   : 0x%08X"
                       "\n\r  CKGR_MOR   : 0x%08X"
                       "\n\r  CKGR_MCFR  : 0x%08X"
                       "\n\r  CKGR_PLLAR : 0x%08X"
                       "\n\r  CKGR_PLLBR : 0x%08X"
                       "\n\r  PMC_MCKR   : 0x%08X"
                       "\n\r  PMC_PCK0   : 0x%08X"
                       "\n\r  PMC_PCK1   : 0x%08X"
                       "\n\r  PMC_PCK2   : 0x%08X"
                       "\n\r  PMC_PCK3   : 0x%08X"
                       "\n\r  PMC_SR     : 0x%08X"
                       "\n\r  PMC_IMR    : 0x%08X",
                       (VP32(PMC_SCSR)),
                       (VP32(PMC_PCSR)),
                       (VP32(CKGR_MOR)),
                       (VP32(CKGR_MCFR)),
                       (VP32(CKGR_PLLAR)),
                       (VP32(CKGR_PLLBR)),
                       (VP32(PMC_MCKR)),
                       (VP32(PMC_PCK0)),
                       (VP32(PMC_PCK1)),
                       (VP32(PMC_PCK2)),
                       (VP32(PMC_PCK3)),
                       (VP32(PMC_SR)),
                       (VP32(PMC_IMR))
                       );

        return CLI_PRT_NONE;
    }
    else
    {
        UTL_PRT_Printf("\n\rUnknown parameter!");
        return CLI_PRT_FAIL;
    }

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_PioCtrl
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    /* total PIO count is 32 */
    #define TOTAL_PIO_COUNT     (32)

    GT_BOOL     b_all = GT_FALSE;
    GT_UI32     pio_base_addr;
    GT_UI32     pio_no = 0;
    GT_UI32     value;

    if (n_param < 3)
    {
        UTL_PRT_Printf("\n\rMissing parameter!");
        return CLI_PRT_FAIL;
    }

    if (strcmp(param[1], (const GT_UI8 *)"A") == 0)
    {
        pio_base_addr = PIOA_BASE_REG;
    }
    else if (strcmp(param[1], (const GT_UI8 *)"B") == 0)
    {
        pio_base_addr = PIOB_BASE_REG;
    }
    else if (strcmp(param[1], (const GT_UI8 *)"C") == 0)
    {
        pio_base_addr = PIOC_BASE_REG;
    }
    else if (strcmp(param[1], (const GT_UI8 *)"D") == 0)
    {
        pio_base_addr = PIOD_BASE_REG;
    }
    else
    {
        UTL_PRT_Printf("\n\rUnknown parameter!");
        return CLI_PRT_FAIL;
    }

    /* get PIO number */
    if (strcmp(param[2], (const GT_UI8 *)"all") == 0)
    {
        b_all = GT_TRUE;
    }
    else if (strcmp(param[2], (const GT_UI8 *)"dump") == 0)
    {
        UTL_PRT_Printf("\n\rPIO[%s] Register Dump:"
                       "\n\r  PIO_PSR    : 0x%08X"
                       "\n\r  PIO_OSR    : 0x%08X"
                       "\n\r  PIO_ODSR   : 0x%08X"
                       "\n\r  PIO_PDSR   : 0x%08X"
                       "\n\r  PIO_IMR    : 0x%08X"
                       "\n\r  PIO_ISR    : 0x%08X"
                       "\n\r  PIO_MDSR   : 0x%08X"
                       "\n\r  PIO_PUSR   : 0x%08X"
                       "\n\r  PIO_ABSR   : 0x%08X"
                       "\n\r  PIO_OWSR   : 0x%08X",
                       param[1],
                       (VP32(PIO_PSR(pio_base_addr))),
                       (VP32(PIO_OSR(pio_base_addr))),
                       (VP32(PIO_ODSR(pio_base_addr))),
                       (VP32(PIO_PDSR(pio_base_addr))),
                       (VP32(PIO_IMR(pio_base_addr))),
                       (VP32(PIO_ISR(pio_base_addr))),
                       (VP32(PIO_MDSR(pio_base_addr))),
                       (VP32(PIO_PUSR(pio_base_addr))),
                       (VP32(PIO_ABSR(pio_base_addr))),
                       (VP32(PIO_OWSR(pio_base_addr)))
                       );

        return CLI_PRT_NONE;
    }
    else
    {
        pio_no = ATOI(param[2]);

        if ((pio_no <= 0) || (pio_no >= TOTAL_PIO_COUNT))
        {
            UTL_PRT_Printf("\n\rUnknown PIO number!");
            return CLI_PRT_FAIL;
        }
    }

    if (strcmp(param[3], (const GT_UI8 *)"read") == 0)
    {
        if (b_all == GT_TRUE)
        {
            UTL_PRT_Printf("\n\rPIO[%s] All Read : 0x%08X",
                           param[1],
                           CPU_PIO_ReadBit(pio_base_addr, (~0)));
        }
        else
        {
            UTL_PRT_Printf("\n\rPIO[%s] [%d] Read : %d",
                           param[1],
                           pio_no,
                           CPU_PIO_ReadBit(pio_base_addr, pio_no));
        }

        return CLI_PRT_NONE;
    }
    else if (strcmp(param[3], (const GT_UI8 *)"write") == 0)
    {
        if (n_param < 4)
        {
            UTL_PRT_Printf("\n\rMissing parameter!");
            return CLI_PRT_FAIL;
        }

        value = ATOI(param[4]);
        if ((value != 0) && (value != 1))
        {
            UTL_PRT_Printf("\n\rUnknown write value!");
            return CLI_PRT_FAIL;
        }

        if (b_all == GT_TRUE)
        {
            CPU_PIO_WriteBit(pio_base_addr, (~0), value);

            UTL_PRT_Printf("\n\rPIO[%s] All Write : %d",
                           param[1], value);
        }
        else
        {
            CPU_PIO_WriteBit(pio_base_addr, pio_no, value);

            UTL_PRT_Printf("\n\rPIO[%s] [%d] Write : %d",
                           param[1], pio_no, value);
        }

        return CLI_PRT_SUCCESS;
    }
    else if (strcmp(param[3], (const GT_UI8 *)"int") == 0)
    {
        if (n_param < 4)
        {
            UTL_PRT_Printf("\n\rMissing parameter!");
            return CLI_PRT_FAIL;
        }

        if (strcmp(param[4], (const GT_UI8 *)"enable") == 0)
        {
            if (b_all == GT_TRUE)
            {
                (VP32(PIO_PER(pio_base_addr)))  = ~0;    /* PIO control enable */
                (VP32(PIO_ODR(pio_base_addr)))  = ~0;    /* PIO output disable */

                (VP32(PIO_IER(pio_base_addr)))  = ~0;    /* PIO input change interrupt enable */
                (VP32(PIO_PUER(pio_base_addr))) = ~0;    /* PIO pull-up resister enable       */

                UTL_PRT_Printf("\n\rPIO[%s] All Interrupt : Enable", param[1]);
            }
            else
            {
                (VP32(PIO_PER(pio_base_addr)))  = (1 << pio_no);    /* PIO control enable */
                (VP32(PIO_ODR(pio_base_addr)))  = (1 << pio_no);    /* PIO output disable */

                (VP32(PIO_IER(pio_base_addr)))  = (1 << pio_no);    /* PIO input change interrupt enable */
                (VP32(PIO_PUER(pio_base_addr))) = (1 << pio_no);    /* PIO pull-up resister enable       */

                UTL_PRT_Printf("\n\rPIO[%s] [%d] Interrupt : Enable",
                               param[1], pio_no);
            }
        }
        else if (strcmp(param[4], (const GT_UI8 *)"disable") == 0)
        {
            if (b_all == GT_TRUE)
            {
                (VP32(PIO_PER(pio_base_addr)))  = ~0;    /* PIO control enable */
                (VP32(PIO_ODR(pio_base_addr)))  = ~0;    /* PIO output disable */

                (VP32(PIO_IDR(pio_base_addr)))  = ~0;    /* PIO input change interrupt enable */
                (VP32(PIO_PUDR(pio_base_addr))) = ~0;    /* PIO pull-up resister disable      */

                UTL_PRT_Printf("\n\rPIO[%s] All Interrupt : Disable", param[1]);
            }
            else
            {
                (VP32(PIO_PER(pio_base_addr)))  = (1 << pio_no);    /* PIO control enable */
                (VP32(PIO_ODR(pio_base_addr)))  = (1 << pio_no);    /* PIO output disable */

                (VP32(PIO_IDR(pio_base_addr)))  = (1 << pio_no);    /* PIO input change interrupt disable */
                (VP32(PIO_PUDR(pio_base_addr))) = (1 << pio_no);    /* PIO pull-up resister disable       */

                UTL_PRT_Printf("\n\rPIO[%s] [%d] Interrupt : Disable",
                               param[1], pio_no);
            }
        }
        else
        {
            UTL_PRT_Printf("\n\rUnknown parameter!");
            return CLI_PRT_FAIL;
        }
    }
    else
    {
        UTL_PRT_Printf("\n\rUnknown parameter!");
        return CLI_PRT_FAIL;
    }

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_DBGUIntCtrl
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    extern void CPU_ISR_SYS_Handler(void);

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter!");
        return CLI_PRT_FAIL;
    }

    if (strcmp(param[1], (const GT_UI8 *)"enable") == 0)
    {
        if (!CPU_INT_SetInterrupt(CPU_INT_SRC_DBGU_UART, GT_TRUE))
        {
            UTL_PRT_Printf("\n\rEnable interrupt fail!");
            return CLI_PRT_FAIL;
        }

        /* enable DBGU Rx interrupt */
        (VP32(DBGU_IER)) = ((0x1 << 0) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7));
    }
    else if (strcmp(param[1], (const GT_UI8 *)"disable") == 0)
    {
        if (!CPU_INT_SetInterrupt(CPU_INT_SRC_DBGU_UART, GT_FALSE))
        {
            UTL_PRT_Printf("\n\rDisable interrupt fail!");
            return CLI_PRT_FAIL;
        }

        /* disable DBGU Rx interrupt */
        (VP32(DBGU_IDR)) = ((0x1 << 0) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7));
    }
    else if (strcmp(param[1], (const GT_UI8 *)"hook") == 0)
    {
        if (!CPU_INT_HookInterrupt(CPU_INT_SRC_DBGU_UART,
                                   CPU_INT_HIGHEST_AIC_PRIORITY,
                                   CPU_INT_SRC_TYPE_INT_LEVEL_SENSITIVE,
                                   DRV_SIO_UpdateBuffer))
        {
            UTL_PRT_Printf("\n\rHook DBGU ISR fail!");
            return CLI_PRT_FAIL;
        }
    }
    else
    {
        UTL_PRT_Printf("\n\rUnknown parameter!");
        return CLI_PRT_FAIL;
    }

    return CLI_PRT_SUCCESS;
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
static CLI_REG_CMD_RETURN_T app_cmd_rtcCtrl
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  current_param = 1;
    GT_UI32  current_val, old_val;

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter!");
        return CLI_PRT_FAIL;
    }

    if (strcmp(param[current_param], (const GT_UI8 *)"read") == 0)
    {
        old_val = (VP32(ST_CRTR));
        UTL_PRT_Printf("\n\rReal Time Clock value : (ESC to exit) "
                       "\n\r 0x%08X",
                       old_val);
        while (CLI_IO_ReadKey() != VT_KEY_ESC)
        {
            current_val = (VP32(ST_CRTR));
            if (current_val != old_val)
            {
                /* will be re-fresh once every 1 second */

                UTL_PRT_Printf("\b\b\b\b\b\b\b\b%08X", current_val);
                old_val = current_val;
            }
        }

        return CLI_PRT_NONE;
    }
    else
    {
        UTL_PRT_Printf("\n\rUnknown parameter!");
        return CLI_PRT_FAIL;
    }
}


static CLI_REG_CMD_T cmd_app[] =
{
    {(GT_UI8 *)"reboot",     app_cmd_Reboot,      (GT_UI8 *)"reboot (reboot system)"},
    {(GT_UI8 *)"testkey",    app_cmd_TestKey,     (GT_UI8 *)"testkey (test key)"},
    {(GT_UI8 *)"j",          app_cmd_Jump,        (GT_UI8 *)"j <address> (jump to 'address' and run)"},

    /* show memory */
    {(GT_UI8 *)"dd",         app_cmd_ShowMem32,   (GT_UI8 *)"dd <start> | dd <start> <number> | dd <start> - <end> (32bit show)"},
    {(GT_UI8 *)"dw",         app_cmd_ShowMem16,   (GT_UI8 *)"dw <start> | dw <start> <number> | dw <start> - <end> (16bit show)"},
    {(GT_UI8 *)"db",         app_cmd_ShowMem8,    (GT_UI8 *)"db <start> | db <start> <number> | db <start> - <end> ( 8bit show)"},

    /* write memory */
    {(GT_UI8 *)"ed",         app_cmd_Write32,     (GT_UI8 *)"ed <address> <value1> <value2> ... (write memory in 32bit)"},
    {(GT_UI8 *)"ew",         app_cmd_Write16,     (GT_UI8 *)"ew <address> <value1> <value2> ... (write memory in 16bit)"},
    {(GT_UI8 *)"eb",         app_cmd_Write8,      (GT_UI8 *)"eb <address> <value1> <value2> ... (write memory in  8bit)"},
    {(GT_UI8 *)"memset",     app_cmd_Memset,      (GT_UI8 *)"memset <address> <value> <len> (clear memory, only support sdram)"},

    /* copy len byte from address1 to address2 */
    {(GT_UI8 *)"copy",       app_cmd_Copy,        (GT_UI8 *)"copy <address_from> <address_to> <len> (support flash <-> sdram)"},
    {(GT_UI8 *)"flash",      app_cmd_DebugFlash,  (GT_UI8 *)"flash [init/config/info/erase/detect] {<address> <len>} (debug flash)"},
    {(GT_UI8 *)"download",   app_cmd_Download,    (GT_UI8 *)"download (xmodem download firmware)"},
    {(GT_UI8 *)"upload",     app_cmd_Upload,      (GT_UI8 *)"upload <address> <len> (xmodem upload firmware)"},

    {(GT_UI8 *)"int",        app_cmd_IntCtrl,     (GT_UI8 *)"int <number> [enable/disable/dump/test] (enable/disable/dump/test interrupt)"},
    {(GT_UI8 *)"clock",      app_cmd_ClockCtrl,   (GT_UI8 *)"clock [enable/disable/dump] (enable/disable/dump clock)"},
    {(GT_UI8 *)"pio",        app_cmd_PioCtrl,     (GT_UI8 *)"pio [A/B/C/D] [<number>/all/dump] [read / write [1/0] / int [enable/disable]] (pio control)"},

    {(GT_UI8 *)"dbgu",       app_cmd_DBGUIntCtrl, (GT_UI8 *)"dbgu [enable/disable/hook] (enable/disable/hook DBGU interrupt)"},
    {(GT_UI8 *)"rtc",        app_cmd_rtcCtrl,     (GT_UI8 *)"rtc [read] (read RTC)"},

};


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
GT_BOOL CMD_APP_RegisterCmd(void)
{
    GT_UI32  loop;

    for (loop = 0; loop < sizeof(cmd_app)/sizeof(cmd_app[0]); loop++)
    {
        if (!CLI_REG_RegisterCmd(&cmd_app[loop]))
        {
            UTL_PRT_Printf("\n\r%s(line%d): Register Command %s Fail!",
                           __FILE__, __LINE__, cmd_app[loop].cmd_name);
        }
    }

    return GT_TRUE;
}

