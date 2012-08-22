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

    pth_time_set(&a1_mod_start_time, PTH_TIME_ZERO);

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

    fprintf(MOD_LOG_FILE, "%-8.8s", "t");
    
    /* Print out the info */
    for ( i = 0; i < user_threads_count; i++ ) {
        if (i < user_threads_count - 1) {
            fprintf(MOD_LOG_FILE, "%-8.8s", user_threads[i]->name);
        } else {
            fprintf(MOD_LOG_FILE, "%s", user_threads[i]->name);
            break;
        }
    }
    fflush(MOD_LOG_FILE);
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
int a1_mod_is_user_thread(pth_t t) {
    return pth_sched != NULL && pth_main != NULL && t != pth_sched && t != pth_main;
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_log_print_line_start
 *
 *  Description:  Prints the start of a new log line. Called just before thread
 *                switching.
 * 
 *      Version:  0.0.1
 *       Params:  pth_time_t t - Time object
 *      Returns:  void
 *        Usage:  a1_mod_log_print_line_start( pth_time_t t, pth_time_t running )
 *      Outputs:  N/A
 * ============================================================================
 */
void a1_mod_log_print_line_start(pth_time_t t, pth_time_t running) {
    /* Work out the diff between start time and time now */
    int t_ms = pth_time_t2msi(&t);
    int start_ms = pth_time_t2msi(&a1_mod_start_time);
    int running_ms = pth_time_t2msi(&running);
    int slice_time = t_ms - start_ms - running_ms;

    /* Convert slice time into a string */
    int slice_s_size = (int) (slice_time / 10);
    if (slice_s_size < 1)
        slice_s_size = 1;
    char slice_s[slice_s_size + 1];
    if (slice_time == 0) {
        strcpy(slice_s, "0");
    } else {
        snprintf(slice_s, slice_s_size, "%d", slice_time);
    }

    fprintf(MOD_LOG_FILE, "\n%-8.8s", slice_s);
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_log_print_line_end
 *
 *  Description:  Loops through and prints the remainder of the log entry
 *                displaying what yielded or was preempted.
 * 
 *      Version:  0.0.1
 *       Params:  pth_t t - The thread that was ended
 *                char py - The method by which it returned to scheduler
 *      Returns:  void
 *        Usage:  a1_mod_log_print_line_end( pth_t t, char py )
 *      Outputs:  N/A
 * ============================================================================
 */
void a1_mod_log_print_line_end(pth_t t, char py) {
    int i;
    char str[9];
    char result[5];
    snprintf(result, 5, "+++%c", py);

    for ( i = 0; i < user_threads_count; i++ ) {
        /* Check if this is the thread we just finished with */
        str[0] = '\0';
        if (t == user_threads[i]) {
            strcpy(str, result);
        }

        /* Print out the info */
        if (i != user_threads_count - 1 && !strcmp(str, "")) {
            fprintf(MOD_LOG_FILE, "%-8.8s", str);
        } else {
            fprintf(MOD_LOG_FILE, "%s", str);
        }

        /* Don't need to do anything else if we just printed a result */
        if (strcmp(str, ""))
            break;
    }
}
