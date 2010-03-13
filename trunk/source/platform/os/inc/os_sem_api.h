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

#ifndef __OS_SEM_API_H
#define __OS_SEM_API_H

#ifdef __cplusplus
extern "C" {
#endif


/* invalid SCB */
#define OS_SEM_API_INVALID_SCB        (-1)


typedef enum
{
    OS_SEM_API_TYPE_BINARY = 0,
    OS_SEM_API_TYPE_COUNT  = 1,
    OS_SEM_API_TYPE_MUTEX  = 2
} OS_SEM_API_TYPE;


typedef struct os_api_SCB
{
    struct os_api_SCB  *prev;       /* previous semaphore entry */
    struct os_api_SCB  *next;       /* next semaphore entry     */

    /* sem id.
     * used to distinguish sem control block.
     */
    GT_I32              sem_id;

    GT_UI8              name[OS_API_ELEMENT_MAX_NAME_LEN]; /* semaphore name  */
    OS_SEM_API_TYPE     sem_type;   /* semaphore type          */

    GT_UI32             sem_counts; /* semaphore counts        */

    /* latest task name, who owners this semaphore */
    GT_UI8               task_name[OS_API_ELEMENT_MAX_NAME_LEN];

} OS_SEM_API_SCB;


void OS_SEM_API_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __OS_SEM_API_H */

