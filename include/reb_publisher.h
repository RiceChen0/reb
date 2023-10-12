/*
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-10     RiceChen     the first version
 */

#ifndef __REB_PUBLISHER_H__
#define __REB_PUBLISHER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "reb_def.h"

typedef void (*pub_notify)(uint32_t event, uint32_t data, bool free);

reb_status publisher_factory_create(pub_notify notify);

reb_status publisher_send(uint16_t type, uint16_t sub_type,
                          uint32_t data, reb_time_t timeout);

reb_status publisher_send_with_free(uint16_t type, uint16_t sub_type,
                                    uint32_t data, reb_time_t timeout);

reb_status publisher_urgent_send(uint16_t type, uint16_t sub_type,
                                 uint32_t data, reb_time_t timeout);

reb_status publisher_urgent_send_with_free(uint16_t type, uint16_t sub_type,
                                           uint32_t data, reb_time_t timeout);

#ifdef __cplusplus
}
#endif

#endif
