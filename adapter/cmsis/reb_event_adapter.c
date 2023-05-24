#include "reventbus_def.h"
#include "cmsis_os2.h"

reb_event_id reb_event_create(void)
{
    osEventFlagsId_t event = NULL;
    event = osEventFlagsNew(NULL);

    return (reb_event_id)event;
}

uint32_t reb_event_recv(reb_event_id event, uint32_t flags)
{
    uint32_t flag = 0;
    if (event == NULL) {
        return RALARM_ERROR;
    }
    flag = osEventFlagsWait((osEventFlagsId_t)event, flags, osFlagsWaitAny, osWaitForever);
    return flag;
}

reb_err_t reb_event_send(reb_event_id event, uint32_t flags)
{
    if (event == NULL) {
        return RALARM_ERROR;
    }
    if((osEventFlagsSet((osEventFlagsId_t)event, flags) & flags) == flags) {
        return RALARM_EOK;
    }
    return RALARM_ERROR;
}

void reb_event_delete(reb_event_id event)
{
    if (event == NULL) {
        return;
    }
    osEventFlagsDelete(event);
}