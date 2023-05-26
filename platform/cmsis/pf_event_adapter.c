#include "platform_def.h"
#include "cmsis_os2.h"

pf_event_id pf_event_create(void)
{
    osEventFlagsId_t event = NULL;
    event = osEventFlagsNew(NULL);

    return (pf_event_id)event;
}

uint32_t pf_event_recv(pf_event_id event, uint32_t flags)
{
    uint32_t flag = 0;
    if (event == NULL) {
        return RALARM_ERROR;
    }
    flag = osEventFlagsWait((osEventFlagsId_t)event, flags, osFlagsWaitAny, osWaitForever);
    return flag;
}

pf_err_t pf_event_send(pf_event_id event, uint32_t flags)
{
    if (event == NULL) {
        return RALARM_ERROR;
    }
    if((osEventFlagsSet((osEventFlagsId_t)event, flags) & flags) == flags) {
        return RALARM_EOK;
    }
    return RALARM_ERROR;
}

void pf_event_delete(pf_event_id event)
{
    if (event == NULL) {
        return;
    }
    osEventFlagsDelete(event);
}