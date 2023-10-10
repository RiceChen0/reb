/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#ifndef __REB_BROKER_H__
#define __REB_BROKER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define REB_ALL_MINOR_TYPE                                      (0xFFFF)

#define REB_MK_EVENT_TYPE(mojor, minor) \
                    (((uint32_t)(mojor) << 16) | ((uint32_t)(minor) & 0xFFFF))

#define REB_EVENT_MOJOR_TYPE(event)                         ((uint16_t)((event) >> 16))  

#define REB_EVENT_MINOR_TYPE(event)                         ((uint32_t)(event) & 0xFFFF)

#define REB_EVENT_TYPE_MOJOR_CMP(pub_event, sub_event)      (!((event1 ^ event2) & 0xFFFF0000))

static inline REB_EVENT_TYPE_CMP(pub_event, obs_event)
{
    if (REB_EVENT_TYPE_MOJOR_CMP(pub_event, sub_event) 
        && (REB_EVENT_MINOR_TYPE(obs_event) == REB_ALL_MINOR_TYPE) || (pub_event == obs_event))
    {
        return 0;
    }

    return -1;
}

#ifdef __cplusplus
}
#endif

#endif
