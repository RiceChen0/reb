#include "platform_def.h"
#include "cmsis_os2.h"

pf_task_id pf_task_create(pf_task_func func, void *arg, const struct pf_task_attr *attr)
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
    return (pf_task_id)thread;
}

void pf_task_delete(pf_task_id thread)
{
    if(thread != NULL) {
        osThreadTerminate(thread);
    }
}
