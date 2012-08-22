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
 *         Name:  open_mod_log_file
 *
 *  Description:  Opens the file located at MOD_LOG_FILE_LOCATION. Creates if
 *                doesn't exist and overwrites if exists.
 *                Saves file pointer to MOD_LOG_FILE
 * 
 *      Version:  0.0.1
 *       Params:  N/A
 *      Returns:  int 0 on success 1 otherwise
 *        Usage:  open_mod_log_file()
 *      Outputs:  N/A
 * ============================================================================
 */
int open_mod_log_file() {
    MOD_LOG_FILE = fopen(MOD_LOG_FILE_LOCATION, "w+");

    fprintf(stderr, "%s\n", "Before check");
    if (MOD_LOG_FILE == NULL) {
        perror("Failed to open log file for writing");
        return false;
    }

    return true;
}


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  close_mod_log_file
 *
 *  Description:  Closes the file handle MOD_LOG_FILE opened on init
 * 
 *      Version:  0.0.1
 *       Params:  N/A
 *      Returns:  void
 *        Usage:  close_mod_log_file()
 *      Outputs:  N/A
 * ============================================================================
 */
void close_mod_log_file() {
    if (MOD_LOG_FILE != NULL)
        fclose(MOD_LOG_FILE);
    MOD_LOG_FILE = NULL;
}
