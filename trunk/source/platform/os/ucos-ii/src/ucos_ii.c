/*******************************************************************************
 *                       uC/OS-II
 *                The Real-Time Kernel
 *
 *     (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
 *                 All Rights Reserved
 *     (c) Copyright ARM Limited 1999.  All rights reserved.
 *
 *
 * File    : ucos_ii.c
 * By      : Jean J. Labrosse
 * Version : v2.76
*******************************************************************************/

#define  OS_GLOBALS           /* Declare GLOBAL variables */
#include <inc/ucos_ii.h>

/* Prevent the following files from including includes.h */
#define  OS_MASTER_FILE

#include <os_core.c>
#include <os_flag.c>
#include <os_mbox.c>
#include <os_mem.c>
#include <os_mutex.c>
#include <os_q.c>
#include <os_sem.c>
#include <os_task.c>
#include <os_time.c>

