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

    /* Create dummy thread */
    pth_attr_t attr = pth_attr_new();
    pth_attr_set(attr, PTH_ATTR_NAME, "dummy");
    pth_attr_set(attr, PTH_ATTR_DUMMY, TRUE);
    a1_mod_dummy_thread = pth_spawn(attr, a1_mod_dummy_thread_func, NULL);
    if (!a1_mod_dummy_thread)
        perror("Failed to spawn dummy thread");

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
    return pth_sched != NULL && pth_main != NULL &&
        t != pth_sched && t != pth_main && !t->dummy;
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
 *        Usage:  a1_mod_log_print_line_start( pth_time_t t, pth_time_t running, int dummy )
 *      Outputs:  N/A
 * ============================================================================
 */
void a1_mod_log_print_line_start(pth_time_t t, pth_time_t running, int dummy) {
    /* Don't do anything if we don't have a start time yet */
    if (pth_time_equal(a1_mod_start_time, *PTH_TIME_ZERO))
        return;

    /* Work out the diff between start time and time now */
    int t_ms = pth_time_t2msi(&t);
    int start_ms = pth_time_t2msi(&a1_mod_start_time);
    int running_ms = pth_time_t2msi(&running);
    int slice_time = t_ms - start_ms - running_ms;
    if (slice_time < 0)
        slice_time = 0;

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

    if (dummy)
        fprintf(MOD_LOG_FILE, "\n%s", slice_s);
    else
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


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_update_deadlines
 *
 *  Description:  Loops through the list of user threads and decrements all
 *                deadlines. Any that are zero AFTER decrementing will be reset
 *                to their origin value
 * 
 *      Version:  0.0.1
 *       Params:  N/A
 *      Returns:  void
 *        Usage:  a1_mod_update_deadlines()
 *      Outputs:  N/A
 * ============================================================================
 */
void a1_mod_update_deadlines() {
    int i;
    pth_t t;

    for ( i = 0; i < user_threads_count; i++ ) {
        t = user_threads[i];
        t->deadline_t_counter--;
        if (t->deadline_t_counter == -1) {
            /* Reset deadline period */
            t->deadline_t_counter = t->deadline_t;
            t->deadline_run_count = 0;
        }
    }
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_update_run_count
 *
 *  Description:  Updates the deadline_run_count for the given thread
 * 
 *      Version:  0.0.1
 *       Params:  pth_t t - Thread to update
 *      Returns:  void
 *        Usage:  a1_mod_update_run_count( pth_t t )
 *      Outputs:  N/A
 * ============================================================================
 */
void a1_mod_update_run_count(pth_t t) {
    t->deadline_run_count++;
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_is_schedulable
 *
 *  Description:  Checks whether the given thread passes the schedulability tests
 * 
 *      Version:  0.0.1
 *       Params:  pth_t t - Thread to check
 *                pth_pqueue_t queues[4] - Array of queues to check
 *      Returns:  bool true if schedulable
 *                bool false otherwise
 *        Usage:  a1_mod_is_schedulable( pth_t t, pth_pqueue_t queues[4] )
 *      Outputs:  N/A
 * ============================================================================
 */
int a1_mod_is_schedulable(pth_t t, pth_pqueue_t* queues[]) {
    int i;
    float total;
    pth_t head;

    total = ((float) t->deadline_c / (float) t->deadline_t);

    for ( i = 0; i < 4; i++ ) {
        head = pth_pqueue_head(queues[i]);

        /* Find the lowest deadline */
        while (head != NULL) {
            if (head != pth_main) {
                total += (float) head->deadline_c / (float) head->deadline_t;
            }
            head = pth_pqueue_walk(queues[i], head, PTH_WALK_NEXT);
        }
    }

    return (total <= 1.0);
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_dummy_thread_func
 *
 *  Description:  Dummy thread. Just an infinite while loop. Used to fill in
 *                gaps where no threads are schedulable
 * 
 *      Version:  0.0.1
 *       Params:  void arg*
 *      Returns:  static void *
 *        Usage:  a1_mod_dummy_thread_func( void *arg )
 *      Outputs:  N/A
 * ============================================================================
 */
void *a1_mod_dummy_thread_func(void *arg) {
    while (true) { /* Loop forever! */ }
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_earliest_deadline_first
 *
 *  Description:  This is the heart of A1 Mod. This is where the pth_pqueue
 *                shuffling happens. Takes a pth_pqueue, walks through it,
 *                storing the lowest deadline it finds. Does tiebreaking as
 *                it goes. Returns TRUE if a schedulable thread is found, FALSE
 *                otherwise.
 * 
 *      Version:  0.0.1
 *       Params:  pth_pqueue_t q - The queue to manipulate
 *      Returns:  bool true if schedulable thread found
 *                bool false otherwise
 *        Usage:  a1_mod_earliest_deadline_first( pth_pqueue_t *q )
 *      Outputs:  N/A
 * ============================================================================
 */
int a1_mod_earliest_deadline_first(pth_pqueue_t *q) {
    pth_t head = pth_pqueue_head(q);
    pth_t favourite = NULL;

    /* Find the lowest deadline */
    while (head != NULL) {
        if (head == pth_main) {
            favourite = head;
            break;
        }
        if (a1_mod_is_runnable(head)) {
            favourite = a1_mod_lowest_thread(head, favourite);
        }
        head = pth_pqueue_walk(q, head, PTH_WALK_NEXT);
    }

    if (favourite != NULL) {
        pth_pqueue_favorite(q, favourite);
        return true;
    } else {
        return false;
    }
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_is_runnable
 *
 *  Description:  Checks whether a thread is eligible to be run in the current
 *                time slice
 * 
 *      Version:  0.0.1
 *       Params:  pth_t t
 *      Returns:  bool true if runnable
 *                bool false otherwise
 *        Usage:  a1_mod_is_runnable( pth_t t )
 *      Outputs:  N/A
 * ============================================================================
 */
bool a1_mod_is_runnable(pth_t t) {
    return t != NULL && t->deadline_run_count < t->deadline_c;
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  a1_mod_lowest_thread
 *
 *  Description:  Take two threads and checks which one has the earliest deadline.
 *                Applies tie breakers where necessary.
 * 
 *      Version:  0.0.1
 *       Params:  pth_t t1
 *                pth_t t2
 *      Returns:  pth_t lowest
 *        Usage:  a1_mod_lowest_thread( pth_t t1, pth_t t2 )
 *      Outputs:  N/A
 * ============================================================================
 */
pth_t a1_mod_lowest_thread(pth_t t1, pth_t t2) {
    if (t1 == NULL && t2 == NULL)
        return NULL;
    if (t2 == NULL || t1->deadline_t_counter < t2->deadline_t_counter)
        return t1;
    if (t1 == NULL || t1->deadline_t_counter > t2->deadline_t_counter)
        return t2;

    /* Tiebreaker */
    if (pth_time_cmp(&t1->spawned, &t2->spawned) <= 0)
        return t1;
    else
        return t2;

    // Will never get here
    return NULL;
}
