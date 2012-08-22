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

#ifndef _PTH_A1_MODS_H_
#define _PTH_A1_MODS_H_

/* These globals only need to be seen in pth_a1mods.c */
#ifdef _INCLUDE_A1_MODS_

/* Boolean helpers */
typedef int bool;
bool false = 0;
bool true = 1;

/* Pointer to log file created on initialisation */
FILE* MOD_LOG_FILE;

char* MOD_LOG_FILE_LOCATION = "sched.log";

#endif /* _PTH_A1_MODS_C_ */

/* These declarations are globally available to the rest of the pth lib */

extern int a1_mod_init();

extern void a1_mod_kill();

#endif /* _PTH_A1_MODS_H_ */
