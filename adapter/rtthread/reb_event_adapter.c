#include "reventbus_def.h"
#include "rtthread.h"

reb_event_id reb_event_create(void)
{
    rt_event_t event = NULL;

    event = rt_event_create("reb", RT_IPC_FLAG_FIFO);
    return (reb_event_id)event;
}

uint32_t reb_event_recv(reb_event_id event, uint32_t flags)
{
    uint32_t flag = 0;
    if (event == NULL) {
        return RALARM_ERROR;
    }
    rt_event_recv((rt_event_t)event, flags, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &flag);
    return flag;
}

reb_err_t reb_event_send(reb_event_id event, uint32_t flags)
{
    if (event == NULL) {
        return RALARM_ERROR;
    }
    rt_event_send((rt_event_t)event, flags);

    return RALARM_ERROR;
}

void reb_event_delete(reb_event_id event)
{
    rt_event_delete((rt_event_t)event);
}
