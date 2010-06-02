/******************************************************************************
 *
 * (C) Copyright 2007-2010
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *   MA 02111-1307 USA
 *
 * FILE NAME:
 *   aduc7020.ld
 * DESCRIPTION:
 *   Linker Script File.
 * HISTORY:
 *   2010.5.31        Panda.Xiong         Create/Update
 *
*****************************************************************************/


/* Memory Definitions */
MEMORY
{
  IntFLASH (rx) : ORIGIN = 0x00000000, LENGTH = 51200
  IntRAM   (rw) : ORIGIN = 0x00010000, LENGTH = 8192
}


/* Section Definitions */
SECTIONS
{
  /* .text is used for code */
  .text :
  {
    __text_start = .;
    *startup.o (.text)         /* Startup code    */
    *(.text)                   /* remaining code  */
	. = ALIGN(4);
   } >IntFLASH

  /* .rodata section which is used for read-only data (constants) */
  .rodata :
  {
    *(.rodata)
    . = ALIGN(16);
  } >IntFLASH

  /* do not try to remove this alignment!!! */
  . = ALIGN(4);
  __text_end = .;

  /* .data section which is used for initialized data */
  .data : AT (__text_end)
  {
    __data_start = .;

	. = ALIGN(16);
    *(.data)

	. = ALIGN(4);
	__data_end = .;
  } >IntRAM

  /* do not try to remove this alignment!!! */
  . = ALIGN(4);

  /* .bss section which is used for uninitialized data */
  .bss :
  {
    __bss_start = .;
    *(.bss)
    . = ALIGN(4);
    __bss_end = .;
  } >IntRAM
}

