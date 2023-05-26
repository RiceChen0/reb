#include "platform_def.h"
#include "cmsis_os2.h"

pf_mutex_id pf_mutex_create(void)
{
    osMutexId_t mutex = NULL;
    mutex = osMutexNew(NULL);

    return (pf_mutex_id)mutex;
}

pf_err_t pf_mutex_lock(pf_mutex_id mutex)
{
    if (mutex == NULL) {
        return RALARM_EINVAL;
    }
    if(osMutexAcquire((osMutexId_t)mutex, osWaitForever) == osOK) {
        return RALARM_EOK;
    }
    return RALARM_ERROR;
}

pf_err_t pf_mutex_unlock(pf_mutex_id mutex)
{
    if (mutex == NULL) {
        return RALARM_EINVAL;
    }
    if(osMutexRelease((osMutexId_t)mutex) == osOK) {
        return RALARM_EOK;
    }
    return RALARM_ERROR;
}

void pf_mutex_delete(pf_mutex_id mutex)
{
    if (mutex == NULL) {
        return;
    }
    osMutexDelete(mutex);
}
