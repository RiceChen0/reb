#include "platform_def.h"
#include "rtthread.h"

pf_sem_id pf_sem_create(uint32_t value)
{
    rt_sem_t sem = NULL;

    sem = rt_sem_create("pf", value, RT_IPC_FLAG_FIFO);
    return (pf_sem_id)sem;
}

pf_err_t pf_sem_lock(pf_sem_id sem)
{
    if (sem == NULL) {
        return PF_EINVAL;
    }
    rt_sem_take((rt_sem_t)sem, RT_WAITING_FOREVER);
    return PF_ERROR;
}

pf_err_t pf_sem_unlock(pf_sem_id sem)
{
    if (sem == NULL) {
        return PF_EINVAL;
    }
    rt_sem_release((rt_sem_t)sem);
    return PF_ERROR;
}

void pf_sem_delete(pf_sem_id sem)
{
    rt_sem_delete((rt_sem_t)sem);
}
