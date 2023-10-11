#include "reb_def.h"
#include "cmsis_os2.h"

reb_sem_id reb_sem_create(uint32_t value)
{
    osSemaphoreId_t sem = NULL;
    sem = osSemaphoreNew(1, value, NULL);

    return (reb_sem_id)sem;
}

reb_status reb_sem_lock(reb_sem_id sem, reb_time_t timeout)
{
    if (sem == NULL) {
        return REB_INVAL;
    }
    if(osSemaphoreAcquire((osSemaphoreId_t)sem, timeout) == osOK) {
        return REB_OK;
    }
    return REB_ERROR;
}

reb_status reb_sem_unlock(reb_sem_id sem)
{
    if (sem == NULL) {
        return REB_INVAL;
    }
    if(osSemaphoreRelease((osSemaphoreId_t)sem) == osOK) {
        return REB_OK;
    }
    return REB_ERROR;
}

void reb_sem_delete(reb_sem_id sem)
{
    if (sem == NULL) {
        return;
    }
    osSemaphoreDelete((osSemaphoreId_t)sem);
}
