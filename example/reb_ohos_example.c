#include "shell.h"
#include "los_task.h"
#include "reb_broker.h"
#include "reb_observer.h"
#include "reb_publisher.h"

observer_base *obs_signal;
observer_base *obs_call;
observer_base *obs_task;

void sig_thread_handle(void *arg)
{
    while(1) {
        if(observer_signal_wait(obs_signal, (reb_time_t)LOS_WAIT_FOREVER) == REB_OK) {
            printf("signal: recv success\r\n");        
        }
    }
}

void obs_callback(uint32_t event, uint32_t data, void *arg)
{
    printf("call: event: 0x%08x, data: %s\r\n", event, (char *)data);
}

void obs_task_fun(uint32_t event, uint32_t data, void *arg)
{
    printf("task: event: 0x%08x, data: %s\r\n", event, (char *)data);
}

int reb_init(void)
{
    UINT32 task;
    TSK_INIT_PARAM_S param = {
        .pcName = "sig_task",
        .pfnTaskEntry = sig_thread_handle,
        .uwArg = NULL,
        .uwStackSize = 2048,
        .usTaskPrio = 20,
        .stackAddr = 0,
    };

    printf("reb init\r\n");

    broker_create();

    obs_signal = observer_signal_create(1, REB_ALL_MINOR_TYPE);

    LOS_TaskCreate(&task, &param);

    obs_call = observer_callback_create(1, REB_ALL_MINOR_TYPE, obs_callback, NULL);

    obs_task = observer_task_create(1, REB_ALL_MINOR_TYPE, obs_task_fun, NULL, 1024 * 2, 15);

    broker_observer_attach(obs_signal);
    broker_observer_attach(obs_call);
    broker_observer_attach(obs_task);

    return LOS_OK;
}

int reb_test(void)
{
    char *data = "RiceChen";
    publisher_send(1, 1, (int)data, 1000);
    publisher_send(1, 2, (int)data, 1000);
    publisher_send(1, 3, (int)data, 1000);
    return LOS_OK;
}

