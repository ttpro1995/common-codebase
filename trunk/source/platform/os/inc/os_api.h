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

#ifndef __OS_API_H
#define __OS_API_H

#ifdef __cplusplus
extern "C" {
#endif


#include <h/basetype.h>
#include <inc/cpu_int.h>
#include <inc/utl_prt.h>
#include <inc/utl_tran.h>


/* OS per tick time, set to 10ms */
#define OS_API_PER_TICK_TIME        (10)


#define OS_API_TASK_MAX_NUM         (10)    /* number of tasks */
#define	OS_API_TASK_MAX_STACK_SIZE	(1024)  /* Stack size, in 32bit */

#define OS_API_SEM_MAX_NUM          (20)    /* number of semaphores */

#define OS_API_QUEUE_MAX_NUM        (10)    /* number of queues */

#define OS_API_EVENT_MAX_NUM        (10)    /* number of events */

#define OS_API_POOL_MAX_NUM         (10)    /* number of pool */

#define OS_API_BLOCK_MAX_NUM        (10)    /* number of block */

/* OS max element name length */
#define OS_API_ELEMENT_MAX_NAME_LEN (16)

#define OS_API_Printf               UTL_PRT_Printf


/* OS handler, all OS APIs use this data-struct to control */
typedef void *OS_API_HANDLE;


#include "./os_task_api.h"
#include "./os_sem_api.h"
#include "./os_event_api.h"
#include "./os_queue_api.h"
#include "./os_pool_api.h"
#include "./os_block_api.h"


void OS_API_FatalError(IN const void *fmt, ...);

GT_BOOL OS_API_Init(void);

void OS_API_Start(void);


#ifdef __cplusplus
}
#endif

#endif /* __OS_API_H */

