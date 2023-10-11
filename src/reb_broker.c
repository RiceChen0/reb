/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#include "reb_broker.h"
#include "reb_def.h"

static bool reb_brober_event_type_cmp(uint32_t pub_event, uint32_t sub_event)
{
    if (REB_EVENT_TYPE_MOJOR_CMP(pub_event, sub_event) && 
        (REB_EVENT_MINOR_TYPE(obs_event) == REB_ALL_MINOR_TYPE) || (pub_event == obs_event))
    {
        return 0;
    }
    return -1;
}
