/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#include "reb_observer.h"

static int observer_create(observer_base *base, uint32_t event,
                           obs_trigger_cb trigger, void *arg)
{
    reb_list_init(&base->node);
    base->event = event;
    base->trigger = trigger;
    base->arg = arg;
    return REB_OK;
}

static void observer_signal_trigger(observer_base *base, uint32_t event, uint32_t data)
{
    observer_signal *obs = reb_container_of(base, observer_signal, base);
    reb_sem_unlock(obs->signal);
}

observer_base *observer_signal_create(uint16_t type, uint16_t sub_type)
{
    observer_signal *obs = NULL;
    
    obs = REB_MALLOC(sizeof(observer_signal));
    if(obs == NULL) {
        REB_LOGE("Observer: Failed to create observer for signal mode, event type: 0x%08x", REB_MK_EVENT_TYPE(type, sub_type));
        return NULL;
    }
    memset(obs, 0, sizeof(observer_signal));

    observer_create(&obs->base, 
                    REB_MK_EVENT_TYPE(type, sub_type), 
                    observer_signal_trigger, NULL);
    obs->base.mode = OBS_MODE_SIGNAL;
    obs->signal = reb_sem_create(0);
    if(obs->signal == NULL) {
        REB_LOGE("Observer: create sem failed");
        REB_FREE(obs);
        obs = NULL;
        return NULL;
    }

    REB_LOGD("Observer: create signal mode success, event type: 0x%08x", REB_MK_EVENT_TYPE(type, sub_type));

    return &obs->base;
}

reb_status observer_signal_wait(observer_base *base, reb_time_t timeout)
{
    observer_signal *obs = reb_container_of(base, observer_signal, base);
    return reb_sem_lock(obs->signal, timeout);
}

static void observer_callback_trigger(observer_base *base, uint32_t event, uint32_t data)
{
    observer_callback *obs = reb_container_of(base, observer_callback, base);

    if(obs->cb) {
        obs->cb(event, data, obs->base.arg);
    }
}

observer_base *observer_callback_create(uint16_t type,
                                        uint16_t sub_type,
                                        obs_callback_cb cb,
                                        void *arg)
{
    observer_callback *obs = NULL;
    
    obs = REB_MALLOC(sizeof(observer_callback));
    if(obs == NULL) {
        REB_LOGE("Observer: Failed to create observer for callback mode, event type: 0x%08x", REB_MK_EVENT_TYPE(type, sub_type));
        return NULL;
    }
    memset(obs, 0, sizeof(observer_callback));

    observer_create(&obs->base, 
                    REB_MK_EVENT_TYPE(type, sub_type), 
                    observer_callback_trigger, arg);
    obs->base.mode = OBS_MODE_CALLBACK;
    obs->cb = cb;

    REB_LOGD("Observer: create callback mode success, event type: 0x%08x", REB_MK_EVENT_TYPE(type, sub_type));

    return &obs->base;
}

void task_trigger_handle(void *arg)
{
    observer_task *obs = (observer_task *)arg;

    obs->run(obs->event, obs->data, obs->base.arg);
}

static void observer_task_trigger(observer_base *base, uint32_t event, uint32_t data)
{
    observer_task *obs = reb_container_of(base, observer_task, base);
    reb_task_attr task_attr = {
        .name = "obs_task",
        .priority = obs->prio,
        .stack_size = obs->stack_size,
    };

    if(obs->task != NULL) {
        return;
    }

    obs->event = event;
    obs->data = data;

    if(obs->task) {
        REB_LOGE("Observer: Task still running");
        return;
    }

    obs->task = reb_task_create(task_trigger_handle, obs, &task_attr);
    if(obs->task == NULL) {
        REB_LOGE("Observer: create task failed");
        return;
    }
}

observer_base *observer_task_create(uint16_t type,
                                    uint16_t sub_type,
                                    obs_task_cb run,
                                    void *arg,
                                    uint32_t stack_size,
                                    uint32_t prio)
{
    observer_task *obs = NULL;
    
    obs = REB_MALLOC(sizeof(observer_task));
    if(obs == NULL) {
        REB_LOGE("Observer: Failed to create observer for task mode, event type: 0x%08x", REB_MK_EVENT_TYPE(type, sub_type));
        return NULL;
    }
    memset(obs, 0, sizeof(observer_task));

    observer_create(&obs->base, 
                    REB_MK_EVENT_TYPE(type, sub_type), 
                    observer_task_trigger, arg);
    obs->base.mode = OBS_MODE_TASK;
    obs->run = run;
    obs->stack_size = stack_size;
    obs->prio = prio;

    REB_LOGD("Observer: create task mode success, event type: 0x08x", REB_MK_EVENT_TYPE(type, sub_type));

    return &obs->base;
}

reb_status observer_delete(observer_base *base)
{
    if(base == NULL) {
        return REB_INVAL;
    }
    if(base->state != OBS_STATE_IDLE) {
        return REB_ERROR;
    }
    REB_FREE(base);
    return REB_OK;
}
