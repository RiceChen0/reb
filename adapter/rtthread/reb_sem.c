#include "reb_def.h"
#include "rtthread.h"

reb_sem_id reb_sem_create(uint32_t value)
{
    rt_sem_t sem = NULL;

    sem = rt_sem_create("reb", value, RT_IPC_FLAG_FIFO);
    return (reb_sem_id)sem;
}

reb_status reb_sem_lock(reb_sem_id sem, reb_time_t timeout)
{
    if (sem == NULL) {
        return REB_INVAL;
    }
    rt_sem_take((rt_sem_t)sem, timeout);
    return REB_OK;
}

reb_status reb_sem_unlock(reb_sem_id sem)
{
    if (sem == NULL) {
        return REB_INVAL;
    }
    rt_sem_release((rt_sem_t)sem);
    return REB_OK;
}

void reb_sem_delete(reb_sem_id sem)
{
    rt_sem_delete((rt_sem_t)sem);
}
