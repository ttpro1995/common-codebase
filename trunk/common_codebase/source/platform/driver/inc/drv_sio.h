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

#ifndef __DRV_SIO_H
#define __DRV_SIO_H

#ifdef __cplusplus
extern "C" {
#endif


#include <h/basetype.h>
#include <inc/utl_vt.h>


/* SIO rx buffer used when tx polling */
#define SIO_BUFFER_SIZE					(5*1024)

/* get char from SIO */
int DRV_SIO_GetChar(void);

/* send char from SIO.
 * if there is some input data, save it into buffer,
 * to make sure the SIO input data will not loss.
 */
void DRV_SIO_PutChar(unsigned char c);

/* here, we use polling uart Rx to delay, to make there is no data loss. */
void DRV_SIO_Delay(unsigned int msec);

/* update SIO buffer, only used by interrupt vector */
void DRV_SIO_UpdateBuffer(void);

/* init SIO buffer */
void DRV_SIO_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __DRV_SIO_H */

