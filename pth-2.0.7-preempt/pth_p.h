/* pth_p.h -- autogenerated from pth_p.h.in, DO NOT EDIT! */
#line 1 "pth_p.h.in"
/*
**  GNU Pth - The GNU Portable Threads
**  Copyright (c) 1999-2006 Ralf S. Engelschall <rse@engelschall.com>
**
**  This file is part of GNU Pth, a non-preemptive thread scheduling
**  library which can be found at http://www.gnu.org/software/pth/.
**
**  This library is free software; you can redistribute it and/or
**  modify it under the terms of the GNU Lesser General Public
**  License as published by the Free Software Foundation; either
**  version 2.1 of the License, or (at your option) any later version.
**
**  This library is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**  Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License along with this library; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
**  USA, or contact Ralf S. Engelschall <rse@engelschall.com>.
**
**  pth_p.h: Pth private API definitions
*/

#ifndef _PTH_P_H_
#define _PTH_P_H_

/* mandatory system headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <time.h>

/* COMP3301 Assignment 1 modifications header */
#include "pth_a1mods.h"

/* library version */
#define _PTH_VERS_C_AS_HEADER_
#include "pth_vers.c"
#undef  _PTH_VERS_C_AS_HEADER_

/* public API headers */
#define _PTH_PRIVATE
#include "pth.h"
#undef _PTH_PRIVATE

/* autoconf defines and macros */
#include "pth_acdef.h"
#include "pth_acmac.h"

/* optional system headers */
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#ifdef HAVE_NET_ERRNO_H
#include <net/errno.h>
#endif
#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

/* dmalloc support */
#ifdef PTH_DMALLOC
#include <dmalloc.h>
#endif

/* OSSP ex support */
#ifdef PTH_EX
#define __EX_NS_USE_CUSTOM__
#include "ex.h"
#endif

/* paths */
#ifdef HAVE_PATHS_H
#include <paths.h>
#endif
#ifdef _PATH_BSHELL
#define PTH_PATH_BINSH _PATH_BSHELL
#else
#define PTH_PATH_BINSH "/bin/sh"
#endif

/* non-blocking flags */
#ifdef  O_NONBLOCK
#define O_NONBLOCKING O_NONBLOCK
#else
#ifdef  O_NDELAY
#define O_NONBLOCKING O_NDELAY
#else
#ifdef  FNDELAY
#define O_NONBLOCKING FNDELAY
#else
#error "No O_NONBLOCK, O_NDELAY or FNDELAY flag available!"
#endif
#endif
#endif

/* fallback definition for fdset_t size */
#if !defined(FD_SETSIZE)
#define FD_SETSIZE 1024
#endif

/* fallback definition for struct timespec */
#ifndef HAVE_STRUCT_TIMESPEC
struct timespec {
    time_t  tv_sec;     /* seconds */
    long    tv_nsec;    /* and nanoseconds */
};
#endif

/* compiler happyness: avoid ``empty compilation unit'' problem */
#define COMPILER_HAPPYNESS(name) \
    int __##name##_unit = 0;

/* generated contents */
BEGIN_DECLARATION
#line 36 "pth_compat.c"
#if !defined(HAVE_STRERROR)
char *_pth_compat_strerror(int);
#define strerror(errnum) _pth_compat_strerror(errnum)
#endif
#line 31 "pth_debug.c"

#ifndef PTH_DEBUG

#define pth_debug1(a1)                     /* NOP */
#define pth_debug2(a1, a2)                 /* NOP */
#define pth_debug3(a1, a2, a3)             /* NOP */
#define pth_debug4(a1, a2, a3, a4)         /* NOP */
#define pth_debug5(a1, a2, a3, a4, a5)     /* NOP */
#define pth_debug6(a1, a2, a3, a4, a5, a6) /* NOP */

#else

#define pth_debug1(a1)                     pth_debug(__FILE__, __LINE__, 1, a1)
#define pth_debug2(a1, a2)                 pth_debug(__FILE__, __LINE__, 2, a1, a2)
#define pth_debug3(a1, a2, a3)             pth_debug(__FILE__, __LINE__, 3, a1, a2, a3)
#define pth_debug4(a1, a2, a3, a4)         pth_debug(__FILE__, __LINE__, 4, a1, a2, a3, a4)
#define pth_debug5(a1, a2, a3, a4, a5)     pth_debug(__FILE__, __LINE__, 5, a1, a2, a3, a4, a5)
#define pth_debug6(a1, a2, a3, a4, a5, a6) pth_debug(__FILE__, __LINE__, 6, a1, a2, a3, a4, a5, a6)

