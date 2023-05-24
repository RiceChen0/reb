#include "reventbus_def.h"
#include "rtthread.h"

reb_task_id reb_task_create(reb_task_func func, void *arg, const struct reb_task_attr *attr)
{
    rt_thread_t thread = NULL;

    thread = rt_thread_create(attr->name, func, arg, attr->stack_size, attr->priority, 20);
    rt_thread_startup(thread);

    return (reb_task_id)thread;
}

void reb_task_delete(reb_task_id thread)
{
    rt_thread_delete((rt_thread_t)thread);
}
