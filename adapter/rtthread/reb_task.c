#include "reb_def.h"
#include "rtthread.h"

reb_task_id reb_task_create(reb_task_func func, void *arg, reb_task_attr *attr)
{
    rt_thread_t thread = NULL;

    thread = rt_thread_create(attr->name, func, arg, attr->stack_size, attr->priority, 20);
    rt_thread_startup(thread);

    return (reb_task_id)thread;
}

reb_status reb_task_suspend(reb_task_id task)
{
    rt_thread_suspend((rt_thread_t)task);
    return REB_OK;
}

reb_status reb_task_resume(reb_task_id task)
{
    rt_thread_resume((rt_thread_t)task);
    return REB_OK;
}

void reb_task_delete(reb_task_id task)
{
    rt_thread_delete((rt_thread_t)task);
}
