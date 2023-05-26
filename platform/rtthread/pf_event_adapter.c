#include "platform_def.h"
#include "rtthread.h"

pf_event_id pf_event_create(void)
{
    rt_event_t event = NULL;

    event = rt_event_create("pf", RT_IPC_FLAG_FIFO);
    return (pf_event_id)event;
}

uint32_t pf_event_recv(pf_event_id event, uint32_t flags)
{
    uint32_t flag = 0;
    if (event == NULL) {
        return PF_ERROR;
    }
    rt_event_recv((rt_event_t)event, flags, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &flag);
    return flag;
}

pf_err_t pf_event_send(pf_event_id event, uint32_t flags)
{
    if (event == NULL) {
        return PF_ERROR;
    }
    rt_event_send((rt_event_t)event, flags);

    return PF_ERROR;
}

void pf_event_delete(pf_event_id event)
{
    rt_event_delete((rt_event_t)event);
}
