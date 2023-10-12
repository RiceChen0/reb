/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#include "reb_broker.h"
#include "reb_def.h"

static broker_base base;

int broker_create(void)
{
    reb_list_init(&base.obs_head);
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
    obs->state = OBS_STATE_ATTACH_ONCE;
}

int broker_observer_attach(observer_base *obs)
{
    obs->state = OBS_STATE_ATTACH;
}

int broker_observer_detach(observer_base *obs)
{

}
