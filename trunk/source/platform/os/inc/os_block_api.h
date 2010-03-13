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

#ifndef __OS_BLOCK_API_H
#define __OS_BLOCK_API_H

#ifdef __cplusplus
extern "C" {
#endif


/* invalid BCB */
#define OS_BLOCK_API_INVALID_BCB        (-1)


typedef struct os_api_BCB
{
    struct os_api_BCB  *prev;          /* previous block entry             */
    struct os_api_BCB  *next;          /* next block entry                 */

    /* block id.
     * used to distinguish block control block.
     */
    GT_I32              block_id;

    GT_UI8              name[OS_API_ELEMENT_MAX_NAME_LEN]; /* block name   */
    GT_UI32            *block_base;    /* block base address               */
    GT_UI32             total_size;    /* usable size of this block        */
    GT_UI32             block_size;    /* size of each block (in Bytes)    */
    GT_UI32             total_block;   /* total blocks                     */

    GT_UI32             used_block;    /* used blocks counts               */

} OS_BLOCK_API_BCB;


void OS_BLOCK_API_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __OS_BLOCK_API_H */