#endif /* PTH_DEBUG */

#line 68 "pth_syscall.c"
#if PTH_SYSCALL_HARD
/* hard syscall mapping */
#if HAVE_SYS_SYSCALL_H
#include <sys/syscall.h>
#endif
#ifdef HAVE_SYS_SOCKETCALL_H
#include <sys/socketcall.h>
#endif
#define pth_sc(func) pth_sc_##func
#else /* !PTH_SYSCALL_HARD */
/* no hard syscall mapping */
#define pth_sc(func) func
#endif /* PTH_SYSCALL_HARD */
#line 113 "pth_syscall.c"
typedef int (*pth_syscall_fct_t)();
typedef struct {
    char             *name;    /* name of system/function call */
    pth_syscall_fct_t addr;    /* address of wrapped system/function call */
} pth_syscall_fct_tab_t;
typedef struct {
    char             *path;    /* path to dynamic library */
    void             *handle;  /* handle of dynamic library */
} pth_syscall_lib_tab_t;
#line 30 "pth_errno.c"

/* enclose errno in a block */
#define pth_shield \
        for ( pth_errno_storage = errno, \
              pth_errno_flag = TRUE; \
              pth_errno_flag; \
              errno = pth_errno_storage, \
              pth_errno_flag = FALSE )

/* return plus setting an errno value */
#if defined(PTH_DEBUG)
#define pth_error(return_val,errno_val) \
        (errno = (errno_val), \
        pth_debug4("return 0x%lx with errno %d(\"%s\")", \
                   (unsigned long)(return_val), (errno), strerror((errno))), \
        (return_val))
#else
#define pth_error(return_val,errno_val) \
        (errno = (errno_val), (return_val))
#endif

#line 55 "pth_ring.c"
#define pth_ring_elements(r) \
    ((r) == NULL ? (-1) : (r)->r_nodes)
#line 61 "pth_ring.c"
#define pth_ring_first(r) \
    ((r) == NULL ? NULL : (r)->r_hook)
#line 67 "pth_ring.c"
#define pth_ring_last(r) \
    ((r) == NULL ? NULL : ((r)->r_hook == NULL ? NULL : (r)->r_hook->rn_prev))
#line 73 "pth_ring.c"
#define pth_ring_next(r, rn) \
    (((r) == NULL || (rn) == NULL) ? NULL : ((rn)->rn_next == (r)->r_hook ? NULL : (rn)->rn_next))
#line 79 "pth_ring.c"
#define pth_ring_prev(r, rn) \
    (((r) == NULL || (rn) == NULL) ? NULL : ((rn)->rn_prev == (r)->r_hook->rn_prev ? NULL : (rn)->rn_prev))
#line 85 "pth_ring.c"
#define pth_ring_insert(r, rn) \
    pth_ring_append((r), (rn))
#line 175 "pth_ring.c"
#define pth_ring_push(r, rn) \
    pth_ring_prepend((r), (rn))
#line 208 "pth_ring.c"
#define pth_ring_enqueue(r, rn) \
    pth_ring_prepend((r), (rn))
#line 30 "pth_mctx.c"

/*
 * machine context state structure
 *
 * In `jb' the CPU registers, the program counter, the stack
 * pointer and (usually) the signals mask is stored. When the
 * signal mask cannot be implicitly stored in `jb', it's
 * alternatively stored explicitly in `sigs'. The `error' stores
 * the value of `errno'.
 */

#if PTH_MCTX_MTH(mcsc)
#include <ucontext.h>
#endif

typedef struct pth_mctx_st pth_mctx_t;
struct pth_mctx_st {
#if PTH_MCTX_MTH(mcsc)
    ucontext_t uc;
    int restored;
#elif PTH_MCTX_MTH(sjlj)
    pth_sigjmpbuf jb;
#else
#error "unknown mctx method"
#endif
    sigset_t sigs;
#if PTH_MCTX_DSP(sjlje)
    sigset_t block;
#endif
    int error;
};

