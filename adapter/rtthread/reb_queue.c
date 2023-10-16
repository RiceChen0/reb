#include "reb_def.h"
#include "rtthread.h"

reb_queue_id reb_queue_create(int size, int count)
{
    rt_mq_t queue = NULL;

    queue = rt_mq_create("reb", size, count, RT_IPC_FLAG_FIFO);
    return (reb_queue_id)queue;
}

reb_status reb_queue_send(reb_queue_id queue, const void *msg,
                          uint32_t size, reb_time_t timeout)
{
    if(rt_mq_send_wait((rt_mq_t)queue, msg, size, timeout) == RT_EOK) {
        return REB_OK;
    }
    return REB_ERROR;
}

reb_status reb_queue_urgent_send(reb_queue_id queue, const void *msg,
                                 uint32_t size, reb_time_t timeout)
{
    if(rt_mq_urgent((rt_mq_t)queue, msg, size) == RT_EOK) {
        return REB_OK;
    }
    return REB_ERROR;
}

reb_status reb_queue_recv(reb_queue_id queue, void *msg, uint32_t size)
{
#if RTTHREAD_VERSION >= 50000
    if(rt_mq_recv((rt_mq_t)queue, msg, size, RT_WAITING_FOREVER) > 0) {
        return REB_OK;
    }
#else 
    if(rt_mq_recv((rt_mq_t)queue, msg, size, RT_WAITING_FOREVER) == RT_EOK) {
        return REB_OK;
    }
#endif
    return REB_ERROR;
}

void reb_queue_delete(reb_queue_id queue)
{
    if(queue != NULL) {
        rt_mq_delete((rt_mq_t)queue);
    }
}
