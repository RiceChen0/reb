/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#include "reb_broker.h"
#include "reb_def.h"

int broker_create(void)
{

}

int broker_delete(void)
{

}

static bool reb_brober_event_type_cmp(uint32_t pub_event, uint32_t sub_event)
{
    if (REB_EVENT_TYPE_MOJOR_CMP(pub_event, sub_event) && 
        (REB_EVENT_MINOR_TYPE(pub_event) == REB_ALL_MINOR_TYPE) || (pub_event == sub_event))
    {
        return 0;
    }
    return -1;
}

int broker_observer_attach_once(observer_base *obs)
{

}

int broker_observer_attach(observer_base *obs)
{

}

int broker_observer_detach(observer_base *obs)
{

}

int broker_publisher_send(uint16_t type, uint16_t sub_type, void *data, reb_time_t timeout)
{

}