/*
** ____ MACHINE STATE SWITCHING ______________________________________
*/

/*
 * save the current machine context
 */
#if PTH_MCTX_MTH(mcsc)
#define pth_mctx_save(mctx) \
        ( (mctx)->error = errno, \
          (mctx)->restored = 0, \
          getcontext(&(mctx)->uc), \
          (mctx)->restored )
#elif PTH_MCTX_MTH(sjlj) && PTH_MCTX_DSP(sjlje)
#define pth_mctx_save(mctx) \
        ( (mctx)->error = errno, \
          pth_sc(sigprocmask)(SIG_SETMASK, &((mctx)->block), NULL), \
          pth_sigsetjmp((mctx)->jb) )
#elif PTH_MCTX_MTH(sjlj)
#define pth_mctx_save(mctx) \
        ( (mctx)->error = errno, \
          pth_sigsetjmp((mctx)->jb) )
#else
#error "unknown mctx method"
#endif

/*
 * restore the current machine context
 * (at the location of the old context)
 */
#if PTH_MCTX_MTH(mcsc)
#define pth_mctx_restore(mctx) \
        ( errno = (mctx)->error, \
          (mctx)->restored = 1, \
          (void)setcontext(&(mctx)->uc) )
#elif PTH_MCTX_MTH(sjlj)
#define pth_mctx_restore(mctx) \
        ( errno = (mctx)->error, \
          (void)pth_siglongjmp((mctx)->jb, 1) )
#else
#error "unknown mctx method"
#endif

/*
 * restore the current machine context
 * (at the location of the new context)
 */
#if PTH_MCTX_MTH(sjlj) && PTH_MCTX_DSP(sjlje)
#define pth_mctx_restored(mctx) \
        pth_sc(sigprocmask)(SIG_SETMASK, &((mctx)->sigs), NULL)
#else
#define pth_mctx_restored(mctx) \
        /*nop*/
#endif

/*
 * switch from one machine context to another
 */
#define SWITCH_DEBUG_LINE \
        "==== THREAD CONTEXT SWITCH ==========================================="
#ifdef PTH_DEBUG
#define  _pth_mctx_switch_debug pth_debug(NULL, 0, 1, SWITCH_DEBUG_LINE);
#else
#define  _pth_mctx_switch_debug /*NOP*/
#endif
#if PTH_MCTX_MTH(mcsc)
#define pth_mctx_switch(old,new) \
    _pth_mctx_switch_debug \
    swapcontext(&((old)->uc), &((new)->uc));
#elif PTH_MCTX_MTH(sjlj)
#define pth_mctx_switch(old,new) \
    _pth_mctx_switch_debug \
    if (pth_mctx_save(old) == 0) \
        pth_mctx_restore(new); \
    pth_mctx_restored(old);
#else
#error "unknown mctx method"
#endif

#line 31 "pth_clean.c"

typedef struct pth_cleanup_st pth_cleanup_t;
struct pth_cleanup_st {
    pth_cleanup_t *next;
    void (*func)(void *);
    void *arg;
};

#line 31 "pth_time.c"
#define PTH_TIME_NOW  (pth_time_t *)(0)
#define PTH_TIME_ZERO &pth_time_zero
#define PTH_TIME(sec,usec) { sec, usec }
#define pth_time_equal(t1,t2) \
        (((t1).tv_sec == (t2).tv_sec) && ((t1).tv_usec == (t2).tv_usec))
#line 57 "pth_time.c"
#if defined(HAVE_GETTIMEOFDAY_ARGS1)
#define __gettimeofday(t) gettimeofday(t)
#else
#define __gettimeofday(t) gettimeofday(t, NULL)
#endif
#define pth_time_set(t1,t2) \
    do { \
        if ((t2) == PTH_TIME_NOW) \
            __gettimeofday((t1)); \
        else { \
            (t1)->tv_sec  = (t2)->tv_sec; \
            (t1)->tv_usec = (t2)->tv_usec; \
        } \
    } while (0)
#line 109 "pth_time.c"
#define pth_time_add(t1,t2) \
    (t1)->tv_sec  += (t2)->tv_sec; \
    (t1)->tv_usec += (t2)->tv_usec; \
    if ((t1)->tv_usec > 1000000) { \
        (t1)->tv_sec  += 1; \
        (t1)->tv_usec -= 1000000; \
    }
