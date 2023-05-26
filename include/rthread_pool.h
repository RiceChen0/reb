#ifndef __TP_MANAGE_H__
#define __TP_MANAGE_H__

#include "platform_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RTP_NAME_LEN        16
#define RTP_PRIORITY        10

typedef void (*task_handle)(void *argv);

typedef struct rtp_task_ {
    task_handle handle;
    void *argv;
    struct rtp_task_ *next;
}rtp_task;

typedef struct {
    struct pf_task_attr attr;
    pf_task_id *thread_id;
}rtp_thread_info;

typedef struct {
    pf_mutex_id queue_lock;
    pf_sem_id queue_ready;
    rtp_thread_info *threads;
    rtp_task *task_queue;
    uint8_t thread_num;
    uint8_t wait_task_num;
}rtp;

/**
 * Create thread pool
 * 
 * @param pool      thread pool handle
 * @param name      thread name
 * @param stackSize thread stack size
 * @param threadNum Number of threads in thread pool
 * @return create thread pool result. 
 *         TP_EOK: init success
 *         TP_ERROR: init failed
*/
pf_err_t rtp_create(rtp *pool, const char *name, 
                   uint32_t stackSize, uint8_t threadNum);

/**
 * Adding tasks to the thread pool
 * 
 * @param pool thread pool handle
 * @param handl task handler
 * @param argv task parameter
 * @return add task result. 
 *         TP_EOK: init success
 *         TP_ERROR: init failed
*/
pf_err_t rtp_add_task(rtp *pool, task_handle handle, void *argv);

/**
 * Destroy thread pool
 * 
 * @param pool thread pool handle
 * @return destroy thread pool result. 
 *         TP_EOK: init success
 *         TP_ERROR: init failed
*/
pf_err_t rtp_destroy(rtp *pool);

#ifdef __cplusplus
}
#endif

#endif
