#include "reventbus_def.h"
#include "cmsis_os2.h"

reb_mutex_id reb_mutex_create(void)
{
    osMutexId_t mutex = NULL;
    mutex = osMutexNew(NULL);

    return (reb_mutex_id)mutex;
}

reb_err_t reb_mutex_lock(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return RALARM_EINVAL;
    }
    if(osMutexAcquire((osMutexId_t)mutex, osWaitForever) == osOK) {
        return RALARM_EOK;
    }
    return RALARM_ERROR;
}

reb_err_t reb_mutex_unlock(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return RALARM_EINVAL;
    }
    if(osMutexRelease((osMutexId_t)mutex) == osOK) {
        return RALARM_EOK;
    }
    return RALARM_ERROR;
}

void reb_mutex_delete(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return;
    }
    osMutexDelete(mutex);
}