#line 120 "pth_time.c"
#define pth_time_sub(t1,t2) \
    (t1)->tv_sec  -= (t2)->tv_sec; \
    (t1)->tv_usec -= (t2)->tv_usec; \
    if ((t1)->tv_usec < 0) { \
        (t1)->tv_sec  -= 1; \
        (t1)->tv_usec += 1000000; \
    }
#line 29 "pth_tcb.c"

#define PTH_TCB_NAMELEN 40

    /* thread control block */
struct pth_st {
    /* priority queue handling */
    pth_t          q_next;               /* next thread in pool                         */
    pth_t          q_prev;               /* previous thread in pool                     */
    int            q_prio;               /* (relative) priority of thread when queued   */

    /* standard thread control block ingredients */
    int            prio;                 /* base priority of thread                     */
    char           name[PTH_TCB_NAMELEN];/* name of thread (mainly for debugging)       */
    int            dispatches;           /* total number of thread dispatches           */
    pth_state_t    state;                /* current state indicator for thread          */

    /* timing */
    pth_time_t     spawned;              /* time point at which thread was spawned      */
    pth_time_t     lastran;              /* time point at which thread was last running */
    pth_time_t     running;              /* time range the thread was already running   */

    /* event handling */
    pth_event_t    events;               /* events the tread is waiting for             */

    /* per-thread signal handling */
    sigset_t       sigpending;           /* set    of pending signals                   */
    int            sigpendcnt;           /* number of pending signals                   */

    /* machine context */
    pth_mctx_t     mctx;                 /* last saved machine state of thread          */
    char          *stack;                /* pointer to thread stack                     */
    unsigned int   stacksize;            /* size of thread stack                        */
    long          *stackguard;           /* stack overflow guard                        */
    int            stackloan;            /* stack type                                  */
    void        *(*start_func)(void *);  /* start routine                               */
    void          *start_arg;            /* start argument                              */

    /* thread joining */
    int            joinable;             /* whether thread is joinable                  */
    void          *join_arg;             /* joining argument                            */

    /* per-thread specific storage */
    const void   **data_value;           /* thread specific  values                     */
    int            data_count;           /* number of stored values                     */

    /* cancellation support */
    int            cancelreq;            /* cancellation request is pending             */
    unsigned int   cancelstate;          /* cancellation state of thread                */
    pth_cleanup_t *cleanups;             /* stack of thread cleanup handlers            */

    /* mutex ring */
    pth_ring_t     mutexring;            /* ring of aquired mutex structures            */

#ifdef PTH_EX
    /* per-thread exception handling */
    ex_ctx_t       ex_ctx;               /* exception handling context                  */
#endif
};

#line 31 "pth_util.c"
#define pth_util_min(a,b) \
        ((a) > (b) ? (b) : (a))
#line 30 "pth_pqueue.c"

/* thread priority queue */
struct pth_pqueue_st {
    pth_t q_head;
    int   q_num;
};
typedef struct pth_pqueue_st pth_pqueue_t;

#line 165 "pth_pqueue.c"
#define pth_pqueue_favorite_prio(q) \
    ((q)->q_head != NULL ? (q)->q_head->q_prio + 1 : PTH_PRIO_MAX)
#line 199 "pth_pqueue.c"
#define pth_pqueue_elements(q) \
    ((q) == NULL ? (-1) : (q)->q_num)
#line 205 "pth_pqueue.c"
#define pth_pqueue_head(q) \
    ((q) == NULL ? NULL : (q)->q_head)
#line 31 "pth_event.c"

/* pre-declare type of function event callback
   (mainly to workaround va_arg(3) problems below) */
typedef int (*pth_event_func_t)(void *);

/* event structure */
struct pth_event_st {
    struct pth_event_st *ev_next;
    struct pth_event_st *ev_prev;
    pth_status_t ev_status;
    int ev_type;
    int ev_goal;
    union {
        struct { int fd; }                                          FD;
        struct { int *n; int nfd; fd_set *rfds, *wfds, *efds; }     SELECT;
        struct { sigset_t *sigs; int *sig; }                        SIGS;
        struct { pth_time_t tv; }                                   TIME;
        struct { pth_msgport_t mp; }                                MSG;
        struct { pth_mutex_t *mutex; }                              MUTEX;
        struct { pth_cond_t *cond; }                                COND;
        struct { pth_t tid; }                                       TID;
        struct { pth_event_func_t func; void *arg; pth_time_t tv; } FUNC;
    } ev_args;
};

