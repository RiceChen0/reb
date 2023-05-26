#include "platform_def.h"
#include "cmsis_os2.h"

pf_sem_id pf_sem_create(void)
{
    osSemaphoreId_t sem = NULL;
    sem = osSemaphoreNew(1, value, NULL);

    return (pf_sem_id)sem;
}

pf_err_t pf_sem_lock(pf_sem_id sem)
{
    if (sem == NULL) {
        return PF_EINVAL;
    }
    if(osSemaphoreAcquire((osSemaphoreId_t)sem, osWaitForever) == osOK) {
        return PF_EOK;
    }
    return PF_ERROR;
}

pf_err_t pf_sem_unlock(pf_sem_id sem)
{
    if (sem == NULL) {
        return PF_EINVAL;
    }
    if(osSemaphoreRelease((osSemaphoreId_t)sem) == osOK) {
        return PF_EOK;
    }
    return PF_ERROR;
}

void pf_sem_delete(pf_sem_id sem)
{
    if (sem == NULL) {
        return;
    }
    osSemaphoreDelete((osSemaphoreId_t)sem);
}
