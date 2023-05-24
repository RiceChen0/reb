#include "reventbus_def.h"
#include "cmsis_os2.h"

reb_task_id reb_task_create(reb_task_func func, void *arg, const struct reb_task_attr *attr)
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

void reb_task_delete(reb_task_id thread)
{
    if(thread != NULL) {
        osThreadTerminate(thread);
    }
}
