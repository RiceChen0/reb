#include "reb_def.h"
#include "rtthread.h"

reb_mutex_id reb_mutex_create(void)
{
    rt_mutex_t mutex = NULL;

    mutex = rt_mutex_create("reb", RT_IPC_FLAG_FIFO);
    return (reb_mutex_id)mutex;
}

reb_status reb_mutex_lock(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return REB_INVAL;
    }
    rt_mutex_take((rt_mutex_t)mutex, RT_WAITING_FOREVER);
    return REB_ERROR;
}

reb_status reb_mutex_unlock(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return REB_INVAL;
    }
    rt_mutex_release((rt_mutex_t)mutex);
    return REB_ERROR;
}

void reb_mutex_delete(reb_mutex_id mutex)
{
    rt_mutex_delete((rt_mutex_t)mutex);
}