#line 30 "pth_msg.c"

/* message port structure */
struct pth_msgport_st {
    pth_ringnode_t mp_node;  /* maintainance node handle */
    const char    *mp_name;  /* optional name of message port */
    pth_t          mp_tid;   /* corresponding thread */
    pth_ring_t     mp_queue; /* queue of messages pending on port */
};

#line 30 "pth_attr.c"

enum {
    PTH_ATTR_GET,
    PTH_ATTR_SET
};

struct pth_attr_st {
    pth_t        a_tid;
    int          a_prio;
    int          a_dispatches;
    char         a_name[PTH_TCB_NAMELEN];
    int          a_joinable;
    unsigned int a_cancelstate;
    unsigned int a_stacksize;
    char        *a_stackaddr;
};

#line 43 "pth_lib.c"
#define pth_implicit_init() \
    if (!pth_initialized) \
        pth_init();
#line 1 "_ON_THE_FLY_"

/* make sure the scpp source extensions are skipped */
#define cpp 0
#define intern /**/

/* move intern variables to hidden namespace */
#define pth_syscall_fct_tab __pth_syscall_fct_tab
#define pth_errno_storage __pth_errno_storage
#define pth_errno_flag __pth_errno_flag
#define pth_time_zero __pth_time_zero
#define pth_state_names __pth_state_names
#define pth_main __pth_main
#define pth_sched __pth_sched
#define pth_current __pth_current
#define pth_NQ __pth_NQ
#define pth_RQ __pth_RQ
#define pth_WQ __pth_WQ
#define pth_SQ __pth_SQ
#define pth_DQ __pth_DQ
#define pth_favournew __pth_favournew
#define pth_loadval __pth_loadval
#define pth_initialized __pth_initialized

