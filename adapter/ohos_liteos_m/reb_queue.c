#include "reb_def.h"
#include "los_task.h"
#include "los_queue.h"

reb_queue_id reb_queue_create(int size, int count)
{
    UINT32 ret;
    UINT32 queue;

    ret = LOS_QueueCreate((char *)NULL, (UINT16)count, &queue, 0, (UINT16)size);
    if (ret != LOS_OK) 
    {
        return (reb_queue_id)NULL;
    }
    return (reb_queue_id)(GET_QUEUE_HANDLE(queue));
}

reb_status reb_queue_send(reb_queue_id queue, const void *msg,
                          uint32_t size, reb_time_t timeout)
{
    UINT32 ret;
    if (queue == NULL) {
        return REB_INVAL;
    }
    ret = LOS_QueueWriteCopy(((LosQueueCB *)queue)->queueID, msg, size, timeout);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

reb_status reb_queue_urgent_send(reb_queue_id queue, const void *msg,
                                 uint32_t size, reb_time_t timeout)
{
    UINT32 ret;
    if (queue == NULL) {
        return REB_INVAL;
    }
    ret = LOS_QueueWriteHeadCopy(((LosQueueCB *)queue)->queueID, msg, size, timeout);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

reb_status reb_queue_recv(reb_queue_id queue, void *msg, uint32_t size)
{
    UINT32 ret;
    if (queue == NULL) {
        return REB_INVAL;
    }
    ret = LOS_QueueReadCopy(((LosQueueCB *)queue)->queueID, msg, size, LOS_WAIT_FOREVER);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

void reb_queue_delete(reb_queue_id queue)
{
    if (queue == NULL) {
        return;
    }
    LOS_QueueDelete(((LosQueueCB *)queue)->queueID);
}
