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

#ifndef __OS_TASK_API_H
#define __OS_TASK_API_H

#ifdef __cplusplus
extern "C" {
#endif


/* invalid TCB */
#define OS_TASK_API_INVALID_TCB        (-1)


/* OS task entry */
typedef void (*OS_TASK_API_ENTRY)(void *arg);


typedef enum
{
    OS_TASK_API_STATE_NOT_START    = 0,
    OS_TASK_API_STATE_RUNNING,
    OS_TASK_API_STATE_WAIT_SEM,
    OS_TASK_API_STATE_WAIT_MBOX,
    OS_TASK_API_STATE_WAIT_QUEUE,
    OS_TASK_API_STATE_WAIT_EVENT,
    OS_TASK_API_STATE_SUSPEND
} OS_TASK_API_STATE;


typedef struct os_api_TCB
{
    struct os_api_TCB  *prev;       /* previous task entry     */
    struct os_api_TCB  *next;       /* next task entry         */

    /* task id.
     * used to distinguish task control block.
     */
    GT_I32              task_id;

    GT_UI8              name[OS_API_ELEMENT_MAX_NAME_LEN];  /* task name */
    OS_TASK_API_ENTRY   entry;      /* task entry              */
    void                *arg;       /* task argument           */
    GT_UI32             priority;   /* task priority           */
    GT_BOOL             b_start;    /* after task is created, start it or not */

    /* task stack information */
    GT_UI32            *stack_base; /* stack base address      */
    GT_UI32             stack_size; /* total usable stack size */
    GT_UI32             stack_used; /* used task stack size    */

    GT_UI32             task_time_counts; /* task time counts from start */

    /* current task status */
    OS_TASK_API_STATE   task_status; /* task status            */
    GT_UI8              wait_element_name[OS_API_ELEMENT_MAX_NAME_LEN]; /* only when task is at waiting pend state, this is valid */

} OS_TASK_API_TCB;


OS_API_HANDLE OS_TASK_API_Create
(
    IN const GT_UI8     *task_name,
    IN OS_TASK_API_ENTRY entry,
	IN const void       *arg,
	IN const GT_UI32     stack_size,
	IN const GT_UI32     priority,
	IN const GT_BOOL     b_start
);

void OS_TASK_API_Delete(IN OS_TASK_API_TCB *p_TCB);

void OS_TASK_API_Suspend(IN OS_TASK_API_TCB *p_TCB);

void OS_TASK_API_Resume(IN OS_TASK_API_TCB *p_TCB);

GT_BOOL OS_TASK_API_GetTCB(IN OUT OS_TASK_API_TCB *p_TCB);

GT_BOOL OS_TASK_API_GetNextTCB(IN OUT OS_TASK_API_TCB *p_get_TCB);

void OS_TASK_API_Sleep(IN const GT_UI32 sleep_ms);

void OS_TASK_API_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __OS_TASK_API_H */

