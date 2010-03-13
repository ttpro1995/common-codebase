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

#include <h/basetype.h>
#include <inc/boot_define.h>
#include <inc/delay.h>
#include <inc/cpu_watchdog.h>
#include <inc/drv_flash.h>
#include <inc/utl_tran.h>
#include <inc/utl_vt.h>
#include <inc/utl_prt.h>
#include <xmodem/inc/xmodem_port.h>
#include <inc/drv_image.h>
#include <inc/utl_lzma.h>

/* auto load kernel image timeout, ms */
#define AUTO_LOAD_KERNEL_IMAGE_TIMEOUT      (100)

#define PRINT_MENU  "\n\r BootLoader %s"                           \
                    "\n\r ***************************************" \
                    "\n\r  1. Test SDRAM."                         \
                    "\n\r  2. xModem Download Image."              \
                    "\n\r  3. Display Loader TEXT Data in SDRAM."  \
                    "\n\r  4. Display Kernel TEXT Data in SDRAM."  \
                    "\n\r  5. Display Xmodem Download Data."       \
                    "\n\r  6. Display Loader Data in Flash."       \
                    "\n\r  7. Display Kernel Data in Flash."       \
                    "\n\r  8. Load Kernel Image."                  \
                    "\n\r  9. Reset System."                       \
                    "\n\r ***************************************" \
                    "\n\r"


/* declare start stack space.
 *
 * here, should make it align to 4byte, so, use GT_UI32 to align it.
 */
GT_UI32	start_stack[BOOT_STACK_SIZE/sizeof(GT_UI32)];

#if !defined(__START_FROM_SRAM__)
/******************************************************************************
FUNCTION NAME:
    test_ram
DESCRIPTION:
    Test RAM
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
static GT_BOOL test_ram(GT_UI32 value)
{
    volatile GT_UI32  *addr;
    GT_UI32            read_data;

    for (addr=(GT_UI32 *)KERNEL_TEXT_BASE; (GT_UI32)addr<LOADER_TEXT_BASE; addr++)
    {
        (VP32(addr)) = value;

        read_data = (VP32(addr));

        if (read_data != value)
        {
            UTL_PRT_Printf("\n\rTest RAM fail : address=0x%08X, "
                           "WriteValue=0x%08X, ReadValue=0x%08X",
                           (GT_UI32)addr, value, read_data);

            return GT_FALSE;
        }
    }

    return GT_TRUE;
}

void display_memory(GT_UI32 start_addr, GT_UI32 length)
{
    GT_UI32      tmp_loop, tmp_count;
    GT_UI8       *addr;

    UTL_VT_ClearScreen();
    tmp_count = 0;
    for (addr = (GT_UI8 *)start_addr; (GT_UI32)addr < (start_addr+length); )
    {
        UTL_PRT_Printf("\n\r%08X :", (GT_UI32)addr);
        tmp_loop = 16;
        while (tmp_loop-- > 0)
        {
            UTL_PRT_Printf(" %02X", (VP8(addr)));
            addr++;
        }

        if (tmp_count++ == 10)
        {
            int     ch;

            tmp_count = 0;
            UTL_PRT_Printf("\n\r-- Press SPACE to continue, Or ESC to quit --");
            while (1)
            {
                while ((ch = UTL_VT_GetChar(1)) == -1)
                {}

                if (ch == VT_KEY_ESC)
                {
                    return;
                }
                else if (ch == VT_KEY_SPACE)
                {
                    break;
                }
            }
        }
    }
}
#endif

static void load_kernel_image(void)
{
	typedef void (*JUMP_FUNC)(void);

	GT_UI8      *sdram_addr = (GT_UI8 *)(KERNEL_TEXT_BASE);
	GT_UI8      *flash_addr = (GT_UI8 *)(IMAGE_KERNEL_BASE);
	JUMP_FUNC    entry_func = (JUMP_FUNC)(sdram_addr);
	GT_UI32      image_size, decode_buf_size;
    GT_UI8      *decode_buf;
    GT_I32       decompress_size;
    GT_UI32      offset;

    image_size = DRV_Image_GetImageSize(flash_addr);
    if (image_size == 0)
    {
        UTL_PRT_Printf("\n\rError : Invalid Kernel Image!");
        return;
    }

    switch (DRV_Image_GetCompressMode(flash_addr))
    {
        case DRV_IMAGE_COMPRESS_NONE:
            /* copy kernel image */
            memcpy(sdram_addr, flash_addr, image_size);
            break;

#if !defined(__START_FROM_SRAM__)
        case DRV_IMAGE_COMPRESS_LZMA:
            /* calculate the real image offset, and it's alligned to 4 Bytes */
            offset = IMAGE_SIG_HEADER_OFFSET + sizeof(GT_IMAGE_HEADER);
            offset = (offset + 3) & (~3);

            flash_addr += offset;
            image_size -= offset;

            decode_buf_size = UTL_LZMA_GetDecodeBufSize(flash_addr);
            if (decode_buf_size >= (LOADER_TEXT_BASE - KERNEL_TEXT_BASE))
            {
                UTL_PRT_Printf("\n\rError : Kernel Image Size Overflow!");
                return;
            }

            /* use the block before the BootLoader as decode buffer */
            decode_buf = (GT_UI8 *)(LOADER_TEXT_BASE - decode_buf_size);

            /* decompress image */
            decompress_size = (GT_I32)UTL_LZMA_Decompress(
                                         flash_addr,
                                         image_size,
                                         sdram_addr,
                                         (LOADER_TEXT_BASE - decode_buf_size - KERNEL_TEXT_BASE),
                                         decode_buf,
                                         NULL);

            if (decompress_size == 0)
            {
                UTL_PRT_Printf("\n\rError : Decompress kernel image fail!");
                return;
            }
            else if (decompress_size < 0)
            {
                UTL_PRT_Printf("\n\rError : User Break!");
                return;
            }
            break;