/* move intern functions to hidden namespace */
#define pth_debug __pth_debug
#define pth_dumpstate __pth_dumpstate
#define pth_dumpqueue __pth_dumpqueue
#define pth_syscall_init __pth_syscall_init
#define pth_syscall_kill __pth_syscall_kill
#define pth_sc_fork __pth_sc_fork
#define pth_sc_sigprocmask __pth_sc_sigprocmask
#define pth_sc_waitpid __pth_sc_waitpid
#define pth_sc_connect __pth_sc_connect
#define pth_sc_accept __pth_sc_accept
#define pth_sc_select __pth_sc_select
#define pth_sc_read __pth_sc_read
#define pth_sc_write __pth_sc_write
#define pth_sc_readv __pth_sc_readv
#define pth_sc_writev __pth_sc_writev
#define pth_sc_recv __pth_sc_recv
#define pth_sc_send __pth_sc_send
#define pth_sc_recvfrom __pth_sc_recvfrom
#define pth_sc_sendto __pth_sc_sendto
#define pth_ring_init __pth_ring_init
#define pth_ring_insert_after __pth_ring_insert_after
#define pth_ring_insert_before __pth_ring_insert_before
#define pth_ring_delete __pth_ring_delete
#define pth_ring_prepend __pth_ring_prepend
#define pth_ring_append __pth_ring_append
#define pth_ring_pop __pth_ring_pop
#define pth_ring_favorite __pth_ring_favorite
#define pth_ring_dequeue __pth_ring_dequeue
#define pth_ring_contains __pth_ring_contains
#define pth_mctx_set __pth_mctx_set
#define pth_mctx_set __pth_mctx_set
#define pth_mctx_set __pth_mctx_set
#define pth_mctx_set __pth_mctx_set
#define pth_mctx_set __pth_mctx_set
#define pth_mctx_set __pth_mctx_set
#define pth_cleanup_popall __pth_cleanup_popall
#define pth_time_usleep __pth_time_usleep
#define pth_time_cmp __pth_time_cmp
#define pth_time_div __pth_time_div
#define pth_time_mul __pth_time_mul
#define pth_time_t2d __pth_time_t2d
#define pth_time_t2i __pth_time_t2i
#define pth_time_pos __pth_time_pos
#define pth_tcb_alloc __pth_tcb_alloc
#define pth_tcb_free __pth_tcb_free
#define pth_util_sigdelete __pth_util_sigdelete
#define pth_util_cpystrn __pth_util_cpystrn
#define pth_util_fd_valid __pth_util_fd_valid
#define pth_util_fds_merge __pth_util_fds_merge
#define pth_util_fds_test __pth_util_fds_test
#define pth_util_fds_select __pth_util_fds_select
#define pth_pqueue_init __pth_pqueue_init
#define pth_pqueue_insert __pth_pqueue_insert
#define pth_pqueue_delmax __pth_pqueue_delmax
#define pth_pqueue_delete __pth_pqueue_delete
#define pth_pqueue_favorite __pth_pqueue_favorite
#define pth_pqueue_increase __pth_pqueue_increase
#define pth_pqueue_tail __pth_pqueue_tail
#define pth_pqueue_walk __pth_pqueue_walk
#define pth_pqueue_contains __pth_pqueue_contains
#define pth_sched_housekeeping __pth_sched_housekeeping
#define pth_sched_preempt __pth_sched_preempt
#define pth_sched_enable_preempt __pth_sched_enable_preempt
#define pth_sched_bootstrap __pth_sched_bootstrap
#define pth_scheduler_init __pth_scheduler_init
#define pth_scheduler_drop __pth_scheduler_drop
#define pth_scheduler_kill __pth_scheduler_kill
#define pth_scheduler __pth_scheduler
#define pth_sched_eventmanager __pth_sched_eventmanager
#define pth_sched_eventmanager_sighandler __pth_sched_eventmanager_sighandler
#define pth_key_destroydata __pth_key_destroydata
#define pth_mutex_releaseall __pth_mutex_releaseall
#define pth_attr_ctrl __pth_attr_ctrl
#define pth_thread_exists __pth_thread_exists
#define pth_thread_cleanup __pth_thread_cleanup
#define pth_readv_faked __pth_readv_faked
#define pth_writev_iov_bytes __pth_writev_iov_bytes
#define pth_writev_iov_advance __pth_writev_iov_advance
#define pth_writev_faked __pth_writev_faked
#define pth_vsnprintf __pth_vsnprintf
#define pth_snprintf __pth_snprintf
#define pth_vasprintf __pth_vasprintf
#define pth_asprintf __pth_asprintf

/* prototypes for intern variables */
#line 136 "pth_syscall.c"
extern pth_syscall_fct_tab_t pth_syscall_fct_tab[];
#line 54 "pth_errno.c"
extern int pth_errno_storage;
#line 55 "pth_errno.c"
extern int pth_errno_flag;
#line 40 "pth_time.c"
extern pth_time_t pth_time_zero;
#line 91 "pth_tcb.c"
extern const char *pth_state_names[];
#line 33 "pth_sched.c"
extern pth_t pth_main;
#line 34 "pth_sched.c"
extern pth_t pth_sched;
#line 35 "pth_sched.c"
extern pth_t pth_current;
#line 36 "pth_sched.c"
extern pth_pqueue_t pth_NQ;
#line 37 "pth_sched.c"
extern pth_pqueue_t pth_RQ;
#line 38 "pth_sched.c"
extern pth_pqueue_t pth_WQ;
#line 39 "pth_sched.c"
extern pth_pqueue_t pth_SQ;
#line 40 "pth_sched.c"
extern pth_pqueue_t pth_DQ;
#line 41 "pth_sched.c"
extern int pth_favournew;
#line 42 "pth_sched.c"
extern float pth_loadval;
#line 42 "pth_lib.c"
extern int pth_initialized;

