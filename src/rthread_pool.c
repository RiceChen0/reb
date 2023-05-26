#include "rthread_pool.h"

static void rtp_handler(void *argv);

pf_err_t rtp_create(rtp *pool, const char *name, 
                   uint32_t stack_size, uint8_t thread_num)
{
    int index = 0;
    if(pool == NULL) {
        PF_LOGE("Thread pool handle is NULL");
        return PF_EINVAL;
    }

    if((pool->queue_lock = pf_mutex_create()) == NULL) {
        PF_LOGE("Create thread pool mutex failed");
        return PF_ERROR;
    }
    if((pool->queue_ready = pf_sem_create(0)) == NULL) {
        PF_LOGE("Create thread pool sem failed");
        return PF_ERROR;
    }
    pool->task_queue = NULL;
    pool->thread_num = thread_num;
    pool->wait_task_num = 0;
    pool->threads = (rtp_thread_info *)PF_MALLOC(thread_num * sizeof(rtp_thread_info));
    if(pool->threads == NULL) {
        PF_LOGE("Malloc thread pool info memory failed");
        return PF_ENOMEM;
    }
    for(index = 0; index < thread_num; index++) {
        pool->threads[index].attr.name = (char *)PF_MALLOC(RTP_NAME_LEN);
        if(pool->threads[index].attr.name == NULL) {
            PF_LOGE("Malloc thread name memory failed");
            return PF_ENOMEM;
        }
        snprintf(pool->threads[index].attr.name, RTP_NAME_LEN, "%s%d", name, index);
        pool->threads[index].attr.stack_size = stack_size;
        pool->threads[index].attr.priority = RTP_PRIORITY;
        pool->threads[index].thread_id = pf_task_create(rtp_handler, pool, &pool->threads[index].attr);
        
    }
    return PF_EOK;
}

pf_err_t rtp_add_task(rtp *pool, task_handle handle, void *argv)
{
    rtp_task *new_task = NULL;
    rtp_task *task_lIst = NULL;

    if(pool == NULL) {
        PF_LOGE("Thread pool handle is NULL");
        return PF_EINVAL;
    }

    new_task = (rtp_task *)PF_MALLOC(sizeof(rtp_task));
    if(new_task == NULL) {
        PF_LOGE("Malloc new task handle memory failed");
        return PF_ENOMEM;
    }
    new_task->handle = handle;
    new_task->argv = argv;
    new_task->next = NULL;

    pf_mutex_lock(pool->queue_lock);
    task_lIst = pool->task_queue;
    if(task_lIst == NULL) {
        pool->task_queue = new_task;
    }
    else {
        while(task_lIst->next != NULL) {
            task_lIst = task_lIst->next;
        }
        task_lIst->next = new_task;
    }
    pool->wait_task_num++;
    pf_mutex_unlock(pool->queue_lock);
    pf_sem_unlock(pool->queue_ready);
    return PF_EOK;
}

pf_err_t rtp_destroy(rtp *pool)
{
    int index = 0;
    rtp_task *head = NULL;

    if(pool == NULL) {
        PF_LOGE("Thread pool handle is NULL");
        return PF_EINVAL;
    }

    for(index = 0; index < pool->thread_num; index++) {
        pf_task_delete(pool->threads[index].thread_id);
        pool->threads[index].thread_id = NULL;
        PF_FREE(pool->threads[index].attr.name);
        pool->threads[index].attr.name = NULL;
    }
    pf_mutex_delete(pool->queue_lock);
    pool->queue_lock = NULL;
    pf_sem_delete(pool->queue_ready);
    pool->queue_ready = NULL;

    PF_FREE(pool->threads);
    pool->threads = NULL;

    while (pool->task_queue != NULL) {
        head = pool->task_queue;
        pool->task_queue = pool->task_queue->next;
        PF_FREE(head);
    }
    pool = NULL;
    return PF_EOK;
}

static void rtp_handler(void *argv)
{
    rtp *pool = (rtp *)argv;
    rtp_task *task = NULL;

    while(1) {
        pf_mutex_lock(pool->queue_lock);
        while(pool->wait_task_num == 0) {
            pf_mutex_unlock(pool->queue_lock);
            pf_sem_lock(pool->queue_ready);
            pf_mutex_lock(pool->queue_lock);
        }
        task = pool->task_queue;
        pool->wait_task_num--;
        pool->task_queue = task->next;
        pf_mutex_unlock(pool->queue_lock);
        task->handle(task->argv);
        PF_FREE(task);
        task = NULL;
    }
}
