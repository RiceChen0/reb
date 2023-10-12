/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#include "reb_publisher.h"
#include "reb_cfg.h"

typedef struct {
    reb_task_id task;
    reb_queue_id queue;
    pub_notify notify;
} publisher_base;

typedef struct {
    uint32_t event;
    uint32_t data;
    bool free;
} publisher_msg;

static publisher_base base;

static void publisher_event_handle(void *arg)
{
    reb_status status;
    publisher_base *base = (publisher_base *)arg;
    publisher_msg msg = {0};

    while(1) {
        status = reb_queue_recv(base->queue, &msg, sizeof(publisher_msg));
        if(status != REB_OK) {
            continue;
        }

        if(base->notify) {
            base->notify(msg.event, msg.data, msg.free);
        }
        else {
            if(msg.free == true) {
                REB_FREE((void *)msg.data);
            }
        }
    };
}

reb_status publisher_factory_create(pub_notify notify)
{
    reb_task_attr task_attr = {
        .name = "reb_pub",
        .stack_size = REB_PUBLISHER_TASK_STACK_SIZE,
        .priority = REB_PUBLISHER_TASK_PRIO,
    };

    base.queue = reb_queue_create(sizeof(publisher_msg), REB_PUBLISHER_QUEUE_MSG_COUNT);
    if(base.queue == NULL) {
        REB_LOGE("Publisher: create queue failed");
        return REB_ERROR;
    }

    base.task = reb_task_create(publisher_event_handle, &base, &task_attr);
    if(base.task == NULL) {
        REB_LOGE("Publisher: create task failed");
        reb_queue_delete(base.queue);
        return REB_ERROR;
    }

    base.notify = notify;
    return REB_OK;
}

static reb_status _publisher_send(uint16_t type, uint16_t sub_type, uint32_t data,
                                  reb_time_t timeout, bool free)
{
    reb_status status;
    publisher_msg msg = {0};
    uint32_t event = REB_MK_EVENT_TYPE(type, sub_type);

    msg.event = event;
    msg.data = data;
    msg.free = free;

    return reb_queue_send(base.queue, &msg, sizeof(publisher_msg), timeout);
}

reb_status publisher_send(uint16_t type, uint16_t sub_type,
                          uint32_t data, reb_time_t timeout)
{
    _publisher_send(type, sub_type, data, timeout, false);
}

reb_status publisher_send_with_free(uint16_t type, uint16_t sub_type,
                                    uint32_t data, reb_time_t timeout)
{
    _publisher_send(type, sub_type, data, timeout, true);
}

static reb_status _publisher_urgent_send(uint16_t type, uint16_t sub_type, uint32_t data,
                                         reb_time_t timeout, bool free)
{
    publisher_msg msg = {0};
    uint32_t event = REB_MK_EVENT_TYPE(type, sub_type);

    msg.event = event;
    msg.data = data;
    msg.free = free;

    return reb_queue_urgent_send(base.queue, &msg, sizeof(publisher_msg), timeout);
}

reb_status publisher_urgent_send(uint16_t type, uint16_t sub_type,
                                 uint32_t data, reb_time_t timeout)
{
    return _publisher_urgent_send(type, sub_type, data, timeout, false);
}

reb_status publisher_urgent_send_with_free(uint16_t type, uint16_t sub_type,
                                           uint32_t data, reb_time_t timeout)
{
    return _publisher_urgent_send(type, sub_type, data, timeout, true);
}