/* prototypes for intern functions */
#line 55 "pth_debug.c"
extern void pth_debug(const char *, int, int, const char *, ...);
#line 81 "pth_debug.c"
extern void pth_dumpstate(FILE *);
#line 98 "pth_debug.c"
extern void pth_dumpqueue(FILE *, const char *, pth_pqueue_t *);
#line 187 "pth_syscall.c"
extern void pth_syscall_init(void);
#line 258 "pth_syscall.c"
extern void pth_syscall_kill(void);
#line 298 "pth_syscall.c"
extern pid_t pth_sc_fork(void);
#line 364 "pth_syscall.c"
extern int pth_sc_sigprocmask(int, const sigset_t *, sigset_t *);
#line 399 "pth_syscall.c"
extern pid_t pth_sc_waitpid(pid_t, int *, int);
#line 421 "pth_syscall.c"
extern int pth_sc_connect(int, const struct sockaddr *, socklen_t);
#line 451 "pth_syscall.c"
extern int pth_sc_accept(int, struct sockaddr *, socklen_t *);
#line 482 "pth_syscall.c"
extern int pth_sc_select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
#line 530 "pth_syscall.c"
extern ssize_t pth_sc_read(int, void *, size_t);
#line 552 "pth_syscall.c"
extern ssize_t pth_sc_write(int, const void *, size_t);
#line 574 "pth_syscall.c"
extern ssize_t pth_sc_readv(int, const struct iovec *, int);
#line 596 "pth_syscall.c"
extern ssize_t pth_sc_writev(int, const struct iovec *, int);
#line 640 "pth_syscall.c"
extern ssize_t pth_sc_recv(int, void *, size_t, int);
#line 664 "pth_syscall.c"
extern ssize_t pth_sc_send(int, void *, size_t, int);
#line 688 "pth_syscall.c"
extern ssize_t pth_sc_recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
#line 710 "pth_syscall.c"
extern ssize_t pth_sc_sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
#line 45 "pth_ring.c"
extern void pth_ring_init(pth_ring_t *);
#line 91 "pth_ring.c"
extern void pth_ring_insert_after(pth_ring_t *, pth_ringnode_t *, pth_ringnode_t *);
#line 104 "pth_ring.c"
extern void pth_ring_insert_before(pth_ring_t *, pth_ringnode_t *, pth_ringnode_t *);
#line 117 "pth_ring.c"
extern void pth_ring_delete(pth_ring_t *, pth_ringnode_t *);
#line 134 "pth_ring.c"
extern void pth_ring_prepend(pth_ring_t *, pth_ringnode_t *);
#line 155 "pth_ring.c"
extern void pth_ring_append(pth_ring_t *, pth_ringnode_t *);
#line 181 "pth_ring.c"
extern pth_ringnode_t *pth_ring_pop(pth_ring_t *);
#line 192 "pth_ring.c"
extern int pth_ring_favorite(pth_ring_t *, pth_ringnode_t *);
#line 214 "pth_ring.c"
extern pth_ringnode_t *pth_ring_dequeue(pth_ring_t *);
#line 225 "pth_ring.c"
extern int pth_ring_contains(pth_ring_t *, pth_ringnode_t *);
#line 159 "pth_mctx.c"
extern int pth_mctx_set(pth_mctx_t *, void (*)(void), char *, char *);
#line 231 "pth_mctx.c"
extern int pth_mctx_set(pth_mctx_t *, void (*)(void), char *, char *);
#line 464 "pth_mctx.c"
extern int pth_mctx_set(pth_mctx_t *, void (*)(void), char *, char *);
#line 498 "pth_mctx.c"
extern int pth_mctx_set(pth_mctx_t *, void (*)(void), char *, char *);
#line 522 "pth_mctx.c"
extern int pth_mctx_set(pth_mctx_t *, void (*)(void), char *, char *);
#line 543 "pth_mctx.c"
extern int pth_mctx_set(pth_mctx_t *, void (*)(void), char *, char *);
#line 73 "pth_clean.c"
extern void pth_cleanup_popall(pth_t, int);
#line 43 "pth_time.c"
extern void pth_time_usleep(unsigned long);
#line 98 "pth_time.c"
extern int pth_time_cmp(pth_time_t *, pth_time_t *);
#line 131 "pth_time.c"
extern void pth_time_div(pth_time_t *, int);
#line 147 "pth_time.c"
extern void pth_time_mul(pth_time_t *, int);
#line 157 "pth_time.c"
extern double pth_time_t2d(pth_time_t *);
#line 166 "pth_time.c"
extern int pth_time_t2i(pth_time_t *);
#line 175 "pth_time.c"
extern int pth_time_pos(pth_time_t *);
#line 103 "pth_tcb.c"
extern pth_t pth_tcb_alloc(unsigned int, void *);
#line 137 "pth_tcb.c"
extern void pth_tcb_free(pth_t);
#line 42 "pth_util.c"
extern int pth_util_sigdelete(int);
#line 78 "pth_util.c"
extern char *pth_util_cpystrn(char *, const char *, size_t);
#line 95 "pth_util.c"
extern int pth_util_fd_valid(int);
#line 105 "pth_util.c"
extern void pth_util_fds_merge(int, fd_set *, fd_set *, fd_set *, fd_set *, fd_set *, fd_set *);
#line 127 "pth_util.c"
extern int pth_util_fds_test(int, fd_set *, fd_set *, fd_set *, fd_set *, fd_set *, fd_set *);
#line 153 "pth_util.c"
extern int pth_util_fds_select(int, fd_set *, fd_set *, fd_set *, fd_set *, fd_set *, fd_set *);
#line 42 "pth_pqueue.c"
extern void pth_pqueue_init(pth_pqueue_t *);
#line 52 "pth_pqueue.c"
extern void pth_pqueue_insert(pth_pqueue_t *, int, pth_t);
#line 97 "pth_pqueue.c"
extern pth_t pth_pqueue_delmax(pth_pqueue_t *);
#line 128 "pth_pqueue.c"
extern void pth_pqueue_delete(pth_pqueue_t *, pth_t);
#line 171 "pth_pqueue.c"
extern int pth_pqueue_favorite(pth_pqueue_t *, pth_t);
#line 187 "pth_pqueue.c"
extern void pth_pqueue_increase(pth_pqueue_t *);
#line 211 "pth_pqueue.c"
extern pth_t pth_pqueue_tail(pth_pqueue_t *);
#line 221 "pth_pqueue.c"
extern pth_t pth_pqueue_walk(pth_pqueue_t *, pth_t, int);
#line 241 "pth_pqueue.c"
extern int pth_pqueue_contains(pth_pqueue_t *, pth_t);
#line 56 "pth_sched.c"
extern void pth_sched_housekeeping(void);
#line 168 "pth_sched.c"
extern void pth_sched_preempt(int);
#line 196 "pth_sched.c"
extern void pth_sched_enable_preempt(void);
#line 233 "pth_sched.c"
extern void pth_sched_bootstrap(void);
#line 249 "pth_sched.c"
extern int pth_scheduler_init(void);
#line 285 "pth_sched.c"
extern void pth_scheduler_drop(void);
#line 317 "pth_sched.c"
extern void pth_scheduler_kill(void);
#line 358 "pth_sched.c"
extern void *pth_scheduler(void *);
#line 438 "pth_sched.c"
extern void pth_sched_eventmanager(pth_time_t *, int);
#line 897 "pth_sched.c"
extern void pth_sched_eventmanager_sighandler(int);
#line 95 "pth_data.c"
extern void pth_key_destroydata(pth_t);
#line 128 "pth_sync.c"
extern void pth_mutex_releaseall(pth_t);
#line 119 "pth_attr.c"
extern int pth_attr_ctrl(int, pth_attr_t, int, va_list);
#line 376 "pth_lib.c"
extern int pth_thread_exists(pth_t);
#line 388 "pth_lib.c"
extern void pth_thread_cleanup(pth_t);
#line 955 "pth_high.c"
extern ssize_t pth_readv_faked(int, const struct iovec *, int);
#line 1138 "pth_high.c"
extern ssize_t pth_writev_iov_bytes(const struct iovec *, int);
#line 1153 "pth_high.c"
extern void pth_writev_iov_advance(const struct iovec *, int, size_t, struct iovec **, int *, struct iovec *, int);
#line 1191 "pth_high.c"
extern ssize_t pth_writev_faked(int, const struct iovec *, int);
#line 647 "pth_string.c"
extern int pth_vsnprintf(char *, size_t, const char *, va_list);
#line 662 "pth_string.c"
extern int pth_snprintf(char *, size_t, const char *, ...);
#line 678 "pth_string.c"
extern char * pth_vasprintf(const char *, va_list);
#line 693 "pth_string.c"
extern char * pth_asprintf(const char *, ...);
#line 131 "pth_p.h.in"
END_DECLARATION

#endif /* _PTH_P_H_ */

