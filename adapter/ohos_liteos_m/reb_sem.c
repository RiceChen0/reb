#include "reb_def.h"
#include "los_sem.h"

reb_sem_id reb_sem_create(uint32_t value)
{
    UINT32 ret;
    UINT32 sem;

    ret = LOS_SemCreate((UINT16)value, &sem);
    if (ret != LOS_OK) 
    {
        return (reb_sem_id)NULL;
    }
    return (reb_sem_id)(GET_SEM(sem));
}

reb_status reb_sem_lock(reb_sem_id sem, reb_time_t timeout)
{
    UINT32 ret;
    if (sem == NULL) {
        return REB_INVAL;
    }
    ret = LOS_SemPend(((LosSemCB *)sem)->semID, LOS_WAIT_FOREVER);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

reb_status reb_sem_unlock(reb_sem_id sem)
{
    UINT32 ret;
    if (sem == NULL) {
        return REB_INVAL;
    }
    ret = LOS_SemPost(((LosSemCB *)sem)->semID);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

void reb_sem_delete(reb_sem_id sem)
{
    if (sem == NULL) {
        return;
    }
    LOS_SemDelete(((LosSemCB *)sem)->semID);
}
