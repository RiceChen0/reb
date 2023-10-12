#include "reb_def.h"
#include "cmsis_os2.h"

reb_queue_id reb_queue_create(int size, int count)
{
    osMessageQueueId_t queue = NULL;
        osMessageQueueAttr_t queue_attr = {
        .name = "reb", 
        .attr_bits = 0, 
        .cb_mem = NULL, 
        .cb_size = 0, 
        .mq_mem = NULL, 
        .mq_size = 0};

    queue = osMessageQueueNew(size, count, &queue_attr);
    return (reb_queue_id)queue;
}

reb_status reb_queue_send(reb_queue_id queue, const void *msg,
                          uint32_t size, reb_time_t timeout)
{
    osStatus_t ret = osMessageQueuePut((osMessageQueueId_t)queue, msg, 0, timeout);
    if (ret != osOK) {
        return REB_ERROR;
    }
    return REB_OK;
}

reb_status reb_queue_urgent_send(reb_queue_id queue, const void *msg,
                                 uint32_t size, reb_time_t timeout)
{
    osStatus_t ret = osMessageQueueGet((osMessageQueueId_t)queue, msg, 1, 0);
    if (ret != osOK) {
        return REB_ERROR;
    }
    return REB_OK;
}

reb_status reb_queue_recv(reb_queue_id queue, void *msg, uint32_t size)
{
    osStatus_t ret = osMessageQueueGet((osMessageQueueId_t)queue, msg, 0, osWaitForever);
    if(ret != osOK) {
        return REB_ERROR;
    }
    return REB_OK;
}

void reb_queue_delete(reb_queue_id queue)
{
    if(queue != NULL) {
        osMessageQueueDelete((osMessageQueueId_t)queue);
    }
}
