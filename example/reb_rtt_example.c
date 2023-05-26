#include "revent_bus.h"

void reb_recv_handler(const char *event, void *payload, uint16_t lenght)
{
    rt_kprintf("Recv: event: %s, payload: %.*s\r\n", event, lenght, payload);
}

int reb_test(void)
{
    reb_init();

    reb_subscribe("event1", reb_recv_handler);
    reb_subscribe("event2", reb_recv_handler);

    reb_info_dump();

    while (1)
    {
        reb_publish("event1", "Rice: event bus", rt_strlen("Rice: event bus"));
        reb_publish("event2", "Publish sync msg", rt_strlen("Publish sync msg"));
        rt_thread_delay(100);
        reb_async_publish("event1", REB_PRIORITY_ORDINARY, "Good Good", rt_strlen("Good Good"));
        reb_async_publish("event2", REB_PRIORITY_ORDINARY, "Publish async msg", rt_strlen("Publish async msg"));
        rt_thread_delay(100);
    }
}
INIT_COMPONENT_EXPORT(reb_test);
