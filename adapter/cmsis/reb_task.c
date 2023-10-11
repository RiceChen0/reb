#include "reb_def.h"
#include "cmsis_os2.h"

reb_task_id reb_task_create(reb_task_func func, void *arg, reb_task_attr *attr)
{
    osThreadId_t thread = NULL;
    osThreadAttr_t task_attr = {
        .name = attr->name,
        .attr_bits = 0,
        .cb_mem = NULL,
        .cb_size = 0,
        .stack_mem = NULL,
        .stack_size = attr->stack_size,
        .priority = (osPriority_t)attr->priority,
        .tz_module = 0,
        .reserved = 0,
    };

    thread = osThreadNew((osThreadFunc_t)func, arg, &task_attr);
    return (reb_task_id)thread;
}

reb_status reb_task_suspend(reb_task_id task)
{
    osThreadSuspend((osThreadId_t)task);
    return REB_OK;
}

reb_status reb_task_resume(reb_task_id task)
{
    osThreadResume((osThreadId_t)task);
    return REB_OK;
}

void reb_task_delete(reb_task_id task)
{
    if(task != NULL) {
        osThreadTerminate((osThreadId_t)task);
    }
}
