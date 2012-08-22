/*
 * ============================================================================
 *
 *       Filename:  pth_a1mods.h
 *
 *    Description:  Header for modifications made in accordance with COMP3301
 *                  Assignnment 1.
 *                  Included in pth_p.h for global declaration as private API
 *                  calls.
 *
 *        Version:  0.0.1
 *        Created:  22/08/12 10:04:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Tony Lee (Roganartu), uni@roganartu.com
 *   Organisation:  UQ Bachelor of Engineering
 *
 * ============================================================================
 */

#include "pth.h"
#include "pth_p.h"

#ifndef _PTH_A1_MODS_H_
#define _PTH_A1_MODS_H_

/* These "globals" only need to be seen in pth_a1mods.c */
#ifdef _INCLUDE_A1_MODS_

/* Boolean helpers */
typedef int bool;
const bool false = 0;
const bool true = 1;

/* Pointer to log file created on initialisation */
FILE* MOD_LOG_FILE;

const char* MOD_LOG_FILE_LOCATION = "sched.log";

bool A1_MOD_LOG_FILE_FIRST_LINE = 1;

/* Array of user threads */
pth_t *user_threads;
int user_threads_count = 0;

/* Local function defs */
void a1_mod_print_log_header();

#endif /* _PTH_A1_MODS_C_ */

/* These declarations are globally available to the rest of the pth lib */

pth_time_t a1_mod_start_time;

/* Private API methods */

extern int a1_mod_init();

extern void a1_mod_kill();

extern void a1_mod_add_thread(pth_t t);

extern int a1_mod_is_user_thread(pth_t t);

extern void a1_mod_log_print_line_start(pth_time_t t, pth_time_t running);

extern void a1_mod_log_print_line_end(pth_t t, char py);

extern void a1_mod_update_deadlines();

extern void a1_mod_update_run_count(pth_t t);

extern int a1_mod_is_schedulable(pth_t t);

#endif /* _PTH_A1_MODS_H_ */
