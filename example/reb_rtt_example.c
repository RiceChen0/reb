#include "rtthread.h"
#include "reb_broker.h"
#include "reb_observer.h"
#include "reb_publisher.h"

observer_base *obs_signal;
observer_base *obs_call;
observer_base *obs_task;

void sig_thread_handle(void *arg)
{
    while(1) {
        if(observer_signal_wait(obs_signal, RT_WAITING_FOREVER) == REB_OK) {
            rt_kprintf("signal: recv success\r\n");        
        }
    }
}

void obs_callback(uint32_t event, uint32_t data, void *arg)
{
    rt_kprintf("call: event: 0x%08x, data: %s\r\n", event, (char *)data);
}

void obs_task_fun(uint32_t event, uint32_t data, void *arg)
{
    rt_kprintf("task: event: 0x%08x, data: %s\r\n", event, (char *)data);
}

int reb_init(void)
{
    rt_thread_t signal_thread = NULL;

    broker_create();

    obs_signal = observer_signal_create(1, REB_ALL_MINOR_TYPE);

    signal_thread = rt_thread_create("sig_thread", sig_thread_handle, NULL, 1024, 10, 20);
    rt_thread_startup(signal_thread);

    obs_call = observer_callback_create(1, REB_ALL_MINOR_TYPE, obs_callback, NULL);

    obs_task = observer_task_create(1, REB_ALL_MINOR_TYPE, obs_task_fun, NULL, 1024, 15);

    broker_observer_attach(obs_signal);
    broker_observer_attach(obs_call);
    broker_observer_attach_once(obs_task);

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(reb_init);

int reb_test(void)
{
    char *data = "RiceChen";
    publisher_send(1, 1, (int)data, 1000);
    publisher_send(1, 2, (int)data, 1000);
    publisher_send(1, 3, (int)data, 1000);
}
MSH_CMD_EXPORT(reb_test, Rice Event broker test);