#endif
        default:
            UTL_PRT_Printf("\n\rError : Unknown compress kernel image!");
            return;
    }

	/* run kernel image */
    (*entry_func)();
}


static void reset_system(void)
{
    UTL_PRT_Printf("\n\rPlease wait, system rebooting ...");

    CPU_WatchDog_Set(GT_TRUE);

    while (1)
    {}
}

/******************************************************************************
FUNCTION NAME:
    startC
DESCRIPTION:
    The C procedure start from here
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void startC(void)
{
    GT_I8    get_char;
    GT_UI32  image_size;
    GT_UI32  count;

	DRV_FLASH_Init();

    /* initial VT,and clear screen */
    UTL_VT_Init();
    UTL_VT_ClearScreen();

#if !defined(__START_FROM_SRAM__)
    count = AUTO_LOAD_KERNEL_IMAGE_TIMEOUT;
    while (count-- > 0)
    {
        get_char = UTL_VT_GetChar(1);
        if (get_char != -1)
        {
            break;
        }

        if (count == 0)
        {
            load_kernel_image();

            /* if load kernel image fail, enter BootLoader Menu */
            break;
        }

        delay_ms(1);
    }

    UTL_VT_ClearScreen();
#endif

    while (1)
    {
        /* print loader menu */
        UTL_PRT_Printf(PRINT_MENU, DRV_IMAGE_GetImageVersion((GT_UI8 *)IMAGE_LOADER_BASE));

        while ((get_char = UTL_VT_GetChar(1000)) == -1)
        {}

        switch (get_char)
        {
          #if !defined(__START_FROM_SRAM__)
            case '1':
                if (test_ram(0x55555555))
                {
                    UTL_PRT_Printf("\n\rCircle 1 Pass!");
                }
                if (test_ram(0xAAAAAAAA))
                {
                    UTL_PRT_Printf("\n\rCircle 2 Pass!");
                }
                if (test_ram(0x0))
                {
                    UTL_PRT_Printf("\n\rCircle 3 Pass!");
                }
                break;
          #endif

            case '2':
            	UTL_PRT_Printf("\n\rxModem Downloading Image ................. ");
            	image_size = xModem_Download((GT_UI8 *)IMAGE_DOWNLOAD_BASE, IMAGE_DOWNLOAD_MAX_SIZE);
                if (image_size == 0)
				{
					UTL_PRT_Printf("Fail!");
					break;
                }

				UTL_PRT_Printf("100 %%");
				switch (DRV_Image_GetImageType((GT_UI8 *)IMAGE_DOWNLOAD_BASE))
				{
				    case DRV_IMAGE_LOADER:
				        UTL_PRT_Printf("\n\rProgramming BootLoader Image .............   0 %%");
        				if (DRV_FLASH_Program((GT_UI8 *)IMAGE_LOADER_BASE,
        									  (GT_UI8 *)IMAGE_DOWNLOAD_BASE,
        									   image_size,
        									   UTL_PRT_Display_Callback))
        				{
                            reset_system();
        				}
        				UTL_PRT_Printf("\n\rFail!");
				        break;

				    case DRV_IMAGE_KERNEL:
				        UTL_PRT_Printf("\n\rProgramming Kernel Image .................   0 %%");
        				if (DRV_FLASH_Program((GT_UI8 *)IMAGE_KERNEL_BASE,
        									  (GT_UI8 *)IMAGE_DOWNLOAD_BASE,
        									   image_size,
        									   UTL_PRT_Display_Callback))
        				{
        					reset_system();
        				}
        				UTL_PRT_Printf("\n\rFail!");
				        break;

				    default:
				        UTL_PRT_Printf("\n\rError : Invalid Image!");
				        break;
				}
				break;

#if !defined(__START_FROM_SRAM__)
			case '3':
			    display_memory(LOADER_TEXT_BASE, IMAGE_LOADER_SIZE);
			    break;
			case '4':
			    display_memory(KERNEL_TEXT_BASE, IMAGE_KERNEL_SIZE);
			    break;
            case '5':
			    display_memory(IMAGE_DOWNLOAD_BASE, IMAGE_DOWNLOAD_MAX_SIZE);
			    break;
			case '6':
			    display_memory(IMAGE_LOADER_BASE, IMAGE_LOADER_SIZE);
			    break;
			case '7':
			    display_memory(IMAGE_KERNEL_BASE, IMAGE_KERNEL_SIZE);
			    break;
#endif

			case '8':
				load_kernel_image();
				break;

            case '9':
                reset_system();
                break;

            default:
                break;
        }

        UTL_PRT_Printf("\n\r");
    }

    /* never reach here */
    return;
}

