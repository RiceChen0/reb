#include "platform_def.h"
#include "rtthread.h"

pf_mutex_id pf_mutex_create(void)
{
    rt_mutex_t mutex = NULL;

    mutex = rt_mutex_create("pf", RT_IPC_FLAG_FIFO);
    return (pf_mutex_id)mutex;
}

pf_err_t pf_mutex_lock(pf_mutex_id mutex)
{
    if (mutex == NULL) {
        return PF_EINVAL;
    }
    rt_mutex_take((rt_mutex_t)mutex, RT_WAITING_FOREVER);
    return PF_ERROR;
}

pf_err_t pf_mutex_unlock(pf_mutex_id mutex)
{
    if (mutex == NULL) {
        return PF_EINVAL;
    }
    rt_mutex_release((rt_mutex_t)mutex);
    return PF_ERROR;
}

void pf_mutex_delete(pf_mutex_id mutex)
{
    rt_mutex_delete((rt_mutex_t)mutex);
}
