/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#ifndef __REB_OBSERVER_H__
#define __REB_OBSERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "reb_def.h"

typedef enum {
    OBS_MODE_SIGNAL,
    OBS_MODE_CALLBACK,
    OBS_MODE_TASK,
} observer_mode;

typedef enum {
    OBS_STATE_IDLE,
    OBS_STATE_ATTACH,
    OBS_STATE_ATTACH_ONCE,
    OBS_STATE_DETACH,
} observer_state;

typedef struct observer_base observer_base;

typedef void (*obs_trigger_cb)(observer_base *base, uint32_t event, uint32_t data);

struct observer_base{
    reb_list_t node;
    uint32_t event;
    int state;
    int mode;
    obs_trigger_cb trigger;
    void *arg;
};

/**
 * Observer signal mode
 */

typedef struct {
    observer_base base;
    reb_sem_id signal;
} observer_signal;

observer_base *observer_signal_create(uint16_t type, uint16_t sub_type);
reb_status observer_signal_wait(observer_base *base, reb_time_t timeout);

/**
 * Observer callback mode
 */

typedef void (*obs_callback_cb)(uint32_t event, uint32_t data, void *arg);

typedef struct {
    observer_base base;
    obs_callback_cb cb;
} observer_callback;

observer_base *observer_callback_create(uint16_t type,
                                        uint16_t sub_type,
                                        obs_callback_cb cb,
                                        void *arg);

/**
 * Observer task mode
 */

typedef void (*obs_task_cb)(uint32_t event, uint32_t data, void *arg);

typedef struct {
    observer_base base;
    reb_task_id task;
    obs_task_cb run;
    uint32_t stack_size;
    uint32_t prio;
    uint32_t event;
    uint32_t data;
} observer_task;

observer_base *observer_task_create(uint16_t type,
                                    uint16_t sub_type,
                                    obs_task_cb run,
                                    void *arg,
                                    uint32_t stack_size,
                                    uint32_t prio);

reb_status observer_delete(observer_base *base);

#ifdef __cplusplus
}
#endif

#endif
