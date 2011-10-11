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
 *   cli_porting.h
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2010.4.13        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __CLI_PORTING_H
#define __CLI_PORTING_H


#define CLI_VERSION             "v1.00"

/* define CLI can exit or not */
#define CLI_ENABLE_EXIT			(TRUE)
#define CLI_EXIT_CMD			"exit"

/* CLI max command history,
 * shouldn't larger than 255 */
#define CLI_MAX_CMD_HISTORY     (255)

/* CLI max command length */
#define CLI_MAX_CMD_LEN         (1024)

/* CLI max parameters in one command */
#define CLI_MAX_PARAM		    (32)

/* screen width of standard VT100 is 80 */
#define CLI_SCREEN_WIDTH        (80)

/* screen hight of standard VT100 is 24 */
#define CLI_SCREEN_HIGHT        (24)

/* CLI dynamic display command result,
 * format: <command> +/D100 --> dynamic executing 100 times of this command.
 */
#define CLI_ENABLE_DYNAMIC_DISPLAY  (TRUE)
#define CLI_DYNAMIC_DISPLAY_KEYWORD "+/D"

#define CLI_HELP_STRING_1       "? "
#define CLI_HELP_STRING_2       "help "


#endif /* __CLI_PORTING_H */

