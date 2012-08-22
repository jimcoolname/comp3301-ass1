/*
 * ============================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  Test file for pth. Assignment 1 COMP3301
 *
 *        Version:  0.0.1
 *        Created:  08/01/12 15:05:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Tony Lee (Roganartu), uni@roganartu.com
 *   Organization:  
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "./pth-2.0.7-preempt/pth.h"

static void *thread_func(void *arg);

int main(int argc, char *argv[]) {
    if (pth_init()) {

        pth_attr_t attr = pth_attr_new();
        pth_attr_set(attr, PTH_ATTR_NAME, "child1");
        pth_spawn(attr, thread_func, NULL);

        pth_attr_set(attr, PTH_ATTR_NAME, "child2");
        pth_spawn(attr, thread_func, NULL);

        pth_sleep(1);

        pth_kill();

        return 0;
    }
    return 1;
}

static void *thread_func(void *arg) {
    pth_yield(NULL);
    while (TRUE) {
        // Do nothing
    }
    return NULL;
}
