#include "platform_def.h"
#include "rtthread.h"

pf_task_id pf_task_create(pf_task_func func, void *arg, const struct pf_task_attr *attr)
{
    rt_thread_t thread = NULL;

    thread = rt_thread_create(attr->name, func, arg, attr->stack_size, attr->priority, 20);
    rt_thread_startup(thread);

    return (pf_task_id)thread;
}

void pf_task_delete(pf_task_id thread)
{
    rt_thread_delete((rt_thread_t)thread);
}
