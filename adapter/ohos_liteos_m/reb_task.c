#include "reb_def.h"
#include "los_task.h"

reb_task_id reb_task_create(reb_task_func func, void *arg, reb_task_attr *attr)
{
    UINT32 ret;
    UINT32 task;
    TSK_INIT_PARAM_S param = {
        .pcName = attr->name,
        .pfnTaskEntry = func,
        .uwArg = arg,
        .uwStackSize = attr->stack_size,
        .usTaskPrio = attr->priority,
        .stackAddr = 0,
    };

    ret = LOS_TaskCreate(&task, &param);
    if (ret != LOS_OK) 
    {
        return (reb_task_id)NULL;
    }
    return (reb_task_id)(OS_TCB_FROM_TID(task));    
}

reb_status reb_task_suspend(reb_task_id task)
{
    UINT32 ret;
    if (task == NULL) {
        return REB_INVAL;
    }
    ret = LOS_TaskSuspend(((LosTaskCB *)task)->taskID);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

reb_status reb_task_resume(reb_task_id task)
{
    UINT32 ret;
    if (task == NULL) {
        return REB_INVAL;
    }
    ret = LOS_TaskResume(((LosTaskCB *)task)->taskID);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

void reb_task_delete(reb_task_id task)
{
    if(task == NULL) 
    {
        return;
    }
    LOS_TaskDelete(((LosTaskCB *)task)->taskID);
}
