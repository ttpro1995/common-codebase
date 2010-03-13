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

#ifndef __OS_QUEUE_API_H
#define __OS_QUEUE_API_H

#ifdef __cplusplus
extern "C" {
#endif


/* invalid QCB */
#define OS_QUEUE_API_INVALID_QCB        (-1)


typedef struct os_api_QCB
{
    struct os_api_QCB  *prev;          /* previous queue entry */
    struct os_api_QCB  *next;          /* next queue entry     */

    /* queue id.
     * used to distinguish queue control block.
     */
    GT_I32              queue_id;

    GT_UI8              name[OS_API_ELEMENT_MAX_NAME_LEN]; /* queue name  */
    GT_UI32             queue_num;     /* the number of queue  */
    GT_UI32             queue_size;    /* queue size           */

    GT_UI32             queue_counts;  /* queue counts         */

} OS_QUEUE_API_QCB;


void OS_QUEUE_API_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __OS_QUEUE_API_H */

