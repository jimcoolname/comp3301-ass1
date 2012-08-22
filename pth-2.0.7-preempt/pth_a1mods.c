/*
 * ============================================================================
 *
 *       Filename:  pth_a1mods.c
 *
 *    Description:  Modifications made in accordance with COMP3301 Assignment 1
 *
 *        Version:  0.0.1
 *        Created:  22/08/12 10:13:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Tony Lee (Roganartu), uni@roganartu.com
 *   Organisation:  UQ Bachelor of Engineering
 *
 * ============================================================================
 */

#include <stdio.h>
#include <unistd.h>

#define _INCLUDE_A1_MODS_
#include "pth_a1mods.h"
#undef _INCLUDE_A1_MODS_


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_init
 *
 *  Description:  Opens the file located at MOD_LOG_FILE_LOCATION. Creates if
 *                doesn't exist and overwrites if exists.
 *                Saves file pointer to MOD_LOG_FILE
 *                Initialises user_threads with malloc for reallocing later
 * 
 *      Version:  0.0.1
 *       Params:  N/A
 *      Returns:  int 0 on success 1 otherwise
 *        Usage:  a1_mod_init()
 *      Outputs:  N/A
 * ============================================================================
 */
int a1_mod_init() {
    /* Open log file */
    MOD_LOG_FILE = fopen(MOD_LOG_FILE_LOCATION, "w+");

    if (MOD_LOG_FILE == NULL) {
        perror("Failed to open log file for writing");
        return false;
    }

    /* Get user_threads ready */
    user_threads = malloc(sizeof(pth_t)*1);

    return true;
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_kill
 *
 *  Description:  Closes the file handle MOD_LOG_FILE opened on init
 * 
 *      Version:  0.0.1
 *       Params:  N/A
 *      Returns:  void
 *        Usage:  a1_mod_kill()
 *      Outputs:  N/A
 * ============================================================================
 */
void a1_mod_kill() {
    /* Close log file */
    if (MOD_LOG_FILE != NULL)
        fclose(MOD_LOG_FILE);
    MOD_LOG_FILE = NULL;

    /* Free malloc'd objects */
    free(user_threads);
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_add_thread
 *
 *  Description:  Adds a new thread to the user_threads array. Calls
 *                a1_mod_print_log_header
 * 
 *      Version:  0.0.1
 *       Params:  pth_t *t - Pointer to thread struct
 *      Returns:  void
 *        Usage:  a1_mod_add_thread( pth_t *t )
 *      Outputs:  N/A
 * ============================================================================
 */
void a1_mod_add_thread(pth_t t) {
    if (a1_mod_is_user_thread(t)) {

        /* Only need to realloc the second and subsequent threads */
        if (user_threads_count++ > 0) {
            user_threads = realloc(user_threads, sizeof(pth_t) * user_threads_count);
        }

        user_threads[user_threads_count - 1] = t;

        a1_mod_print_log_header();
    }
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_print_log_header
 *
 *  Description:  Prints header line out to the MOD_LOG_FILE from user_threads
 *                array.
 * 
 *      Version:  0.0.1
 *       Params:  N/A
 *      Returns:  void
 *        Usage:  a1_mod_print_log_header()
 *      Outputs:  N/A
 * ============================================================================
 */
void a1_mod_print_log_header() {
    int i = 0;

     /* Lead with a newline so we never have trailing blanks */
    if (!A1_MOD_LOG_FILE_FIRST_LINE) {
        fprintf(MOD_LOG_FILE, "%s", "\n");
    } else {
        A1_MOD_LOG_FILE_FIRST_LINE = false;
    }

    fprintf(MOD_LOG_FILE, "%s", "t");
    
    for ( i = 0; i < user_threads_count; i++ ) {
        fprintf(MOD_LOG_FILE, " %7.7s", user_threads[i]->name);
    }
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_is_user_thread
 *
 *  Description:  Returns true if the supplied thread was spawned by the user
 *                and false otherwise
 * 
 *      Version:  0.0.1
 *       Params:  pth_t t - the thread being checked
 *      Returns:  bool true if spawned by user (specifically with pth_spawn)
 *                bool false otherwise
 *        Usage:  a1_mod_is_user_thread( pth_t t )
 *      Outputs:  N/A
 * ============================================================================
 */
bool a1_mod_is_user_thread(pth_t t) {
    return pth_sched != NULL && pth_main != NULL && t != pth_sched && t != pth_main;
}
