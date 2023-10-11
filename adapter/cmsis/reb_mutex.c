#include "reb_def.h"
#include "cmsis_os2.h"

reb_mutex_id reb_mutex_create(void)
{
    osMutexId_t mutex = NULL;
    mutex = osMutexNew(NULL);

    return (reb_mutex_id)mutex;
}

reb_status reb_mutex_lock(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return REB_INVAL;
    }
    if(osMutexAcquire((osMutexId_t)mutex, osWaitForever) == osOK) {
        return REB_OK;
    }
    return REB_ERROR;
}

reb_status reb_mutex_unlock(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return REB_INVAL;
    }
    if(osMutexRelease((osMutexId_t)mutex) == osOK) {
        return REB_OK;
    }
    return REB_ERROR;
}

void reb_mutex_delete(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return;
    }
    osMutexDelete(mutex);
}
